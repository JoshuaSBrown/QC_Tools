
#include "argumentstring.hpp"
#include "properties/propertystring.hpp"
#include "properties/propertystringchoice.hpp"
#include <iostream>

using namespace catnip;
using namespace std;

ArgumentString::ArgumentString(void) { registerProperties_(); }

void ArgumentString::registerProperties_(void) {
  PropertyString* prop_string = new PropertyString;
  size_t_propobjs_.push_back(prop_string);
  PropertyStringChoice* prop_string_choice = new PropertyStringChoice;
  string_set_propobjs_.push_back(prop_string_choice);
}
