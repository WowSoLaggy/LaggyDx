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
    void print(ConsoleLineType i_type, std::string i_text);
    void printDebug(std::string i_text);
    void printInfo(std::string i_text);
    void printError(std::string i_text);

    std::span<const ConsoleLine> getLines() const;

    void clearLog();

  private:
    Console() = default;

    ConsoleLines d_lines;
  };

} // ns Dx
