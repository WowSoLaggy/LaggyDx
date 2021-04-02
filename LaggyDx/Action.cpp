#include "stdafx.h"
#include "Action.h"


namespace Dx
{
  Action::Action(std::function<void()> i_fun)
    : d_fun(i_fun)
  {
  }

  void Action::operator()() const
  {
    if (d_fun)
      d_fun();
  }

} // ns Dx
