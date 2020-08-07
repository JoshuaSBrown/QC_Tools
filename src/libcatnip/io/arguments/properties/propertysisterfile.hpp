
#ifndef _CATNIP_PROPERTY_SISTER_FILE_HPP
#define _CATNIP_PROPERTY_SISTER_FILE_HPP

#include "propertyobject.hpp"
#include <vector>

namespace catnip {

class PropertySisterFile
    : public PropertyObject {
 private:
  bool fileExist(const std::string &) const;
  void extSupported(const std::string &) const;
  void checkExt(const std::string &) const;

 public:

  PropertySisterFile(void);

  virtual PropertyType getPropertyType(void) const noexcept final {
    return PropertyType::SISTER_FILE;
  }

  virtual bool propValid(const std::any &fileNamePath) final;

};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_SISTER_FILE_HPP
