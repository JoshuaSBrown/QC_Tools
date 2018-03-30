
#include "argumentdouble.hpp"
#include "PROPERTIES/propertydouble.hpp"
#include <iostream>

using namespace std;

ArgumentDouble::ArgumentDouble(void){
  registerProperties_();
}

void ArgumentDouble::registerProperties_(void){
  PropertyDouble * prop_double = new PropertyDouble;
  double_propobjs_.push_back(prop_double);
}


