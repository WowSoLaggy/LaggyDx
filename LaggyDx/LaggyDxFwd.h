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
  class ITexture;
  class Texture;
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
  class Channels;
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
  class IMesh;
  class Mesh;

  class Aabb;

  class Roam;
  class Tri;

  class IBitmap;

  class HeightMap;

} // ns Dx


struct ID3D11Buffer;
struct ID3D10Blob;

struct ID3D11PixelShader;
struct ID3D11SamplerState;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D11ShaderResourceView;

struct D3D11_TEXTURE2D_DESC;


namespace DirectX
{
  struct XMMATRIX;
}


namespace ofbx
{
  struct IScene;

} // ns ofbx
