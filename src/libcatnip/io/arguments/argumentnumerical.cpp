
#include "argumentnumerical.hpp"
#include "properties/propertynumerical.hpp"
#include <iostream>
#include <memory>

using namespace catnip;
using namespace std;

ArgumentNumerical::ArgumentNumerical(void) { 
  propobjs_.push_back(std::unique_ptr<PropertyObject>( new PropertyNumerical()));
}

