
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
  UNKNOWN,
  NUMERIC,
  FILE_EXISTS,
  FILE_EXT,
  SISTER_FILE,
  STRING,
  STRING_CHOICE,
  SWITCH
};

enum class Option {
  NO_OPTIONS,
  MIN,
  MAX,
  MIN_LENGTH,
  MAX_LENGTH,
  MUST_EXIST,
  DOES_EXIST,
  ALLOWED_VALUES,
  ENFORCED,
  FILE_NAME,
  FILE_PATH,
  FILE_PATH_NAME,
  VALUE
};

class PropertyObject {
 protected:
  // Map key is the option name and the type "int, double etc"
  // void * points to the correct value for that type
  std::map<Option, std::any> options_;

  bool propOptionValid_(const Option &option) const {
    return options_.count(option);
  }

  virtual void setPropOption_(const Option &option, const std::any &val) {
      options_[option] = val;
  }

 public:
  virtual ~PropertyObject(void) { }

  virtual bool propValid(const std::any &value) = 0;

  virtual PropertyType getPropertyType(void) const noexcept = 0;

  std::vector<Option> getPropertyOptions(void) const {
    std::vector<Option> options;
    for ( std::pair<Option,std::any> option : options_){
      options.push_back(option.first); 
    }
    return options;
  } 

  // Setup the valid options associated with the parameter
  // WARNING setting a property value will overwrite existing values
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
