#include <string>

namespace smirnova
{
  struct Bank;
  class Client
  {
  public:
    std::string name;
    long long loan;
    long long income;
    int termMonths;
  };
}

