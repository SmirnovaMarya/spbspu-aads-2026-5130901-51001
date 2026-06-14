#include "commands.hpp"
#include "solverKnapsack.hpp"
#include "storage.hpp"
#include "bank.hpp"
#include "client.hpp"
#include <sstream>
#include <iostream>

namespace smirnova
{

  void addClientCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string name, bank;
    long long income, loan;
    int term;

    if (!(in >> name >> income >> loan >> bank >> term))
    {
      out << "Invalid arguments\n";
      return;
    }

    if (term <= 0 || loan <= 0 || income <= 0)
    {
        out << "Invalid arguments\n";
        return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
        out << "Bank not found\n";
        return;
    }

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      if (it->name == name)
      {
        out << "Client already exists\n";
        return;
      }
    }

    b->clients.pushBack(Client(name, loan, income, term));
    out << "<client added>\n";
  }

  void removeClientCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bank, name;

    if (!(in >> bank >> name))
    {
      out << "Invalid arguments\n";
      return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
      out << "Bank not found\n";
      return;
    }

    Vector< Client > tmp;

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      if (it->name != name)
      {
        tmp.pushBack(*it);
      }
    }

    b->clients = std::move(tmp);

    out << "<removed>\n";
  }

  void listCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bank;

    if (!(in >> bank))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
      out << "Bank not found\n";
      return;
    }

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      out << it->name << " ";
    }
    out << "\n";
  }

  void reportCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bankName;

    if (!(in >> bankName))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* bank = ctx.getBank(bankName);
    if (!bank)
    {
      out << "Bank not found\n";
      return;
    }

    solveKnapsack(*bank);

    out << bankName << ":\n"
        << "approved=" << bank->approved.size() << "\n"
        << "rejected=" << bank->rejected.size() << "\n";
  }

  void loanReportCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bankName;

    if (!(in >> bankName))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* bank = ctx.getBank(bankName);
    if (!bank)
    {
      out << "Bank not found\n";
      return;
    }

    solveKnapsack(*bank);

    double profit = 0.0;

    for (auto& name : bank->lastApproved)
    {
      for (auto it = bank->clients.begin(); it != bank->clients.end(); ++it)
      {
        if (it->name == name)
        {
          profit += it->expectedProfit(*bank);
          break;
        }
      }
    }

    out << bankName << ":\n"
        << "profit=" << profit << "\n"
        << "approved=" << bank->lastApproved.size() << "\n";
  }

  void showCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string name, bank;

    if (!(in >> name >> bank))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
      out << "Bank not found\n";
      return;
    }

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      if (it->name == name)
      {
        out << "<client " << name << ">\n"
            << "loan=" << it->loan << "\n"
            << "rate=" << b->rate << "\n"
            << "months=" << it->termMonths << "\n"
            << "total_pay=" << it->loan * b->rate << "\n";
        return;
      }
    }
    out << "Client not found\n";
  }

  void solveCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bankName;

    if (!(in >> bankName))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* bank = ctx.getBank(bankName);
    if (!bank)
    {
      out << "Bank not found\n";
      return;
    }

    solveKnapsack(*bank);

    out << "solve " << bankName << ":\n";

    out << "approved: ";
    for (auto it = bank->clients.begin(); it != bank->clients.end(); ++it)
    {
      bool ok = false;
      for (auto& n : bank->lastApproved)
      {
        if (n == it->name)
        {
          ok = true;
        }
      }

      if (ok)
      {
        out << it->name << " ";
      }
    }

    out << "rejected: ";
    for (auto it = bank->clients.begin(); it != bank->clients.end(); ++it)
    {
      bool ok = false;
      for (auto& n : bank->lastApproved)
      {
        if (n == it->name)
        {
          ok = true;
        }
      }

      if (!ok)
      {
        out << it->name << " ";
      }
    }
    out << "\n";
  }

  void printHelp(std::ostream& out)
  {
    out <<
        "================ BANK SYSTEM =================\n"
        "add-client <name income loan bank term>\n"
        "remove-client <bank name>\n"
        "list <bank>\n"
        "show <name bank>\n"
        "report <bank>\n"
        "loan-report <bank>\n"
        "solve <bank>\n"
        "exit\n"
        "==============================================\n";
  }

  void executeCommands(std::istream& in, std::ostream& out)
  {
    CommandContext ctx;


    for (auto it = ctx.banks().begin(); it != ctx.banks().end(); ++it)
    {
      const auto& bank = it->value;

      out << bank.name
          << " limit=" << bank.limit
          << " rate=" << bank.rate
          << " maxTerm=" << bank.maxTermMonths
          << "\n";
    }

    out << "====================\n\n";

    printHelp(out);

    std::string line;

    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::istringstream ss(line);
      std::string cmd;
      ss >> cmd;

      if (!ss)
      {
        continue;
      }
      if (cmd == "add-client")
      {
        addClientCommand(ctx, ss, out);
      }
      else if (cmd == "remove-client")
      {
        removeClientCommand(ctx, ss, out);
      }

      else if (cmd == "list")
      {
        listCommand(ctx, ss, out);
      }
      else if (cmd == "report")
      {
        reportCommand(ctx, ss, out);
      }
      else if (cmd == "loan-report")
      {
        loanReportCommand(ctx, ss, out);
      }
      else if (cmd == "show")
      {
        showCommand(ctx, ss, out);
      }
      else if (cmd == "solve")
      {
        solveCommand(ctx, ss, out);
      }

      else if (cmd == "exit")
      {
        return;
      }
      else
      {
        out << "Unknown command\n";
      }
    }
  }

}

