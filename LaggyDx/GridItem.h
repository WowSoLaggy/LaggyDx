#pragma once


namespace Dx
{
  class GridItem
  {
  public:
    void setTexture(std::string i_textureName);
    const std::string& getTexture() const;

    void setOnClickHandler(std::function<void()> i_onClickHandler);

    void onClick();

  private:
    std::string d_textureName;
    std::function<void()> d_onClickHandler;
  };

} // ns Dx
