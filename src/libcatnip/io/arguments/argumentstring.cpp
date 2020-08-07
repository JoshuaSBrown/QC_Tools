
#include "argumentstring.hpp"
#include "properties/propertystring.hpp"
#include "properties/propertystringchoice.hpp"
#include <iostream>

using namespace catnip;
using namespace std;

ArgumentString::ArgumentString(void) { 
  auto prop_string = std::unique_ptr<PropertyObject>(new PropertyString);
  propobjs_.push_back(prop_string);
  auto prop_string_choice = std::unique_ptr<PropertyObject>(new PropertyStringChoice);
  propobjs_.push_back(prop_string_choice);
}
