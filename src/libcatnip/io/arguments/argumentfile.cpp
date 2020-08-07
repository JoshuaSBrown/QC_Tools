
#include "argumentfile.hpp"
#include "properties/propertyfileexist.hpp"
#include "properties/propertyfileext.hpp"
#include "properties/propertysisterfile.hpp"
#include <iostream>

using namespace catnip;
using namespace std;

ArgumentFile::ArgumentFile(void) { 
  auto prop_file_exist = std::unique_ptr<PropertyObject>(new PropertyFileExist());
  propobjs_.push_back(prop_file_exist);
  auto prop_file_ext = std::unique_ptr<PropertyObject>(new PropertyFileExt());
  propobjs_.push_back(prop_file_ext);
  auto prop_sis_file = std::unique_ptr<PropertyObject>(new PropertySisterFile());
  propobjs_.push_back(prop_sis_file);

}
