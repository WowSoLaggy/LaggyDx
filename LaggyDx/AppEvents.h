#pragma once

#include <LaggySdk/IEvent.h>


namespace Dx
{
  class OnUpdate : public Sdk::IEvent
  {
  public:
    OnUpdate(const double i_dt)
      : d_dt(i_dt)
    {
    }

    double dt() const { return d_dt; }

  private:
    const double d_dt;
  };

} // ns Dx
