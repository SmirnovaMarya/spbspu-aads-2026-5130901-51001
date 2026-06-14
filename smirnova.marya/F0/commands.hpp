#pragma once

#include <string>
#include <istream>
#include <ostream>

#include "storage.hpp"
#include "bank.hpp"

namespace smirnova
{
  void addClientCommand(CommandContext&, std::istream&, std::ostream&);
  void removeClientCommand(CommandContext&, std::istream&, std::ostream&);

  void listCommand(CommandContext&, std::istream&, std::ostream&);
  void reportCommand(CommandContext&, std::istream&, std::ostream&);
  void loanReportCommand(CommandContext&, std::istream&, std::ostream&);
  void showCommand(CommandContext&, std::istream&, std::ostream&);

  void solveCommand(CommandContext&, std::istream&, std::ostream&);

  void printHelp(std::ostream&);
  void executeCommands(std::istream&, std::ostream&);
}

