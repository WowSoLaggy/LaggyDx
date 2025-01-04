#include "stdafx.h"
#include "Renderer2d.h"

#include "FontResource.h"
#include "IShape2d.h"
#include "RenderDevice.h"
#include "ResourceController.h"
#include "Sprite.h"
#include "Texture.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  Renderer2d::Renderer2d(IRenderDevice& io_renderDevice, const Sdk::Vector2I i_resolution)
    : d_renderDevice(dynamic_cast<RenderDevice&>(io_renderDevice))
    , d_resolution(std::move(i_resolution))
    , d_spriteBatch(d_renderDevice.getDeviceContextPtr())
    , d_primitiveBatch(d_renderDevice.getDeviceContextPtr())
    , d_primitiveEffect(d_renderDevice.getDevicePtr())
  {
    d_states = std::make_unique<CommonStates>(d_renderDevice.getDevicePtr());

    d_primitiveEffect.SetVertexColorEnabled(true);
    
    auto proj = Matrix::CreateOrthographicOffCenter(0.f, float(d_resolution.x), float(d_resolution.y),
                                                    0.f, 0.f, 1.f);
    d_primitiveEffect.SetProjection(proj);

    void const* shaderByteCode;
    size_t byteCodeLength;
    d_primitiveEffect.GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    d_renderDevice.getDevicePtr()->CreateInputLayout(VertexPositionColor::InputElements,
                                                   VertexPositionColor::InputElementCount,
                                                   shaderByteCode, byteCodeLength,
                                                   d_primitiveInputLayout.ReleaseAndGetAddressOf());
  }


  void Renderer2d::beginScene()
  {
    d_renderDevice.resetState();

    d_spriteBatch.Begin(SpriteSortMode::SpriteSortMode_Deferred, d_states->NonPremultiplied());
    
    d_primitiveEffect.SetWorld(XMMatrixIdentity());
    d_primitiveBatch.Begin();
  }

  void Renderer2d::beginScene(const Sdk::Vector2F& i_translation)
  {
    d_renderDevice.resetState();

    beginScene(i_translation, { 0.0f, 0.0f }, 0.0f, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
  }

  void Renderer2d::beginScene(const Sdk::Vector2F& i_translation,
                              const Sdk::Vector2F& i_rotationOrigin,
                              const float i_rotation)
  {
    d_renderDevice.resetState();


    beginScene(i_translation, i_rotationOrigin, i_rotation, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
  }

  void Renderer2d::beginScene(const Sdk::Vector2F& i_translation,
                              const Sdk::Vector2F& i_scalingOrigin,
                              const Sdk::Vector2F& i_scaling)
  {
    d_renderDevice.resetState();


    beginScene(i_translation, { 0.0f, 0.0f }, 0.0f, i_scalingOrigin, 0, i_scaling);
  }

  void Renderer2d::beginScene(const Sdk::Vector2F& i_translation,
                              const Sdk::Vector2F& i_rotationOrigin,
                              float i_rotation,
                              const Sdk::Vector2F& i_scalingOrigin,
                              float i_scalingOrientation,
                              const Sdk::Vector2F& i_scaling)
  {
    d_renderDevice.resetState();


    const auto m = XMMatrixTransformation2D(
      { i_scalingOrigin.x, i_scalingOrigin.y },   // scaling origin
      i_scalingOrientation,                       // scaling orientation
      { i_scaling.x, i_scaling.y },               // scaling
      { i_rotationOrigin.x, i_rotationOrigin.y }, // rotation origin
      i_rotation,                                 // rotation
      { i_translation.x, i_translation.y }        // translation
    );

    d_spriteBatch.Begin(SpriteSortMode::SpriteSortMode_Deferred, d_states->NonPremultiplied(),
                        nullptr, nullptr, nullptr, nullptr, m);

    d_primitiveEffect.SetWorld(m);
    d_primitiveBatch.Begin();
  }

  void Renderer2d::endScene()
  {
    d_primitiveBatch.End();
    d_spriteBatch.End();
  }


  const Sdk::Vector2F& Renderer2d::getTranslation() const
  {
    return d_translation;
  }

  void Renderer2d::setTranslation(Sdk::Vector2F i_translation)
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
                              const IFontResource& i_fontResource,
                              const Sdk::Vector4F& i_color,
                              float i_scale)
  {
    const auto& fontResource = dynamic_cast<const FontResource&>(i_fontResource);

    fontResource.getSpriteFont()->DrawString(&d_spriteBatch,
                                             Sdk::s2ws(i_text).c_str(),
                                             XMFLOAT2(d_translation.x, d_translation.y),
                                             { i_color.x, i_color.y, i_color.z, i_color.w },
                                             0,
                                             { 0, 0 },
                                             i_scale);
  }

  void Renderer2d::renderSprite(const Sprite& i_sprite)
  {
    const auto* texture = i_sprite.getTexture();
    if (!texture)
      return;

    const auto pos = i_sprite.getPosition() + Sdk::Vector2I{ (int)d_translation.x, (int)d_translation.y };
    const auto& size = i_sprite.getSize();

    const RECT destinationRect{ pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    const auto& color = i_sprite.getColor();

    const auto sourceRect = i_sprite.getSourceRect();

    const float rotation = (float)i_sprite.getRotation();
    const auto& rotationOrigin = i_sprite.getRotationOrigin();

    d_spriteBatch.Draw(texture->getTexturePtr(),
      destinationRect, &sourceRect,
      { color.x, color.y, color.z, color.w },
      rotation, { rotationOrigin.x, rotationOrigin.y });

    ++d_renderedSprites;
  }

  void Renderer2d::renderLine(const Sdk::Vector2F& i_start, const Sdk::Vector2F& i_end,
                              const Sdk::Vector4F& i_color)
  {
    const auto start = i_start + d_translation;
    const auto end = i_end + d_translation;

    auto* context = d_renderDevice.getDeviceContextPtr();

    d_primitiveEffect.SetColorAndAlpha({ i_color.x, i_color.y, i_color.z, i_color.w });
    d_primitiveEffect.Apply(context);
    context->IASetInputLayout(d_primitiveInputLayout.Get());

    VertexPositionColor p1(XMFLOAT3{ start.x, start.y, 0 },
                           XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
    VertexPositionColor p2(XMFLOAT3{ end.x, end.y, 0 },
                           XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
    d_primitiveBatch.DrawLine(p1, p2);
  }

  void Renderer2d::renderRect(const Sdk::RectI& i_rect, const Sdk::Vector4F& i_color)
  {
    auto rectTranslated = i_rect;
    rectTranslated.move<int>({ (int)d_translation.x, (int)d_translation.y });

    auto* context = d_renderDevice.getDeviceContextPtr();

    d_primitiveEffect.SetColorAndAlpha({ i_color.x, i_color.y, i_color.z, i_color.w });
    d_primitiveEffect.Apply(context);
    context->IASetInputLayout(d_primitiveInputLayout.Get());

    VertexPositionColor p1(XMFLOAT3{ (float)rectTranslated.left(), (float)rectTranslated.top(), 0 },
                           XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
    VertexPositionColor p2(XMFLOAT3{ (float)rectTranslated.right(), (float)rectTranslated.top(), 0 },
                           XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
    VertexPositionColor p3(XMFLOAT3{ (float)rectTranslated.right(), (float)rectTranslated.bottom(), 0 },
                           XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
    VertexPositionColor p4(XMFLOAT3{ (float)rectTranslated.left(), (float)rectTranslated.bottom(), 0 },
                           XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

    d_primitiveBatch.DrawLine(p1, p2);
    d_primitiveBatch.DrawLine(p2, p3);
    d_primitiveBatch.DrawLine(p3, p4);
    d_primitiveBatch.DrawLine(p4, p1);
  }

} // ns Dx
