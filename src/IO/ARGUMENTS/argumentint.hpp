
#ifndef _CATNIP_ARGUMENT_INT_HPP
#define _CATNIP_ARGUMENT_INT_HPP

#include <set>
#include "argumentobject.hpp"

namespace catnip {

class ArgumentInt : public ArgumentObject<int> {
  private:
    std::string getName_(void) const { return "ARGUMENT_INT"; }
    void registerProperties_(void);
  public:
    ArgumentInt(void);
};

}

#endif // _CATNIP_ARGUMENT_INT_HPP

