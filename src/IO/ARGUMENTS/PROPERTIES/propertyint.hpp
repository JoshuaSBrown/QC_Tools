
#ifndef _CATNIP_PROPERTY_INT_HPP
#define _CATNIP_PROPERTY_INT_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyInt : public PropertyObject<int,int> {
  private:
    void intValid(int val);
    std::string getName_(void) { return "PROPERTY_INT";}
    std::vector<std::string> getOpts_(void);
  public:
    PropertyInt(void);
    bool propValid(int value) { intValid(value); return true;}
};

}

#endif // _CATNIP_PROPERTY_INT_HPP
