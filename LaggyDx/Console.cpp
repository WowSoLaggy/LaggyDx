#include "stdafx.h"
#include "Console.h"

#include "ConsoleEvents.h"


namespace Dx
{
  Console& Console::get()
  {
    static Console console;
    return console;
  }


  void Console::print(std::string i_text)
  {
    d_lines.push_back({ std::chrono::system_clock::now(), std::move(i_text) });
    notify(ConsolePrintEvent(d_lines.back()));
  }


  std::span<const ConsoleLine> Console::getLines() const
  {
    return { d_lines };
  }


  void Console::clearLog()
  {
    d_lines.clear();
  }

} // ns Dx
