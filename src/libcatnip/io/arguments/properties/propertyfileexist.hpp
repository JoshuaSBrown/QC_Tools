
#ifndef _CATNIP_PROPERTY_FILEEXIST_HPP
#define _CATNIP_PROPERTY_FILEEXIST_HPP

#include "propertyobject.hpp"
#include <set>

namespace catnip {

class PropertyFileExist : public PropertyObject {
 private:
  bool fileExist(const std::string &) const;

 public:
  explicit PropertyFileExist(void);
  PropertyFileExist(bool fileMustExist);

  virtual bool propValid(const std::any &fileName) final;

  virtual PropertyType getPropertyType() const noexcept final {
    return PropertyType::FILE_EXISTS;
  }

  void postCheck(void) const;
};

}  // namespace catnip

#endif  // _CATNIP_PROPERTY_FILEEXIST_HPP
