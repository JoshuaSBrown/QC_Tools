
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../libcatnip/io/arguments/properties/propertyswitch.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

TEST_CASE("Property Switch","[unit]") {

  cerr << "Testing: PropertySwitch" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertySwitch propSwitch; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertySwitch propSwitch;
    PropertyType type = propSwitch.getPropertyType();
    REQUIRE(type == PropertyType::SWITCH);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertySwitch propSwitch;
    auto options = propSwitch.getPropertyOptions();
    Option opt = options.at(0);
    REQUIRE(opt == Option::DEFAULT);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertySwitch propSwitch;
    bool valid = propSwitch.propValid(0);
    assert(valid);
    valid = propSwitch.propValid(1);
    assert(valid);
    valid = propSwitch.propValid();
    assert(valid);
    bool excep = false;
    try {
      propSwitch.propValid(-1);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      propSwitch.propValid(2);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    valid = propSwitch.propValid("ON");
    assert(valid);
    valid = propSwitch.propValid("OFF");
    assert(valid);
    valid = propSwitch.propValid("TRUE");
    assert(valid);
    valid = propSwitch.propValid("FALSE");
    assert(valid);
    valid = propSwitch.propValid("0");
    assert(valid);
    valid = propSwitch.propValid("1");
    assert(valid);
  }

  cerr << "Testing: setPropOption" << endl;
  {
    PropertySwitch propSwitch;
    propSwitch.setPropOption(Option::DEFAULT, "ON");
    propSwitch.setPropOption(Option::DEFAULT, "OFF");
    //    propSwitch.propValid(0);
    //    bool excep = false;
    //    try {
    //      propSwitch.propValid(-1);
    //    } catch (...) {
    //      excep = true;
    //    }
    //    assert(excep);
  }
}
