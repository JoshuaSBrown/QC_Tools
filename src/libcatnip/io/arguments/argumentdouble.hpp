
#ifndef _CATNIP_ARGUMENT_DOUBLE_HPP
#define _CATNIP_ARGUMENT_DOUBLE_HPP

#include <set>
#include "argumentobject.hpp"

namespace catnip {

class ArgumentDouble : public ArgumentObject<double> {
  private:
    std::string getName_(void) const { return "ARGUMENT_DOUBLE"; }
    void registerProperties_(void);
  public:
    ArgumentDouble(void);
};

}
#endif // _CATNIP_ARGUMENT_DOUBLE_HPP
