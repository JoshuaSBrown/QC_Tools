
#ifndef _ARGUMENT_DOUBLE_HPP
#define _ARGUMENT_DOUBLE_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentDouble : public ArgumentObject<double> {
  private:
    std::string getName_(void) { return "ARGUMENT_DOUBLE"; }
    void registerProperties_(void);
  public:
    ArgumentDouble(void);
};

#endif
