#pragma once

#include <unordered_map>


namespace Dx
{
  class RenderDevice;
  class IRenderDevice;
  class ICamera;
  class FirstPersonCamera;
  class ThirdPersonCamera;
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
  class IInputController;

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
  class IShape3d;
  class Shape3d;

  class IControl;
  class Label;
  class Panel;
  class Layout;
  class Button;
  class Checkbox;
  class Slider;
  class RadioButton;
  class RadioGroup;

  class IGuiEffect;

  struct CollisionPointNormal;
  struct CollisionInfo;

  class ICollider;

  struct Animation;

  class IModel;
  class Model;
  class Mesh;

  class Aabb;

} // ns Dx


struct ID3D11Buffer;
struct ID3D10Blob;

struct ID3D11PixelShader;
struct ID3D11SamplerState;
struct ID3D11VertexShader;
struct ID3D11InputLayout;


namespace DirectX
{
  struct XMMATRIX;
}


namespace ofbx
{
  struct IScene;

} // ns ofbx
