#include <string>
#include "../common/vector.hpp"

namespace smirnova
{
  struct Bank
  {
    std::string name;

    double rate = 1.0;
    long long limit = 0;
    int maxTermMonths = 0;

    double minScore = 0.2;
  };
}

