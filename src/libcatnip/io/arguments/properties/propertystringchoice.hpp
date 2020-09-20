
#ifndef _CATNIP_PROPERTY_STRING_CHOICE_HPP
#define _CATNIP_PROPERTY_STRING_CHOICE_HPP

#include "propertyobject.hpp"
#include <set>

namespace catnip {

class PropertyStringChoice
    : public PropertyObject {
 private:

   std::set<bool> getSetBoolEnforced_(Option option, std::map<Option, std::any> options_);
  ;
 public:
   PropertyStringChoice(void);

   virtual PropertyType getPropertyType(void) const noexcept final {
     return PropertyType::STRING_CHOICE;
   }

   virtual bool propValid(const std::any& string_choice) final;
    };

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_STRING_CHOICE_HPP
