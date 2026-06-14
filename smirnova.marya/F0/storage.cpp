#include "storage.hpp"

namespace smirnova
{
  CommandContext::CommandContext()
  {
    loadBanks();
  }

  void CommandContext::loadBanks()
  {
    Bank b;

    b.name = "Sberbank";
    b.limit = 2000000;
    b.rate = 1.12;
    b.maxTermMonths = 60;
    banks_.insert(b.name, b);

    b = Bank{};
    b.name = "VTB";
    b.limit = 3000000000LL;
    b.rate = 1.13;
    b.maxTermMonths = 60;
    banks_.insert(b.name, b);

    b = Bank{};
    b.name = "Tinkoff";
    b.limit = 2000000000LL;
    b.rate = 1.18;
    b.maxTermMonths = 6;
    banks_.insert(b.name, b);
  }

  Bank* CommandContext::getBank(const std::string& name)
  {
    return banks_.get(name);
  }

  CommandContext::Map& CommandContext::banks()
  {
    return banks_;
  }
}

