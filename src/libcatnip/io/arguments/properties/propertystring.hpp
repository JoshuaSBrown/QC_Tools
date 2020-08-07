
#ifndef _CATNIP_PROPERTY_STRING_HPP
#define _CATNIP_PROPERTY_STRING_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyString : public PropertyObject {
 private:
  void stringValid(const std::string& val) const;

 public:
  PropertyString(void);

  virtual PropertyType getPropertyType(void) const noexcept final {
    return PropertyType::STRING;
  }

  bool propValid(const std::any& val) {

    std::string value = std::any_cast<std::string>(val);
    stringValid(value);
    return true;
  }
};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_STRING_HPP
