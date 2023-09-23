#pragma once


namespace Dx
{
  class GridItem
  {
  public:
    virtual ~GridItem() = default;

    void setTextureName(std::string i_textureName);
    const std::string& getTextureName() const;

  private:
    std::string d_textureName;
  };

} // ns Dx
