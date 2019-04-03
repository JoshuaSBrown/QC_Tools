
#ifndef _CATNIP_ARGUMENT_SWITCH_HPP
#define _CATNIP_ARGUMENT_SWITCH_HPP

#include <set>
#include "argumentobject.hpp"

namespace catnip {
class ArgumentSwitch : public ArgumentObject<std::string> {
  private:
    std::string getName_(void) const { return "ARGUMENT_SWITCH"; }
    void registerProperties_(void);
  public:
    ArgumentSwitch(void);
    bool requiresParameter(void) { return false; }
    bool positive(int val) const { return ((val==1) ? true : false);}
    bool positive(std::string val) const;
};

}
#endif // _CATNIP_ARGUMENT_SWITCH_HPP
