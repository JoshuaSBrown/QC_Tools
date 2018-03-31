
#include "argumentfile.hpp"
#include "PROPERTIES/propertyfileexist.hpp"
#include "PROPERTIES/propertyfileext.hpp"
#include <iostream>

using namespace std;

ArgumentFile::ArgumentFile(void){
  registerProperties_();
}

void ArgumentFile::registerProperties_(void){
  PropertyFileExt * prop_file_ext = new PropertyFileExt;
  string_set_propobjs_.push_back(prop_file_ext);

  PropertyFileExist * prop_file_exist = new PropertyFileExist;
  bool_propobjs_.push_back(prop_file_exist);  
}


