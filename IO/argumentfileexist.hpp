
#ifndef _ARGUMENTFILEEXIST_HPP
#define _ARGUMENTFILEEXIST_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentFileExist : public ArgumentObject<std::string,bool> {
  private:
    bool fileExist(std::string);
    std::string getType_(void) { return "FILE_EXIST"; }
  public:
    ArgumentFileExist(void);
    ArgumentFileExist(std::string fileNamePath);
};

#endif
