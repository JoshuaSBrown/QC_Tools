
#include "../libcatnip/io/arguments/properties/propertydouble.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PropertyDouble" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertyDouble propDouble; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyDouble propDouble;
    string name = propDouble.getPropertyName();
    assert(name.compare("PROPERTY_DOUBLE") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyDouble propDouble;
    auto options = propDouble.getPropertyOptions();
    string opt = options.at(0);
    assert(opt.compare("MIN") == 0);
    opt = options.at(1);
    assert(opt.compare("MAX") == 0);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyDouble propDouble;
    bool valid = propDouble.propValid(0.0);
    assert(valid);
  }

  cerr << "Testing: setPropOption" << endl;
  {
    PropertyDouble propDouble;
    double val = -1.2;
    propDouble.setPropOption("MIN", val);
    propDouble.propValid(0.0);
    bool excep = false;
    try {
      val = -2.3;
      propDouble.propValid(val);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      val = 3.5;
      propDouble.setPropOption("MAXimum", val);
    } catch (...) {
      excep = true;
    }
    assert(excep);
  }
  return 0;
}
