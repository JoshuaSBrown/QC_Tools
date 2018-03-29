
#include <sys/stat.h>
#include <stdexcept>
#include "argumentfileexist.hpp"
#include "../STRING_SUPPORT/string_support.hpp"

using namespace std;


ArgumentFileExist::ArgumentFileExist(void){
  setArgOption_("FILE_EXIST",false);
}

ArgumentFileExist::ArgumentFileExist(string fileNamePath){
  bool existOrNot = fileExist(fileNamePath);
  setArgOption_("FILE_EXIST",existOrNot);
}

bool ArgumentFileExist::fileExist(string fileNamePath){
  struct stat buf;
  return (stat(fileNamePath.c_str(),&buf)==0);
}

