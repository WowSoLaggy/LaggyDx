#pragma once

#include <unordered_map>


namespace Dx
{
  class IRenderDevice;
  class ICamera;
  class IRenderer;
  class IRenderer2d;
  class IRenderer3d;

  class IResourceController;
  class ILoadableResource;
  class IMeshResourceCmo;
  class MeshResourceCmo;
  class ITextureResource;
  class TextureResource;
  class IPixelShaderResource;
  class PixelShaderResource;
  class IVertexShaderResource;
  class VertexShaderResource;
  class IFontResource;
  class FontResource;

  class VertexBuffer;
  class IndexBuffer;

  struct Material;
  struct MaterialSpan;
  class MaterialSequence;

  class IAnimationController3d;

  class IInputDevice;
  class KeyboardState;
  class MouseState;
  struct MousePosition;

  class Sprite;
  struct ImageDescription;
  struct ImageAnimation;
  using AnimationsMap = std::unordered_map<std::string, ImageAnimation>;
} // ns Dx


struct ID3D11Buffer;


namespace DirectX
{
  class Model;
  class SpriteFont;
  class SpriteBatch;
}
