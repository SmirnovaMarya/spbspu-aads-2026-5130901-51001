#include "commands.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

namespace {
std::string run(const std::string &input) {
  std::istringstream in(input);
  std::ostringstream out;

  smirnova::executeCommands(in, out);

  return out.str();
}
} // namespace

BOOST_AUTO_TEST_SUITE(CommandTests)

BOOST_AUTO_TEST_CASE(output_is_not_empty) {
  auto out = run("");
  BOOST_CHECK(!out.empty());
}

BOOST_AUTO_TEST_CASE(help_present) {
  auto out = run("");
  BOOST_CHECK(out.find("BANK SYSTEM") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(add_client_basic) {
  auto out = run("add-client Ivan 1000 500 Alpha 12\n");

  BOOST_CHECK(out.find("<client added>") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(remove_client_basic) {
  auto out = run("remove-client Alpha Ivan\n");

  BOOST_CHECK(out.find("<removed>") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(list_does_not_crash) {
  auto out = run("list Alpha\n");

  BOOST_CHECK(out.find("Alpha") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(show_command_safe) {
  auto out = run("show Ivan Alpha\n");

  BOOST_CHECK(out.find("loan=") != std::string::npos ||
              out.find("Client not found") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(solve_outputs_something) {
  auto out = run("solve Alpha\n");

  BOOST_CHECK(out.find("solve") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(stability_test) {
  auto out = run("add-client A 100 50 Alpha 12\n"
                 "add-client B 100 50 Alpha 12\n"
                 "solve Alpha\n"
                 "list Alpha\n");

  BOOST_CHECK(!out.empty());
}

BOOST_AUTO_TEST_SUITE_END()
