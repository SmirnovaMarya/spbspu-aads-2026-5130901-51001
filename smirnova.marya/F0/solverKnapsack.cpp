#include "solverKnapsack.hpp"
#include "client.hpp"
#include <limits>

namespace smirnova
{
  struct Item
  {
    std::string name;
    long long loan;
    double profit;
  };

  static double totalProfit(const Vector< Item >& items)
  {
    double sum = 0;
    for (auto it = items.begin(); it != items.end(); ++it)
    {
      sum += it->profit;
    }
    return sum;
  }
}

