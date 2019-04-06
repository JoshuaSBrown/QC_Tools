
#ifndef _CATNIP_PROPERTY_STRING_HPP
#define _CATNIP_PROPERTY_STRING_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyString : public PropertyObject<std::string, size_t> {
 private:
  void stringValid(const std::string& val) const;
  std::string getName_(void) const { return "PROPERTY_STRING"; }
  std::vector<std::string> getOpts_(void) const;

 public:
  PropertyString(void);
  bool propValid(const std::string& value) {
    stringValid(value);
    return true;
  }
};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_STRING_HPP
