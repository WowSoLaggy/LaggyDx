#pragma once

#include <unordered_map>


namespace Dx
{
  class RenderDevice;
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
  struct KeyboardKeysState;
  class MouseState;
  struct MousePosition;

  class Sprite;
  class AnimatedSprite;
  struct ImageDescription;
  struct ImageAnimation;
  using AnimationsMap = std::unordered_map<std::string, ImageAnimation>;

  class Game;
  struct GameSettings;

  class IObject;
  class ObjectBase;

  class IShape;

  class IControl;
} // ns Dx


struct ID3D11Buffer;


namespace DirectX
{
  class Model;
  class SpriteFont;
  class SpriteBatch;
  struct XMMATRIX;

  struct VertexPositionColor;
}
