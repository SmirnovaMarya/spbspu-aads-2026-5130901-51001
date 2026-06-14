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

    bool improved = true;

    while (improved)
    {
      improved = false;
      for (size_t i = 0; i < chosen.size(); ++i)
      {
        for (size_t j = 0; j < items.size(); ++j)
        {
          const auto& out = chosen[i];
          const auto& in = items[j];
          long long newUsed = used - out.loan + in.loan;
          if (newUsed <= bank.limit)
          {
            double oldProfit = totalProfit(chosen);

            Vector< Item > test = chosen;
            test[i] = in;

            double newProfit = totalProfit(test);

            if (newProfit > oldProfit)
            {
              chosen = test;
              used = newUsed;
              improved = true;
            }
          }
        }
      }
    }
    bank.lastApproved = {};
    bank.approved = {};
    bank.rejected = {};

    for (auto it = chosen.begin(); it != chosen.end(); ++it)
    {
      bank.lastApproved.pushBack(it->name);
    }

    for (auto it = bank.clients.begin(); it != bank.clients.end(); ++it)
    {
      bool ok = false;
      for (auto& n : bank.lastApproved)
      {
        if (n == it->name)
        {
          ok = true;
        }
      }

      if (ok)
      {
        bank.approved.pushBack(it->name);
      }
      else
      {
        bank.rejected.pushBack(it->name);
      }
    }
  }
}

