#include "storage.hpp"
#include <fstream>

namespace smirnova
{
  CommandContext::CommandContext()
  {
    loadBanks("banks.txt");
  }
  void CommandContext::loadBanks(const std::string& file)
  {
    std::ifstream in(file);

    std::string name;
    long long limit;
    double rate;
    int maxTerm;

    while (in >> name >> limit >> rate >> maxTerm)
    {
      if (rate <= 0 || limit < 0 || maxTerm <= 0)
      {
        continue;
      }

      Bank b;
      b.name = name;
      b.limit = limit;
      b.rate = rate;
      b.maxTermMonths = maxTerm;

      banks_.insert(name, b);
    }
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

