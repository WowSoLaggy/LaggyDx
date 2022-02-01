#pragma once


namespace Dx
{
  class Aabb
  {
  public:
    Aabb();
    Aabb(
      float i_x0, float i_x1,
      float i_y0, float i_y1,
      float i_z0, float i_z1);

    void mergeWith(const Aabb& i_other);

    float getMinX() const;
    float getMaxX() const;
    float getMinY() const;
    float getMaxY() const;
    float getMinZ() const;
    float getMaxZ() const;

  private:
    float d_xMin = 0.0f;
    float d_xMax = 0.0f;
    float d_yMin = 0.0f;
    float d_yMax = 0.0f;
    float d_zMin = 0.0f;
    float d_zMax = 0.0f;
  };

} // ns Dx
