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


  void Console::print(const ConsoleLineType i_type, std::string i_text)
  {
    d_lines.push_back({ std::chrono::system_clock::now(), i_type, std::move(i_text) });
    notify(ConsolePrintEvent(d_lines.back()));
  }

  void Console::printDebug(std::string i_text)
  {
    print(ConsoleLineType::Debug, std::move(i_text));
  }

  void Console::printInfo(std::string i_text)
  {
    print(ConsoleLineType::Info, std::move(i_text));
  }

  void Console::printError(std::string i_text)
  {
    print(ConsoleLineType::Error, std::move(i_text));
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
