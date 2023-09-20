#pragma once

#include <unordered_map>


namespace Dx
{
  class RenderDevice;
  class IRenderDevice;
  class ICamera2;
  class ICamera3;
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
  class MemoryTexture;
  class IPsResource;
  class PsResource;
  class IVsResource;
  class VsResource;
  class IFontResource;
  class FontResource;
  class IFbxResource;

  class VertexBuffer;
  class IndexBuffer;

  class VertexLayout;

  struct Material;
  struct MaterialSpan;

  class IAnimationController3d;

  class IInputDevice;
  class KeyboardState;
  struct KeyboardKeysState;
  class MouseState;
  struct MousePosition;
  class IInputController;

  class ISprite;
  class Sprite;
  class AnimatedSprite;
  struct ImageDescription;
  struct ImageAnimation;
  class Channels;
  using Animations2Map = std::unordered_map<std::string, ImageAnimation>;

  class App;
  struct AppSettings;

  class IObject;
  class IObject2;
  class Object2Base;
  class IObject3;
  class Object3;

  class IShape2d;
  class Shape2d;
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
  class Grid;
  class GridItem;
  using GridItems = std::vector<std::shared_ptr<GridItem>>;

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

  struct RefreshRate;

  class ISpriteShader;

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
