#include "stdafx.h"
#include "TerrainShader.h"

#include "ICamera3.h"
#include "IndexBuffer.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShadersUtils.h"
#include "ShadowCamera.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

#include "Generated/Terrain.gen.ps.h"
#include "Generated/Terrain.gen.vs.h"


namespace Dx
{
  TerrainShader::TerrainShader(const ICamera3& i_camera)
    : d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_cameraBuffer(getRenderDevice(), sizeof(CameraDesc))
    , d_lightBuffer(getRenderDevice(), sizeof(LightDesc))
    , d_shadowMatrixBuffer(getRenderDevice(), sizeof(ShadowCascadesDesc))
    , d_gridBuffer(getRenderDevice(), sizeof(GridDesc))
    , d_paintBuffer(getRenderDevice(), sizeof(TerrainPaintDesc))
    , d_camera(i_camera)
    , d_grassTexture(getResourceController().getTexture("grass.png"))
    , d_defaultTexture(getResourceController().getTexture("white.png"))
  {
    std::fill(std::begin(d_shadowMapTextures), std::end(d_shadowMapTextures), &d_defaultTexture);

    getShaders().initVs(g_terrainVs, sizeof(g_terrainVs), getVertexLayoutPos3NormText());
    getShaders().initPs(g_terrainPs, sizeof(g_terrainPs));
    getShaders().addSampler(true);
    getShaders().addComparisonSampler();
  }


  void TerrainShader::setLightDirection(Sdk::Vector3D i_direction)
  {
    d_lightDesc.lightDirection = getXmfloat3Norm(std::move(i_direction));
  }

  void TerrainShader::setLightColor(const Sdk::Vector4D& i_color)
  {
    d_lightDesc.lightColor = getXmfloat4(i_color);
  }

  void TerrainShader::setAmbientStrength(const double i_strength)
  {
    d_lightDesc.ambientStrength = (float)i_strength;
  }

  void TerrainShader::setShadowCascade(const int i_cascade, const ShadowCamera& i_camera, const ITexture& i_map)
  {
    CONTRACT_EXPECT(i_cascade >= 0 && i_cascade < c_shadowCascadesCount);
    d_shadowCameras[i_cascade] = &i_camera;
    d_shadowMapTextures[i_cascade] = &i_map;
  }

  void TerrainShader::setGridCellSize(const double i_cellSize)
  {
    d_gridCellSize = (float)i_cellSize;
  }

  void TerrainShader::setPaintLayers(const std::vector<TerrainPaintLayer>& i_layers,
    const std::vector<const ITexture*>& i_masks, const Sdk::Vector2D& i_mapWorldSize)
  {
    const int layersCount = static_cast<int>(i_layers.size());
    CONTRACT_EXPECT(layersCount <= c_maxTerrainPaintLayers);
    CONTRACT_EXPECT(static_cast<int>(i_masks.size()) == (layersCount + 3) / 4);
    CONTRACT_EXPECT(i_mapWorldSize.x > 0 && i_mapWorldSize.y > 0);
    for (const auto& layer : i_layers)
      CONTRACT_EXPECT(layer.texture && layer.tile > 0);
    for (const auto* maskPtr : i_masks)
      CONTRACT_EXPECT(maskPtr);

    std::fill(std::begin(d_paintLayers), std::end(d_paintLayers), TerrainPaintLayer{});
    std::copy(i_layers.cbegin(), i_layers.cend(), std::begin(d_paintLayers));
    std::fill(std::begin(d_paintMaskTextures), std::end(d_paintMaskTextures), nullptr);
    std::copy(i_masks.cbegin(), i_masks.cend(), std::begin(d_paintMaskTextures));

    d_invMapSize = { (float)(1.0 / i_mapWorldSize.x), (float)(1.0 / i_mapWorldSize.y) };
  }


  void TerrainShader::draw(const IObject3& i_object) const
  {
    if (!isEnabled())
      return;

    if (!i_object.getVisible())
      return;

    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setCBuffers();
    setShadowCBuffer();
    setGridCBuffer();
    setPaintCBuffer();
    setTextures();

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials())
      {
        setMaterial(materialSpan.material);
        drawIndexed(materialSpan.count, materialSpan.startIndex);
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void TerrainShader::setGeometryBuffers(const IMesh& i_mesh) const
  {
    auto* vbPtr = i_mesh.getVertexBuffer().getPtr();
    unsigned int stride = i_mesh.getVertexBuffer().getStride();
    auto* ibPtr = i_mesh.getIndexBuffer().getPtr();
    unsigned int offset = 0;

    getRenderDevice().getDeviceContextPtr()->IASetVertexBuffers(0, 1, &vbPtr, &stride, &offset);
    getRenderDevice().getDeviceContextPtr()->IASetIndexBuffer(ibPtr, DXGI_FORMAT_R32_UINT, 0);

    const auto topology = i_mesh.getTopology() == Topology::TriangleList
      ? D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
      : D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

    getRenderDevice().getDeviceContextPtr()->IASetPrimitiveTopology(topology);
  }

  void TerrainShader::setXfmMatrices(const IObject3& i_object) const
  {
    auto getWorldMatrixTransposed = [&]()
    {
      const auto& position = i_object.getPosition();
      const auto& rotation = i_object.getRotation();
      const auto& scale = i_object.getScale();
      const auto worldMatrix =
        XMMatrixScaling(scale.x, scale.y, scale.z) *
        XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
        XMMatrixTranslation(position.x, position.y, position.z);

      return XMMatrixTranspose(worldMatrix);
    };

    const auto worldMatrix = getWorldMatrixTransposed();
    const auto viewMatrix = XMMatrixTranspose(d_camera.getViewMatrix());
    const auto projectionMatrix = XMMatrixTranspose(d_camera.getProjectionMatrix());

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (WorldViewProj*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void TerrainShader::setCBuffers() const
  {
    // Camera
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_cameraBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (CameraDesc*)mappedResource.pData;
      dataPtr->cameraPos = getXmfloat3(d_camera.getPosition());

      getRenderDevice().getDeviceContextPtr()->Unmap(d_cameraBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(1, 1, d_cameraBuffer.getPp());
    }
  }

  void TerrainShader::setShadowCBuffer() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_shadowMatrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (ShadowCascadesDesc*)mappedResource.pData;
    for (int i = 0; i < c_shadowCascadesCount; ++i)
    {
      // Identity fallback keeps receivers fully lit until a cascade is wired
      dataPtr->lightViewProj[i] = d_shadowCameras[i]
        ? XMMatrixTranspose(d_shadowCameras[i]->getViewMatrix() * d_shadowCameras[i]->getProjectionMatrix())
        : XMMatrixIdentity();
    }

    getRenderDevice().getDeviceContextPtr()->Unmap(d_shadowMatrixBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(2, 1, d_shadowMatrixBuffer.getPp());
  }

  void TerrainShader::setGridCBuffer() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_gridBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (GridDesc*)mappedResource.pData;
    dataPtr->cellSize = d_gridCellSize;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_gridBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(1, 1, d_gridBuffer.getPp());
  }

  void TerrainShader::setPaintCBuffer() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_paintBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (TerrainPaintDesc*)mappedResource.pData;
    dataPtr->invMapSize = d_invMapSize;
    dataPtr->_reserved = { 0.0f, 0.0f };

    for (int i = 0; i < c_maxTerrainPaintLayers; ++i)
    {
      const auto& layer = d_paintLayers[i];
      auto& desc = dataPtr->layers[i];
      desc.tile = layer.texture ? (float)layer.tile : 1.0f;
      // Coarse tile falls back to the detail tile so the always-executed coarse sample's UVs stay finite
      desc.tileCoarse = layer.tileCoarse > 0 ? (float)layer.tileCoarse : desc.tile;
      desc.coarseMix = layer.tileCoarse > 0 ? (float)layer.coarseMix : 0.0f;
      desc.triplanar = layer.triplanar ? 1.0f : 0.0f;
      desc.tint = { (float)layer.tint.x, (float)layer.tint.y, (float)layer.tint.z };
      desc.enabled = layer.texture ? 1.0f : 0.0f;
    }

    getRenderDevice().getDeviceContextPtr()->Unmap(d_paintBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(2, 1, d_paintBuffer.getPp());
  }

  void TerrainShader::setTextures() const
  {
    constexpr int paintSlot = 1 + c_shadowCascadesCount;
    constexpr int maskSlot = paintSlot + c_maxTerrainPaintLayers;
    constexpr int srvsCount = maskSlot + c_terrainPaintMasksCount;

    ID3D11ShaderResourceView* srvs[srvsCount] = {};
    srvs[0] = d_grassTexture.getTexturePtr();
    for (int i = 0; i < c_shadowCascadesCount; ++i)
      srvs[1 + i] = d_shadowMapTextures[i]->getTexturePtr();
    for (int i = 0; i < c_maxTerrainPaintLayers; ++i)
      srvs[paintSlot + i] = (d_paintLayers[i].texture ? *d_paintLayers[i].texture : d_defaultTexture).getTexturePtr();
    for (int i = 0; i < c_terrainPaintMasksCount; ++i)
      srvs[maskSlot + i] = (d_paintMaskTextures[i] ? *d_paintMaskTextures[i] : d_defaultTexture).getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, srvsCount, srvs);
  }

  void TerrainShader::setMaterial(const Material& i_material) const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_lightBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (LightDesc*)mappedResource.pData;
    *dataPtr = d_lightDesc;
    dataPtr->diffuseColor = XMFLOAT4(
      i_material.diffuseColor.x,
      i_material.diffuseColor.y,
      i_material.diffuseColor.z,
      i_material.diffuseColor.w);
    dataPtr->specularIntensity = i_material.specularIntensity;
    dataPtr->specularPower = i_material.specularPower;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_lightBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(0, 1, d_lightBuffer.getPp());
  }

} // ns Dx
