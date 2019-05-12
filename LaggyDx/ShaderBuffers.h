#pragma once


struct MatrixBuffer
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX projection;
};


struct LightBuffer
{
  XMFLOAT4 diffuseColor;
  XMFLOAT4 lightColor;
  XMFLOAT3 lightDirection;
  float ambientStrength;
};
