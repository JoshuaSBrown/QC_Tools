
#include "../libcatnip/io/arguments/properties/propertyswitch.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PropertySwitch" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertySwitch propSwitch; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertySwitch propSwitch;
    string name = propSwitch.getPropertyName();
    assert(name.compare("PROPERTY_SWITCH") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertySwitch propSwitch;
    auto options = propSwitch.getPropertyOptions();
    string opt = options.at(0);
    assert(opt.compare("DEFAULT") == 0);
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
    propSwitch.setPropOption("DEFAULT", "ON");
    propSwitch.setPropOption("DEFAULT", "OFF");
    //    propSwitch.propValid(0);
    //    bool excep = false;
    //    try {
    //      propSwitch.propValid(-1);
    //    } catch (...) {
    //      excep = true;
    //    }
    //    assert(excep);
  }
  return 0;
}
