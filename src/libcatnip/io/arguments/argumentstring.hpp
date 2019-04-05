
#ifndef _CATNIP_ARGUMENT_STRING_HPP
#define _CATNIP_ARGUMENT_STRING_HPP

#include <set>
#include "argumentobject.hpp"

namespace catnip {

class ArgumentString : public ArgumentObject<std::string> {
  private:
    std::string getName_(void) const { return "ARGUMENT_STRING"; }
    void registerProperties_(void);
  public:
    ArgumentString(void);
};

}

#endif // _CATNIP_ARGUMENT_STRING_HPP

