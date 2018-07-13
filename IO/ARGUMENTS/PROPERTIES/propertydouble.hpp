
#ifndef _CATNIP_PROPERTY_DOUBLE_HPP
#define _CATNIP_PROPERTY_DOUBLE_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyDouble : public PropertyObject<double,double> {
  private:
    void doubleValid(double val);
    std::string getName_(void) { return "PROPERTY_DOUBLE";}
    std::vector<std::string> getOpts_(void);
  public:
    PropertyDouble(void);
    bool propValid(double value) { doubleValid(value); return true; }
};

}

#endif // _CATNIP_PROPERTY_DOUBLE_HPP

