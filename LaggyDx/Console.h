#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/EventHandler.h>


namespace Dx
{
  class Console : public Sdk::EventHandler
  {
  public:
    static Console& get();

  public:
    void print(std::string i_text);

    std::span<const ConsoleLine> getLines() const;

    void clearLog();

  private:
    ConsoleLines d_lines;
  };

} // ns Dx
