
#ifndef _CATNIP_PROPERTY_OBJECT_HPP
#define _CATNIP_PROPERTY_OBJECT_HPP

#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace catnip {

template <typename S, typename T>
class PropertyObject {
 protected:
  // Map key is the option name and the type "int, double etc"
  // void * points to the correct value for that type
  std::map<std::string, void *> options_;

  bool propOptionValid_(const std::string &option) const {
    for (const auto &opt : options_) {
      if (opt.first.compare(option) == 0) return true;
    }
    return false;
  }

  void setPropOption_(const std::string &option, const T &val) {

    if (options_.count(option) == 0) {
      T *opt = new T(val);
      options_[option] = static_cast<void *>(opt);
    } else {
      T *opt = static_cast<T *>(options_[option]);
      *opt = val;
    }
  }

  virtual std::string getName_(void) const { return "UNKNOWN"; }

  virtual std::vector<std::string> getOpts_(void) const {
    std::vector<std::string> options{"NO_OPTIONS"};
    return options;
  }

 public:
  virtual ~PropertyObject(void) {
    for (auto itr : options_) {
      T *opt = static_cast<T *>(itr.second);
      delete opt;
    }
    options_.clear();
  }

  virtual bool propValid(const S &value) = 0;

  std::string getPropertyName(void) const { return getName_(); }

  std::vector<std::string> getPropertyOptions(void) const { return getOpts_(); }

  // Setup the valid options associated with the parameter
  void setPropOption(std::string option, const T & val) {
    if (!propOptionValid_(option)) {
      throw std::invalid_argument("Property option is unrecognized " + option);
    }
    setPropOption_(option, val);
  }

  T getPropOption(const std::string &option) const {
    if (!propOptionValid_(option)) {
      std::string err = "" + option +
                        " is an unrecognized property option for "
                        "property " +
                        getName_();
      throw std::invalid_argument(err);
    }
    return *(static_cast<T *>(options_.at(option)));
  }

  virtual void postCheck(void) const { return; }
};

}  // namespace catnip
#endif  // _CATNIP_PROPERTY_OBJECT_HPP
