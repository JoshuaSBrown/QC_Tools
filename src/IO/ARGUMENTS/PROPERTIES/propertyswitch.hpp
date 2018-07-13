
#ifndef _CATNIP_PROPERTY_SWITCH_HPP
#define _CATNIP_PROPERTY_SWITCH_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertySwitch : public PropertyObject<std::string,std::string> {
  private:
    std::string getName_(void) { return "PROPERTY_SWITCH";}
    std::vector<std::string> getOpts_(void);
    void propOptionValueSettingValid_();
    void switchValid(int val);    
    void switchValid(std::string val);
  public:
    PropertySwitch(void);
    bool propValid(int value) { switchValid(value); return true;}
    bool propValid(std::string value) { switchValid(value); return true;}
    bool propValid() { return true;}
};

}
#endif // _CATNIP_PROPERTY_SWITCH_HPP
