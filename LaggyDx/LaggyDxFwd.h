#pragma once


namespace Dx
{
  class IRenderDevice;
  class ICamera;
  class IRenderer;
  class IRenderer2d;
  class IRenderer3d;

  using ResourceId = int;
  const ResourceId ResourceIdEmpty = ResourceId(-1);

  class IResourceController;
  class ILoadableResource;
  class MeshResourceCmo;
  class TextureResource;
  class PixelShaderResource;
  class VertexShaderResource;
  class FontResource;

  class VertexBuffer;
  class IndexBuffer;

  struct Material;
  struct MaterialSpan;
  class MaterialSequence;

  class IAnimationController;

  class IInputDevice;
  class KeyboardState;
  class MouseState;
  struct MousePosition;

  struct Sprite;
} // ns Dx


struct ID3D11Buffer;


namespace DirectX
{
  class Model;
  class SpriteFont;
  class SpriteBatch;
}
