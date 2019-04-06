
#ifndef _CATNIP_PROPERTY_SWITCH_HPP
#define _CATNIP_PROPERTY_SWITCH_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertySwitch : public PropertyObject<std::string, std::string> {
 private:
  std::string getName_(void) const { return "PROPERTY_SWITCH"; }
  std::vector<std::string> getOpts_(void) const;
  void propOptionValueSettingValid_();
  void switchValid(int val) const;
  void switchValid(std::string val) const;

 public:
  PropertySwitch(void);
  bool propValid(int value) {
    switchValid(value);
    return true;
  }
  bool propValid(const std::string& value) {
    switchValid(value);
    return true;
  }
  bool propValid() { return true; }
};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_SWITCH_HPP
