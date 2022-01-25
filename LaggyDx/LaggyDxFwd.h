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
  class ISimpleRenderer;

  class IResourceController;
  class ILoadableResource;
  class IMeshResourceCmo;
  class MeshResourceCmo;
  class ITextureResource;
  class TextureResource;
  class IPsResource;
  class PsResource;
  class IVsResource;
  class VsResource;
  class IFontResource;
  class FontResource;
  class IFbxResource;

  class VertexBuffer;
  class IndexBuffer;

  struct Material;
  struct MaterialSpan;
  class IMaterialSequence;
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
  using Animations2Map = std::unordered_map<std::string, ImageAnimation>;

  class Game;
  struct GameSettings;

  class IObject;
  class IObject2;
  class Object2Base;
  class IObject3;
  class Object3;

  class IShape;

  class IControl;
  class Label;

  class IGuiEffect;

  struct CollisionPointNormal;
  struct CollisionInfo;

  class ICollider;

  struct Animation;

} // ns Dx


struct ID3D11Buffer;
struct ID3D10Blob;


namespace DirectX
{
  class Model;
  class SpriteFont;
  class SpriteBatch;
  struct XMMATRIX;

  struct VertexPositionColor;

} // ns DirectX

namespace ofbx
{
  struct IScene;

} // ns ofbx
