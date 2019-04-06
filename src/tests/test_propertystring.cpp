
#include "../libcatnip/io/arguments/properties/propertystring.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PropertyString" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertyString propString; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyString propString;
    string name = propString.getPropertyName();
    assert(name.compare("PROPERTY_STRING") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyString propString;
    auto options = propString.getPropertyOptions();
    string opt = options.at(0);
    assert(opt.compare("MIN_LENGTH") == 0);
    opt = options.at(1);
    assert(opt.compare("MAX_LENGTH") == 0);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyString propString;
    bool valid = propString.propValid("Hello");
    assert(valid);
  }

  cerr << "Testing: setPropOption" << endl;
  {
    PropertyString propString;
    size_t val = 3;
    propString.setPropOption("MAX_LENGTH", val);
    propString.propValid("");
    bool excep = false;
    try {
      propString.propValid("Hello");
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      val = 4;
      propString.setPropOption("MAXimum", val);
    } catch (...) {
      excep = true;
    }
    assert(excep);
  }
  return 0;
}
