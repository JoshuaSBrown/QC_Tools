
#include "argumentint.hpp"
#include "PROPERTIES/propertyint.hpp"
#include <iostream>

using namespace std;

ArgumentInt::ArgumentInt(void){
  registerProperties_();
}

void ArgumentInt::registerProperties_(void){
  PropertyInt * prop_int = new PropertyInt;
  int_propobjs_.push_back(prop_int);
}


