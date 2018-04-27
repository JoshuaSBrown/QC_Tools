
#ifndef _ARGUMENT_SWITCH_HPP
#define _ARGUMENT_SWITCH_HPP

#include <set>
#include "argumentobject.hpp"

class ArgumentSwitch : public ArgumentObject<std::string> {
  private:
    std::string getName_(void) { return "ARGUMENT_SWITCH"; }
    void registerProperties_(void);
  public:
    ArgumentSwitch(void);
    bool requiresParameter(void) { return false; }
    bool positive(int val) { return ((val==1) ? true : false);}
    bool positive(std::string val);
};

#endif
