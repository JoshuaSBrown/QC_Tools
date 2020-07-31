
#ifndef _CATNIP_PROPERTY_NUMERIC_HPP
#define _CATNIP_PROPERTY_NUMERIC_HPP

#include "propertyobject.hpp"

#include <any>

namespace catnip {

class PropertyNumeric : public PropertyObject {

  // Override the setting make sure that a double is passed in as the numeric type
  virtual void setPropOption_(const Option & option, const std::any & val) final;

 public:
  PropertyNumeric(void) {
    options_[Option::MIN] = DefaultValue::DEFAULT;
    options_[Option::MAX] = DefaultValue::DEFAULT; 
  };

  virtual std::vector<Option> getPropertyOptions(void) const noexcept final {
    return std::vector<Option> {Option::MIN, Option::MAX};
  }

  virtual PropertyType getPropertyType(void) const noexcept final {
    return PropertyType::NUMERIC;
  }

  virtual bool propValid(const std::any& value) final;
};

}  // namespace catnip

#endif  // _CATNIP_PROPERTY_NUMERIC_HPP
