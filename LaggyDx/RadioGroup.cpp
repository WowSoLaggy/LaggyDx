#include "stdafx.h"
#include "RadioGroup.h"

#include "ControlEvents.h"
#include "RadioButton.h"


namespace Dx
{
  void RadioGroup::processEvent(const Sdk::IEvent& i_event)
  {
    if (const auto* event = dynamic_cast<const RadioButtonCheckEvent*>(&i_event))
      onRadioButtonCheck(event->getControl());

    Layout::processEvent(i_event);
  }


  void RadioGroup::addChild(std::shared_ptr<Sdk::TreeNode> i_child)
  {
    auto* childRadioButton = dynamic_cast<RadioButton*>(i_child.get());
    CONTRACT_ASSERT(childRadioButton);

    if (isAnyChecked())
      childRadioButton->uncheck();

    connectTo(*childRadioButton);

    Layout::addChild(i_child);
  }

  void RadioGroup::removeChild(Sdk::TreeNode& i_child)
  {
    auto* childRadioButton = dynamic_cast<RadioButton*>(&i_child);
    CONTRACT_ASSERT(childRadioButton);

    disconnectFrom(*childRadioButton);

    Layout::removeChild(i_child);
  }


  void RadioGroup::onRadioButtonCheck(const RadioButton& i_radioButton)
  {
    for (auto ctrlPtr : getChildren())
    {
      auto* radioButton = dynamic_cast<RadioButton*>(ctrlPtr.get());
      CONTRACT_ASSERT(radioButton);

      if (radioButton != &i_radioButton)
        radioButton->uncheck();
    }
  }


  bool RadioGroup::isAnyChecked() const
  {
    const auto& children = getChildren();

    return std::any_of(children.begin(), children.end(), [&](const auto ctrlPtr) {
      auto* radioButton = dynamic_cast<RadioButton*>(ctrlPtr.get());
      CONTRACT_ASSERT(radioButton);

      return radioButton->isChecked();
      });
  }

} // ns Dx
