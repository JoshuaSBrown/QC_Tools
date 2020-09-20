
#include "argumentnumerical.hpp"
#include "properties/propertynumeric.hpp"
#include <iostream>
#include <memory>

using namespace catnip;
using namespace std;

ArgumentNumerical::ArgumentNumerical(void) { 
  propobjs_.emplace_back(new PropertyNumeric());
}

