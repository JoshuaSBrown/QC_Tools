
#ifndef _CATNIP_PROPERTY_INT_HPP
#define _CATNIP_PROPERTY_INT_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyInt : public PropertyObject<int, int> {
 private:
  void intValid(const int& val) const;
  std::string getName_(void) const { return "PROPERTY_INT"; }
  std::vector<std::string> getOpts_(void) const;

 public:
  PropertyInt(void);
  bool propValid(const int& value) {
    intValid(value);
    return true;
  }
};

}  // namespace catnip

#endif  // _CATNIP_PROPERTY_INT_HPP
