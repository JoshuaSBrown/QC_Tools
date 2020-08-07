
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../libcatnip/io/arguments/properties/propertystring.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

TEST_CASE("Property String","[unit]") {

  cerr << "Testing: PropertyString" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertyString propString; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyString propString;
    PropertyType type = propString.getPropertyType();
    REQUIRE(type == PropertyType::STRING);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyString propString;
    auto options = propString.getPropertyOptions();
    Option opt = options.at(0);
    REQUIRE(opt == Option::MIN_LENGTH);
    opt = options.at(1);
    REQUIRE(opt == Option::MAX_LENGTH);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyString propString;
    bool valid = propString.propValid("Hello");
    REQUIRE(valid);
  }

  cerr << "Testing: setPropOption" << endl;
  {
    PropertyString propString;
    size_t val = 3;
    propString.setPropOption(Option::MAX_LENGTH, val);
    propString.propValid("");
    CHECK_THROWS(propString.propValid("Hello"));
    
    val = 4;
    CHECK_THROWS(propString.setPropOption(Option::MAX, val));
  }
}
