
#include "argumentstring.hpp"
#include "properties/propertystring.hpp"
#include "properties/propertystringchoice.hpp"
#include <iostream>

using namespace catnip;
using namespace std;

ArgumentString::ArgumentString(void) { 
  propobjs_.emplace_back(new PropertyString);
  propobjs_.emplace_back(new PropertyStringChoice);
}
