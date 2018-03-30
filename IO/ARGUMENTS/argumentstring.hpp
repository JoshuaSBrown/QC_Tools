
#ifndef _ARGUMENT_STRING_HPP
#define _ARGUMENT_STRING_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentString : public ArgumentObject<std::string> {
  private:
    std::string getName_(void) { return "ARGUMENT_STRING"; }
    void registerProperties_(void);
  public:
    ArgumentString(void);
};

#endif
