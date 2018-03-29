
#ifndef _ARGUMENTDOUBLE_HPP
#define _ARGUMENTDOUBLE_HPP

#include "argumentobject.hpp"

class ArgumentDouble : public ArgumentObject<double,double> {
  private:
    void doubleValid(double val);
    std::string getType_(void) { return "DOUBLE";}
  public:
    ArgumentDouble(void);
    bool argValid(double value) { doubleValid(value); return true; }
};

#endif
