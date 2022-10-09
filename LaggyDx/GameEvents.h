#pragma once

#include <LaggySdk/IEvent.h>


namespace Dx
{
  class OnGameUpdate : public Sdk::IEvent
  {
  public:
    OnGameUpdate(const double i_dt)
      : d_dt(i_dt)
    {
    }

    double dt() const { return d_dt; }

  private:
    const double d_dt;
  };

} // ns Dx
