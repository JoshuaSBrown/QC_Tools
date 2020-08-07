
#ifndef _CATNIP_PROPERTY_FILEEXT_HPP
#define _CATNIP_PROPERTY_FILEEXT_HPP

#include "propertyobject.hpp"
#include <set>

namespace catnip {

class PropertyFileExt
    : public PropertyObject {
 private:
  void checkExt(const std::string &) const;
  void extSupported(const std::string &) const;
  std::vector<std::string> getOpts_(void) const;

 public:
  explicit PropertyFileExt(void) {
    std::set<std::string> exts_;
    exts_.insert("*");
    options_[Option::ALLOWED_VALUES] = exts_;
  }

  explicit PropertyFileExt(std::string ext);
  PropertyFileExt(std::set<std::string> exts);

  virtual PropertyType getPropertyType(void) const noexcept final {
    return PropertyType::FILE_EXT;
  }

  bool propValid(const std::any &fileNamePath);
  void setPropOption(Option option, std::string var);
  void setPropOption(Option option, std::set<std::string> var);
};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_FILEEXT_HPP
