
#ifndef _ARGUMENT_FILE_HPP
#define _ARGUMENT_FILE_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentFile : public ArgumentObject<std::string> {
  private:
    std::string getName_(void) { return "ARGUMENT_FILE"; }
    void registerProperties_(void);
  public:
    ArgumentFile(void);
};

#endif
