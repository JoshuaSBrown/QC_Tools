
#include <sys/stat.h>
#include <stdexcept>
#include "propertyfileexist.hpp"
#include "../../../STRING_SUPPORT/string_support.hpp"

using namespace std;


PropertyFileExist::PropertyFileExist(void){
  setPropOption_("FILE_MUST_EXIST",false);
}

PropertyFileExist::PropertyFileExist(bool fileMustExist){
  setPropOption_("FILE_MUST_EXIST",fileMustExist);
}

vector<string> PropertyFileExist::getOpts_(void){
  vector<string> options{"FILE_MUST_EXIST"};
  return options;
}

bool PropertyFileExist::fileExist(string fileNamePath){
  struct stat buf;
  return (stat(fileNamePath.c_str(),&buf)==0);
}

bool PropertyFileExist::propValid(string fileNamePath){
  if(getPropOption("FILE_MUST_EXIST")){
    if(!fileExist(fileNamePath)){
      string err = ""+fileNamePath+" does not exist";
      throw invalid_argument(err);
    }
  }
  return true;
}
