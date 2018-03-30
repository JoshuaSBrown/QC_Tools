
#ifndef _ARGUMENTFILEEXIST_HPP
#define _ARGUMENTFILEEXIST_HPP

#include <set>
#include "argumentfileext.hpp"

class ArgumentFile {
  private:
    ArgumentFileExt ext_property_;
    ArgumentFileExist exist_property_;
//    bool fileExist(std::string);
//    std::string getType_(void) { return "FILE_EXIST"; }
  public:
//    ArgumentFileExist(void);
//    ArgumentFileExist(bool fileMustExist);
    bool argValid(std::string fileName);
};

#endif
