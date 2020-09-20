
#include "argumentfile.hpp"
#include "properties/propertyfileexist.hpp"
#include "properties/propertyfileext.hpp"
#include "properties/propertysisterfile.hpp"
#include <iostream>

using namespace catnip;
using namespace std;

ArgumentFile::ArgumentFile(void) { 
  propobjs_.emplace_back(new PropertyFileExist());
  propobjs_.emplace_back(new PropertyFileExt());
  propobjs_.emplace_back(new PropertySisterFile());

}
