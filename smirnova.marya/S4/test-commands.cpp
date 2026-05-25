#include <functional>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "commands.hpp"

namespace
{
  std::string captureStdout(const std::function< void() >& action)
  {
    std::ostringstream buffer;
    auto* old = std::cout.rdbuf(buffer.rdbuf());

    try
    {
      action();
    }
    catch (...)
    {
      std::cout.rdbuf(old);
      throw;
    }

    std::cout.rdbuf(old);
    return buffer.str();
  }

  smirnova::Dict makeDict(std::initializer_list< std::pair< int, std::string > > items)
  {
    smirnova::Dict dict;

    for (const auto& item : items)
    {
      dict.push(item.first, item.second);
    }

    return dict;
  }

  smirnova::DatasetTree makeDatasets()
  {
    smirnova::DatasetTree datasets;
    datasets.push("a", makeDict({{1, "one"}, {2, "two"}, {3, "three"}}));
    datasets.push("b", makeDict({{2, "two"}, {4, "four"}}));
    return datasets;
  }
}

BOOST_AUTO_TEST_CASE(print_dataset_test)
{
  smirnova::DatasetTree datasets;
  datasets.push("empty", smirnova::Dict{});
  datasets.push("filled", makeDict({{1, "one"}, {2, "two"}}));

  BOOST_TEST(captureStdout([&]() {
    std::istringstream input("print empty");
    smirnova::executeCommand(datasets, input);
  }) == std::string{"<EMPTY>\n"});

  BOOST_TEST(captureStdout([&]() {
    std::istringstream input("print filled");
    smirnova::executeCommand(datasets, input);
  }) == std::string{"filled 1 one 2 two\n"});
}

BOOST_AUTO_TEST_CASE(set_operations_test)
{
  auto datasets = makeDatasets();

  {
    std::istringstream input("complement only_a a b");
    smirnova::executeCommand(datasets, input);
  }

  BOOST_TEST(datasets.contains("only_a"));
  BOOST_TEST(datasets.get("only_a").size() == 2u);
  BOOST_TEST(datasets.get("only_a").get(1) == "one");
  BOOST_TEST(datasets.get("only_a").get(3) == "three");

  {
    std::istringstream input("intersect common a b");
    smirnova::executeCommand(datasets, input);
  }

  BOOST_TEST(datasets.contains("common"));
  BOOST_TEST(datasets.get("common").size() == 1u);
  BOOST_TEST(datasets.get("common").get(2) == "two");

  {
    std::istringstream input("union merged a b");
    smirnova::executeCommand(datasets, input);
  }

  BOOST_TEST(datasets.contains("merged"));
  BOOST_TEST(datasets.get("merged").size() == 4u);
  BOOST_TEST(datasets.get("merged").get(1) == "one");
  BOOST_TEST(datasets.get("merged").get(2) == "two");
  BOOST_TEST(datasets.get("merged").get(3) == "three");
  BOOST_TEST(datasets.get("merged").get(4) == "four");
}

BOOST_AUTO_TEST_CASE(invalid_command_test)
{
  auto datasets = makeDatasets();
  std::istringstream input("unknown command");

  BOOST_CHECK_THROW(smirnova::executeCommand(datasets, input), std::runtime_error);
}

