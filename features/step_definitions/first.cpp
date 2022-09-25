#include <boost/test/unit_test.hpp>

// Pretend to be GTest
#define EXPECT_EQ BOOST_CHECK_EQUAL

#include <cucumber-cpp/autodetect.hpp>

#include "elrat/clp/descriptors.hpp"


using cucumber::ScenarioScope;

struct Context
{
  bool result;
};

GIVEN("^I have passed (.+) as parameter$") {
  REGEX_PARAM(std::string,arg);
  ScenarioScope<Context> ctx;
  ctx->result = elrat::clp::ParameterType::NaturalNumber(arg);
}

THEN("^(.+) should be returned$") {
  REGEX_PARAM(bool,expected);
  ScenarioScope<Context> ctx;
  EXPECT_EQ(expected, ctx->result);
}
