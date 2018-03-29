
#include <sys/stat.h>
#include <stdexcept>
#include "argumentfileext.hpp"
#include "../STRING_SUPPORT/string_support.hpp"

using namespace std;

ArgumentFileExt::ArgumentFileExt(string ext){
  checkExt(ext);
  set<string> exts_;
  exts_.insert(ext);
  setArgOption_("ALLOWED_FILE_TYPE",exts_);
}

ArgumentFileExt::ArgumentFileExt(set<string> exts){
  set<string> exts_;
  for(auto ext : exts){
    checkExt(ext);
    exts_.insert(ext); 
  }
  setArgOption_("ALLOWED_FILE_TYPE",exts_);  
}

void ArgumentFileExt::extSupported(string ext){
  checkExt(ext);
  auto exts_ = getArgOption("ALLOWED_FILE_TYPE"); 
  for(auto ext_ : exts_ ){
    if(ext_.compare("*")==0){
      return;
    }else if(ext_.compare(ext)==0){
      return;
    }
  }   
  string err = "The file ext "+ext+" is unsupported";
  throw invalid_argument(err);
}

void ArgumentFileExt::checkExt(string ext){
  if(ext.compare("")==0){
    return;
  }else if(ext.compare("*")==0){
    return;
  }else{
    string excess = grabStrBeforeLastOccurance(ext,".");
    if(excess.compare("")!=0){
      throw invalid_argument("An extension must be of type '.ext','*','', "
                             "you have used "+ext);
    }
  }
  return;
}

void ArgumentFileExt::setArgOption(std::string option,std::set<std::string> vars){
  ArgumentObject::setArgOption(option,vars);
}

void ArgumentFileExt::setArgOption(std::string option,std::string var){
  set<string> vars;
  vars.insert(var);
  ArgumentObject::setArgOption(option,vars);
}

bool ArgumentFileExt::argValid(string fileNamePath){
  string fileName = lastStringInPath(fileNamePath);
  string ext = grabStrAfterLastOccuranceInclusive(fileName,".");
  extSupported(ext);
  return true;
}
