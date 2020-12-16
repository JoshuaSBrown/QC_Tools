
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "elements.hpp"

using namespace catnip;

TEST_CASE("Elements","[unit]") {

  Elements elements;
  Element elem = Element::H;
  REQUIRE( elem == elements.getElement("H"));
  REQUIRE( elements.getString(elem) == "H");
  REQUIRE( elem == elements.getElement("h"));

  Element unknown = Element::Unknown;
  REQUIRE( unknown == elements.getElement("blah"));
  REQUIRE( "Unknown" == elements.getString(unknown));
}
