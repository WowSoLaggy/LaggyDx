#include "stdafx.h"
#include "Renderer2d.h"

#include "FontResource.h"
#include "RenderDevice.h"
#include "ResourceController.h"
#include "Sprite.h"
#include "TextureResource.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  Renderer2d::Renderer2d(IRenderDevice& io_renderDevice, const Sdk::Vector2I i_resolution)
    : d_renderDevice(dynamic_cast<RenderDevice&>(io_renderDevice))
    , d_resolution(std::move(i_resolution))
    , d_spriteBatch(d_renderDevice.getDeviceContextPtr())
  {
    d_states = std::make_unique<CommonStates>(d_renderDevice.getDevicePtr());

    d_primitiveBatch = std::make_shared<PrimitiveBatch<VertexPositionColor>>(d_renderDevice.getDeviceContextPtr());
    d_primitiveEffect = std::make_unique<BasicEffect>(d_renderDevice.getDevicePtr());
    d_primitiveEffect->SetVertexColorEnabled(true);

    auto proj = Matrix::CreateOrthographicOffCenter(0.f, float(d_resolution.x), float(d_resolution.y),
                                                    0.f, 0.f, 1.f);
    d_primitiveEffect->SetProjection(proj);

    void const* shaderByteCode;
    size_t byteCodeLength;
    d_primitiveEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    d_renderDevice.getDevicePtr()->CreateInputLayout(VertexPositionColor::InputElements,
                                                   VertexPositionColor::InputElementCount,
                                                   shaderByteCode, byteCodeLength,
                                                   d_primitiveInputLayout.ReleaseAndGetAddressOf());
  }


  void Renderer2d::beginScene()
  {
    d_spriteBatch.Begin(SpriteSortMode::SpriteSortMode_Deferred, d_states->NonPremultiplied());
  }

  void Renderer2d::beginScene(const Sdk::Vector2I& i_translation,
                              const Sdk::Vector2I& i_scaleOrigin,
                              const Sdk::Vector2D& i_scaling)
  {
    const auto m = XMMatrixTransformation2D(
      { (float)(i_scaleOrigin.x), (float)(i_scaleOrigin.y) }, // scaling origin
      0,
      { (float)i_scaling.x, (float)i_scaling.y },
      { 0, 0 },
      0,
      { (float)i_translation.x, (float)i_translation.y });

    d_spriteBatch.Begin(SpriteSortMode::SpriteSortMode_Deferred, d_states->NonPremultiplied(),
                        nullptr, nullptr, nullptr, nullptr, m);
  }

  void Renderer2d::endScene()
  {
    d_spriteBatch.End();
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


  int Renderer2d::resetRenderedSprites()
  {
    return std::exchange(d_renderedSprites, 0);
  }


  void Renderer2d::renderText(const std::string& i_text,
                              const IFontResource& i_fontResource, const Sdk::Vector2I& i_position)
  {
    const auto& fontResource = dynamic_cast<const FontResource&>(i_fontResource);

    const auto pos = i_position + d_translation;

    fontResource.getSpriteFont()->DrawString(&d_spriteBatch, Sdk::getWString(i_text).c_str(),
                                             XMFLOAT2((float)pos.x, (float)pos.y));
  }

  void Renderer2d::renderSprite(const Sprite& i_sprite)
  {
    const auto* texture = i_sprite.getTexture();
    if (!texture)
      return;

    const auto& textureResource = dynamic_cast<const TextureResource&>(*texture);

    const auto pos = i_sprite.getPosition() + d_translation;
    const auto& size = i_sprite.getSize();

    const RECT sourceRect = i_sprite.getSourceRect();
    const RECT destinationRect{ pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    const auto& color = i_sprite.getColor();
    const XMVECTORF32 colorVector = { { { color.x, color.y, color.z, color.w } } };

    d_spriteBatch.Draw(textureResource.getTexturePtr(), destinationRect, &sourceRect, colorVector);

    ++d_renderedSprites;
  }

  void Renderer2d::renderLine(const Sdk::Vector2I& i_start, const Sdk::Vector2I& i_end,
                              const Sdk::Vector4F& i_color)
  {
    const auto start = i_start + d_translation;
    const auto end = i_end + d_translation;

    auto* context = d_renderDevice.getDeviceContextPtr();

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
    rectTranslated.move(d_translation);

    auto* context = d_renderDevice.getDeviceContextPtr();

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
