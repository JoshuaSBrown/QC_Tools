
#ifndef _ARGUMENTINT_HPP
#define _ARGUMENTINT_HPP

#include "argumentobject.hpp"

class ArgumentInt : public ArgumentObject<int,int> {
  private:
    void intValid(int val);
    std::string getType_(void) { return "INT";}
  public:
    ArgumentInt(void);
    bool argValid(int value) { intValid(value); return true;}
};

#endif
