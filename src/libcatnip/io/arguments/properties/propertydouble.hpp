
#ifndef _CATNIP_PROPERTY_DOUBLE_HPP
#define _CATNIP_PROPERTY_DOUBLE_HPP

#include "propertyobject.hpp"

namespace catnip {

class PropertyDouble : public PropertyObject<double, double> {
 private:
  void doubleValid(const double& val);
  std::string getName_(void) const { return "PROPERTY_DOUBLE"; }
  std::vector<std::string> getOpts_(void) const;

 public:
  PropertyDouble(void);
  bool propValid(const double& value) {
    doubleValid(value);
    return true;
  }
};

}  // namespace catnip

#endif  // _CATNIP_PROPERTY_DOUBLE_HPP
