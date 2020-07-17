
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Local private includes
#include "../libcatnip/atom.hpp"

#include "../libcatnip/elements.hpp"

using namespace catnip;

TEST_CASE("Elements","[unit]") {

  Elements elements;
  Element H = elements.getElement("H");

  Atom atom(H,0.0,0.0,0.0);
  Atom atom2(H,1.0,0.0,0.0);
  
  REQUIRE( atom != atom2);
  REQUIRE( atom == atom);
  REQUIRE( atom2 == atom2);
}
