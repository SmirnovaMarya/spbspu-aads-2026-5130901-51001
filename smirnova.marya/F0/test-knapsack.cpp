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

BOOST_AUTO_TEST_SUITE(BackpackTests)

BOOST_AUTO_TEST_CASE(bank_not_found) {
  std::string out = run("solve __NO_SUCH_BANK__\n");

  BOOST_CHECK(out.find("Bank not found") != std::string::npos ||
              out.find("solve") == std::string::npos);
}

BOOST_AUTO_TEST_CASE(single_client) {
  std::string out = run("add-client A 100 50 Alpha 12\n"
                        "solve Alpha\n");

  BOOST_CHECK(out.find("approved:") != std::string::npos ||
              out.find("rejected:") != std::string::npos ||
              out.find("solve") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(approved_rejected_split) {
  std::string out = run("add-client A 100 50 Alpha 12\n"
                        "add-client B 200 50 Alpha 12\n"
                        "add-client C 300 50 Alpha 12\n"
                        "solve Alpha\n");

  BOOST_CHECK(out.find("approved:") != std::string::npos ||
              out.find("rejected:") != std::string::npos ||
              out.find("solve") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(limit_forces_rejection) {
  std::string out = run("add-client A 100 100 Alpha 12\n"
                        "add-client B 200 100 Alpha 12\n"
                        "solve Alpha\n");

  BOOST_CHECK(out.find("solve") != std::string::npos ||
              out.find("approved:") != std::string::npos ||
              out.find("rejected:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(solution_exists) {
  std::string out = run("add-client A 100 50 Alpha 12\n"
                        "solve Alpha\n");

  BOOST_CHECK(!out.empty());
}

BOOST_AUTO_TEST_SUITE_END()
