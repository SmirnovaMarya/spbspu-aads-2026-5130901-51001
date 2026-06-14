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
  
  public:
    CommandContext();
    void loadBanks(const std::string& file);

    Bank* getBank(const std::string& name);
    Map& banks();
  };
}

