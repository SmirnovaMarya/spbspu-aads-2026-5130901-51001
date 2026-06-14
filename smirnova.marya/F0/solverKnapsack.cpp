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

  void solveKnapsack(Bank& bank)
  {
    Vector< Item > items;

    for (auto it = bank.clients.begin(); it != bank.clients.end(); ++it)
    {
      const Client& c = *it;

      if (c.loan <= 0 || c.termMonths <= 0)
      {
        continue;
      }

      items.pushBack({c.name, c.loan, c.expectedProfit(bank)});
    }

    for (size_t i = 0; i < items.size(); ++i)
    {
      for (size_t j = i + 1; j < items.size(); ++j)
      {
        double di = items[i].profit / (double)items[i].loan;
        double dj = items[j].profit / (double)items[j].loan;

        if (dj > di)
        {
          auto tmp = items[i];
          items[i] = items[j];
          items[j] = tmp;
        }
      }
    }

    Vector< Item > chosen;
    long long used = 0;

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      if (used + it->loan <= bank.limit)
      {
        used += it->loan;
        chosen.pushBack(*it);
      }
    }
  }
}

