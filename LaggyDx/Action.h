#pragma once


namespace Dx
{
  class Action
  {
  public:
    Action() = default;
    Action(std::function<void()> i_fun);

    void operator()() const;

  private:
    std::function<void()> d_fun;
  };

} // ns Dx
