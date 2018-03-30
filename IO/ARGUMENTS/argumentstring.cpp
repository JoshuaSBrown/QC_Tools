
#include "argumentstring.hpp"
#include "PROPERTIES/propertystring.hpp"
#include <iostream>

using namespace std;

ArgumentString::ArgumentString(void){
  registerProperties_();
}

void ArgumentString::registerProperties_(void){
  PropertyString * prop_string = new PropertyString;
  size_t_propobjs_.push_back(prop_string);
}


