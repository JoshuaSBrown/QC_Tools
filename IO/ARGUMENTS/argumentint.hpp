
#ifndef _ARGUMENT_INT_HPP
#define _ARGUMENT_INT_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentInt : public ArgumentObject<int> {
  private:
    std::string getName_(void) { return "ARGUMENT_INT"; }
    void registerProperties_(void);
  public:
    ArgumentInt(void);
};

#endif
