
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../libcatnip/log.hpp"

using namespace catnip;

TEST_CASE("Log","[unit]") {
  LOG("Testing log message", 1);
}
