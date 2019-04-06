
#include "argumentdouble.hpp"
#include "properties/propertydouble.hpp"
#include <iostream>

using namespace catnip;
using namespace std;

ArgumentDouble::ArgumentDouble(void) { registerProperties_(); }

void ArgumentDouble::registerProperties_(void) {
  PropertyDouble* prop_double = new PropertyDouble();
  double_propobjs_.push_back(prop_double);
}
