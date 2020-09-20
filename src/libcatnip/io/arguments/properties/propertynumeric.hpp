
#ifndef _CATNIP_PROPERTY_NUMERIC_HPP
#define _CATNIP_PROPERTY_NUMERIC_HPP

#include "propertyobject.hpp"

#include <any>
#include <limits>

namespace catnip {

class PropertyNumeric : public PropertyObject {

  // Override the setting make sure that a double is passed in as the numeric type
  virtual void setPropOption_(const Option & option, const std::any & val) final;

 public:
  PropertyNumeric(void) {
    options_[Option::MIN] = std::numeric_limits<double>::min;
    options_[Option::MAX] = std::numeric_limits<double>::max; 
  };

  virtual PropertyType getPropertyType(void) const noexcept final {
    return PropertyType::NUMERIC;
  }

  virtual bool propValid(const std::any& value) final;
};

}  // namespace catnip

#endif  // _CATNIP_PROPERTY_NUMERIC_HPP
