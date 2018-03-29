
#ifndef _PROPERTY_STRING_HPP
#define _PROPERTY_STRING_HPP

#include "propertyobject.hpp"

class PropertyString : public PropertyObject<std::string,size_t>{ 
  private:
    void stringValid(std::string val);
    std::string getName_(void) { return "PROPERTY_STRING";}
    std::vector<std::string> getOpts_(void);
  public:
    PropertyString(void);
    bool propValid(std::string value) { stringValid(value); return true;}
};

#endif 

