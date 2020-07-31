
#ifndef _CATNIP_PROPERTY_OBJECT_HPP
#define _CATNIP_PROPERTY_OBJECT_HPP

#include <any>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace catnip {

enum class PropertyType {
  UNKNOWN
};

enum class Option {
  NO_OPTIONS
};

class PropertyObject {
 protected:
  // Map key is the option name and the type "int, double etc"
  // void * points to the correct value for that type
  std::map<Option, std::any> options_;

  bool propOptionValid_(const Option &option) const {
    return options_.count(option);
  }

  void setPropOption_(const Option &option, const std::any &val) {
      options_[option] = val;
  }

  virtual PropertyType getPropertyType_(void) const noexcept 
  { return PropertyType::UNKNOWN; }

  virtual std::vector<Option> getOpts_(void) const noexcept {
    std::vector<Option> options{Option::NO_OPTIONS};
    return options;
  }

 public:
  virtual ~PropertyObject(void) {
  }

  virtual bool propValid(const std::any &value) = 0;

  PropertyType getPropertyType(void) const noexcept { return getPropertyType_(); }

  std::vector<Option> getPropertyOptions(void) const noexcept { return getOpts_(); }

  // Setup the valid options associated with the parameter
  void setPropOption(Option option, const std::any & val) {
    if (!propOptionValid_(option)) {
      throw std::invalid_argument("Property option is unrecognized.");
    }
    setPropOption_(option, val);
  }

  template<class T>
  T getPropOption(const Option &option) const {
    if (!propOptionValid_(option)) {
      std::string err = " An unrecognized property option was detected.";
      throw std::invalid_argument(err);
    }
    return std::any_cast<T>(options_.at(option));
  }

  virtual void postCheck(void) const { return; }
};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_OBJECT_HPP
