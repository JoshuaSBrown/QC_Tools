
#ifndef _CATNIP_PROPERTY_STRING_HPP
#define _CATNIP_PROPERTY_STRING_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyString : public PropertyObject<std::string,size_t>{ 
  private:
    void stringValid(std::string val);
    std::string getName_(void) { return "PROPERTY_STRING";}
    std::vector<std::string> getOpts_(void);
  public:
    PropertyString(void);
    bool propValid(std::string value) { stringValid(value); return true;}
};

}
#endif // _CATNIP_PROPERTY_STRING_HPP


