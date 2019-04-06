
#include "../libcatnip/io/arguments/properties/propertyint.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PropertyInt" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertyInt propInt; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyInt propInt;
    string name = propInt.getPropertyName();
    assert(name.compare("PROPERTY_INT") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyInt propInt;
    auto options = propInt.getPropertyOptions();
    string opt = options.at(0);
    assert(opt.compare("MIN") == 0);
    opt = options.at(1);
    assert(opt.compare("MAX") == 0);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyInt propInt;
    bool valid = propInt.propValid(0);
    assert(valid);
  }

  cerr << "Testing: setPropOption" << endl;
  {
    PropertyInt propInt;
    propInt.setPropOption("MIN", 0);
    propInt.propValid(0);
    bool excep = false;
    try {
      propInt.propValid(-1);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      propInt.setPropOption("MAXimum", 3);
    } catch (...) {
      excep = true;
    }
    assert(excep);
  }
  return 0;
}
