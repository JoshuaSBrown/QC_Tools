
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "io/arguments/properties/propertynumerical.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

TEST_CASE("Property Numerical","[unit]") {

  cerr << "Testing: PropertyNumerical" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertyNumerical propNumerical; }

  cerr << "Testing: getPropertyType" << endl;
  {
    PropertyNumerical propNumerical;
    PropertyType name = propNumerical.getPropertyType();
    assert(name == PropertyType::NUMERICAL);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyNumerical propNumerical;
    auto options = propNumerical.getPropertyOptions();
    string opt = options.at(0);
    assert(opt == Option::MIN);
    opt = options.at(1);
    assert(opt == Option::MAX);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyNumerical propNumerical;
    bool valid = propNumerical.propValid(0);
    assert(valid);
  }

  cerr << "Testing: setPropOption" << endl;
  {
    PropertyNumerical propNumerical;
    propNumerical.setPropOption(Option::MIN, 0);
    propNumerical.propValid(0);
    bool excep = false;
    try {
      propNumerical.propValid(-1);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      propNumerical.setPropOption(Option::ENFORCED, 3);
    } catch (...) {
      excep = true;
    }
    assert(excep);
  }
}
