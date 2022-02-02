#pragma once

#include "Label.h"
#include "Sprite.h"


namespace Dx
{
  enum class CheckboxState
  {
    Normal,
    HoverUnticked,
    HoverTicked,
    Ticked,
  };

  class Checkbox : public Control
  {
  public:
    void setTextureName(CheckboxState i_state, std::string i_textureName);

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;

    virtual void onMouseMove() override;
    virtual void onMouseClick(MouseKey i_key) override;

    bool isChecked() const;

    void onCheck();
    void setOnCheck(std::function<void(bool)> i_onCheck);

  private:
    Sprite d_sprite;

    CheckboxState d_state = CheckboxState::Normal;
    std::unordered_map<CheckboxState, std::string> d_textures;

    std::function<void(bool)> d_onCheck;

    void updateTexture();
    void setState(CheckboxState i_state);
  };

} // ns Dx
