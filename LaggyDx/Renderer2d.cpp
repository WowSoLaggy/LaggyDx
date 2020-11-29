#include "stdafx.h"
#include "Renderer2d.h"

#include "FontResource.h"
#include "RenderDevice.h"
#include "ResourceController.h"
#include "Sprite.h"
#include "TextureResource.h"

#include <LaggySdk/StringUtils.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  Renderer2d::Renderer2d(IRenderDevice& io_renderDevice)
    : d_renderDevice(io_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);

    d_states = std::make_unique<CommonStates>(renderDevice.getDevicePtr());

    d_primitiveBatch = std::make_shared<PrimitiveBatch<VertexPositionColor>>(renderDevice.getDeviceContextPtr());
    d_primitiveEffect = std::make_unique<BasicEffect>(renderDevice.getDevicePtr());
    d_primitiveEffect->SetVertexColorEnabled(true);

    auto proj = Matrix::CreateOrthographicOffCenter(0.f, float(1600), float(900), 0.f, 0.f, 1.f);
    d_primitiveEffect->SetProjection(proj);

    void const* shaderByteCode;
    size_t byteCodeLength;
    d_primitiveEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    renderDevice.getDevicePtr()->CreateInputLayout(VertexPositionColor::InputElements,
                                                   VertexPositionColor::InputElementCount,
                                                   shaderByteCode, byteCodeLength,
                                                   d_primitiveInputLayout.ReleaseAndGetAddressOf());
  }


  void Renderer2d::beginScene()
  {
    d_renderedSprites = 0;
  }

  void Renderer2d::endScene()
  {
  }


  const Sdk::Vector2I& Renderer2d::getTranslation() const
  {
    return d_translation;
  }

  void Renderer2d::setTranslation(Sdk::Vector2I i_translation)
  {
    d_translation = std::move(i_translation);
  }

  void Renderer2d::resetTranslation()
  {
    d_translation = { 0, 0 };
  }


  int Renderer2d::getRenderedSprites()
  {
    return d_renderedSprites;
  }


  void Renderer2d::renderText(const std::string& i_text,
                              const IFontResource& i_fontResource, const Sdk::Vector2I& i_position)
  {
    const auto& fontResource = dynamic_cast<const FontResource&>(i_fontResource);

    const auto pos = i_position - d_translation;

    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    SpriteBatch textBatch(renderDevice.getDeviceContextPtr());

    textBatch.Begin(SpriteSortMode_Deferred, d_states->NonPremultiplied());
    fontResource.getSpriteFont()->DrawString(&textBatch, Sdk::getWString(i_text).c_str(),
                                             XMFLOAT2((float)pos.x, (float)pos.y));
    textBatch.End();
  }

  void Renderer2d::renderSprite(const Sprite& i_sprite)
  {
    const auto* texture = i_sprite.getTexture();
    if (!texture)
      return;

    const auto& textureResource = dynamic_cast<const TextureResource&>(*texture);

    const auto pos = i_sprite.getPosition() - d_translation;
    const auto& size = i_sprite.getSize();

    const RECT sourceRect = i_sprite.getSourceRect();
    const RECT destinationRect{ pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    const auto& color = i_sprite.getColor();
    const XMVECTORF32 colorVector = { { { color.x, color.y, color.z, color.w } } };

    const XMVECTOR Scaling = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    const XMVECTOR Translation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    const XMVECTOR origin = XMVectorSet((float)(pos.x + size.x / 2), (float)(pos.y + size.y / 2), 0.0f, 0.0f);
    XMMATRIX matrix = XMMatrixAffineTransformation2D(Scaling, origin, (float)i_sprite.getRotation(), Translation);

    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    SpriteBatch spriteBatch(renderDevice.getDeviceContextPtr());
    spriteBatch.Begin(SpriteSortMode_Deferred, d_states->NonPremultiplied(),
                      nullptr, nullptr, nullptr, nullptr,
                      matrix);

    spriteBatch.Draw(textureResource.getTexturePtr(), destinationRect, &sourceRect, colorVector);

    ++d_renderedSprites;

    spriteBatch.End();
  }

  void Renderer2d::renderLine(const Sdk::Vector2I& i_start, const Sdk::Vector2I& i_end,
                              const Sdk::Vector4F& i_color)
  {
    const auto start = i_start - d_translation;
    const auto end = i_end - d_translation;

    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    auto* context = renderDevice.getDeviceContextPtr();

    d_primitiveEffect->Apply(context);
    context->IASetInputLayout(d_primitiveInputLayout.Get());

    d_primitiveBatch->Begin();

    VertexPositionColor p1(XMFLOAT3{ (float)start.x, (float)start.y, 0 },
                           XMFLOAT4{ i_color.x, i_color.y, i_color.z, i_color.w });
    VertexPositionColor p2(XMFLOAT3{ (float)end.x, (float)end.y, 0 },
                           XMFLOAT4{ i_color.x, i_color.y, i_color.z, i_color.w });
    d_primitiveBatch->DrawLine(p1, p2);

    d_primitiveBatch->End();
  }

  void Renderer2d::renderRect(const Sdk::RectI& i_rect, const Sdk::Vector4F& i_color)
  {
    auto rectTranslated = i_rect;
    rectTranslated.move(-d_translation);

    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    auto* context = renderDevice.getDeviceContextPtr();

    d_primitiveEffect->Apply(context);
    context->IASetInputLayout(d_primitiveInputLayout.Get());

    d_primitiveBatch->Begin();

    VertexPositionColor p1(XMFLOAT3{ (float)rectTranslated.left(), (float)rectTranslated.top(), 0 },
                           XMFLOAT4{ i_color.x, i_color.y, i_color.z, i_color.w });
    VertexPositionColor p2(XMFLOAT3{ (float)rectTranslated.right(), (float)rectTranslated.top(), 0 },
                           XMFLOAT4{ i_color.x, i_color.y, i_color.z, i_color.w });
    VertexPositionColor p3(XMFLOAT3{ (float)rectTranslated.right(), (float)rectTranslated.bottom(), 0 },
                           XMFLOAT4{ i_color.x, i_color.y, i_color.z, i_color.w });
    VertexPositionColor p4(XMFLOAT3{ (float)rectTranslated.left(), (float)rectTranslated.bottom(), 0 },
                           XMFLOAT4{ i_color.x, i_color.y, i_color.z, i_color.w });

    d_primitiveBatch->DrawLine(p1, p2);
    d_primitiveBatch->DrawLine(p2, p3);
    d_primitiveBatch->DrawLine(p3, p4);
    d_primitiveBatch->DrawLine(p4, p1);

    d_primitiveBatch->End();
  }

} // ns Dx
