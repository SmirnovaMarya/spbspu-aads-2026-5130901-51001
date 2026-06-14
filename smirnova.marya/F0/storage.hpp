#pragma once

#include <string>
#include "CuckooHashTable.hpp"
#include "bank.hpp"

namespace smirnova
{
  class CommandContext
  {
  public:
    using Map = CuckooHashTable<std::string, Bank>;

  private:
    Map banks_;
  };
}

