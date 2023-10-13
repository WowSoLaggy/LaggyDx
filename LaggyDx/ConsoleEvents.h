#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/IEvent.h>


namespace Dx
{
  class ConsolePrintEvent : public Sdk::IEvent
  {
  public:
    ConsolePrintEvent(const ConsoleLine& i_line)
      : d_line(i_line)
    {
    }

    const ConsoleLine& getLine() const
    {
      return d_line;
    }

  private:
    const ConsoleLine& d_line;
  };

} // ns Dx
