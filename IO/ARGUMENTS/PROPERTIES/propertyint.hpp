
#ifndef _PROPERTY_INT_HPP
#define _PROPERTY_INT_HPP

#include "propertyobject.hpp"

class PropertyInt : public PropertyObject<int,int> {
  private:
    void intValid(int val);
    std::string getName_(void) { return "PROPERTY_INT";}
    std::vector<std::string> getOpts_(void);
  public:
    PropertyInt(void);
    bool propValid(int value) { intValid(value); return true;}
};

#endif
