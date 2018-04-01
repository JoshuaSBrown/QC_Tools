
#include "argumentfile.hpp"
#include "PROPERTIES/propertyfileexist.hpp"
#include "PROPERTIES/propertyfileext.hpp"
#include "PROPERTIES/propertysisterfile.hpp"
#include <iostream>

using namespace std;

ArgumentFile::ArgumentFile(void){
  registerProperties_();
}

void ArgumentFile::registerProperties_(void){

  PropertyFileExist * prop_file_exist = new PropertyFileExist();
  int_propobjs_.push_back(prop_file_exist);  
  PropertyFileExt * prop_file_ext = new PropertyFileExt();
  string_set_propobjs_.push_back(prop_file_ext);
  PropertySisterFile * prop_sis_file = new PropertySisterFile();
  string_vec_propobjs_.push_back(prop_sis_file); 
}


