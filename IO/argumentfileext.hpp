
#ifndef _ARGUMENTFILEEXT_HPP
#define _ARGUMENTFILEEXT_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentFileExt : public ArgumentObject<std::string,std::set< std::string > >{
  private:
    void checkExt(std::string);
    void extSupported(std::string);
    std::string getType_(void) { return "FILE_EXT"; }
  public:
    ArgumentFileExt(void){};
    ArgumentFileExt(std::string ext);
    ArgumentFileExt(std::set<std::string> exts);
    bool argValid(std::string fileNamePath);
    void setArgOption(std::string option,std::string var);
    void setArgOption(std::string option,std::set<std::string> var);
};
#endif
