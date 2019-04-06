
#ifndef _CATNIP_ARGUMENTOBJECT_HPP
#define _CATNIP_ARGUMENTOBJECT_HPP

#include "../../string_support.hpp"
#include "properties/propertyobject.hpp"
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace catnip {
// The template is simply for specifying the type of the argument coming
// from the command line
template <typename S>
class ArgumentObject {
 protected:
  std::vector<PropertyObject<S, int>*> int_propobjs_;
  std::vector<PropertyObject<S, double>*> double_propobjs_;
  std::vector<PropertyObject<S, size_t>*> size_t_propobjs_;
  std::vector<PropertyObject<S, std::string>*> string_propobjs_;
  std::vector<PropertyObject<S, std::set<std::string>>*> string_set_propobjs_;
  std::vector<PropertyObject<S, std::vector<std::string>>*>
      string_vec_propobjs_;

  virtual std::string getName_(void) const { return "UNKNOWN"; }

  void checkValid_(const S& val) const {
    for (auto prop : int_propobjs_) {
      prop->propValid(val);
    }
    for (auto prop : double_propobjs_) {
      prop->propValid(val);
    }
    for (auto prop : size_t_propobjs_) {
      prop->propValid(val);
    }
    for (auto prop : string_propobjs_) {
      prop->propValid(val);
    }
    for (auto prop : string_set_propobjs_) {
      prop->propValid(val);
    }
    for (auto prop : string_vec_propobjs_) {
      prop->propValid(val);
    }
  }

  // Register the relevant properties
  virtual void registerProperties_(void){};

 public:
  virtual ~ArgumentObject(void) {

    for (auto int_ptr : int_propobjs_) {
      delete int_ptr;
    }
    int_propobjs_.clear();
    for (auto double_ptr : double_propobjs_) {
      delete double_ptr;
    }
    double_propobjs_.clear();
    for (auto size_t_ptr : size_t_propobjs_) {
      delete size_t_ptr;
    }
    size_t_propobjs_.clear();
    for (auto str_ptr : string_propobjs_) {
      delete str_ptr;
    }
    string_propobjs_.clear();
    for (auto set_ptr : string_set_propobjs_) {
      delete set_ptr;
    }
    string_set_propobjs_.clear();
    for (auto vec_ptr : string_vec_propobjs_) {
      delete vec_ptr;
    }
    string_vec_propobjs_.clear();
  }

  // The argument object by default requires a parameter
  virtual bool requiresParameter(void) { return true; }

  std::string getArgumentName(void) { return getName_(); }

  std::vector<std::string> getProperties(void) {
    std::vector<std::string> props;
    for (auto prop : int_propobjs_) {
      props.push_back(prop->getPropertyName());
    }
    for (auto prop : double_propobjs_) {
      props.push_back(prop->getPropertyName());
    }
    for (auto prop : size_t_propobjs_) {
      props.push_back(prop->getPropertyName());
    }
    for (auto prop : string_propobjs_) {
      props.push_back(prop->getPropertyName());
    }
    for (auto prop : string_set_propobjs_) {
      props.push_back(prop->getPropertyName());
    }
    for (auto prop : string_vec_propobjs_) {
      props.push_back(prop->getPropertyName());
    }

    if (props.size() == 0) props.push_back("NO_PROPERTIES");
    return props;
  }

  std::vector<std::string> getPropertyOptions(void) {
    std::vector<std::string> ops;
    for (auto prop : int_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    for (auto prop : double_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    for (auto prop : size_t_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    for (auto prop : string_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    for (auto prop : string_set_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    for (auto prop : string_vec_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    if (ops.size() == 0) ops.push_back("NO_OPTIONS");
    return ops;
  }

  std::map<std::string, std::string> getPropertyValues(void) {
    std::map<std::string, std::string> opts_values;
    for (auto prop : int_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      for (auto opt : vec_opt) {
        auto value = std::to_string(prop->getPropOption(opt));
        opts_values[opt] = value;
      }
    }
    for (auto prop : double_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      for (auto opt : vec_opt) {
        auto value = std::to_string(prop->getPropOption(opt));
        opts_values[opt] = value;
      }
    }
    for (auto prop : size_t_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      for (auto opt : vec_opt) {
        auto value = std::to_string(prop->getPropOption(opt));
        opts_values[opt] = value;
      }
    }
    for (auto prop : string_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      for (auto opt : vec_opt) {
        auto value = prop->getPropOption(opt);
        opts_values[opt] = value;
      }
    }
    for (auto prop : string_set_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      for (auto opt : vec_opt) {
        std::set<std::string> value_set = prop->getPropOption(opt);
        std::string values = "";
        for (auto val : value_set) {
          values.append(val);
          values.append(" ");
        }
        trim(values);
        opts_values[opt] = values;
      }
    }
    for (auto prop : string_vec_propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      for (auto opt : vec_opt) {
        std::vector<std::string> value_vec = prop->getPropOption(opt);
        std::string values = "";
        for (auto val : value_vec) {
          values.append(val);
          values.append(" ");
        }
        trim(values);
        opts_values[opt] = values;
      }
    }
    return opts_values;
  }

  std::string getPropertyValues(std::string property,const std::string & option) {
    for (auto prop : int_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        auto vec_opt = prop->getPropertyOptions();
        for (auto opt : vec_opt) {
          if (option.compare(opt) == 0) {
            return std::to_string(prop->getPropOption(opt));
          }
        }
      }
    }
    for (auto prop : double_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        auto vec_opt = prop->getPropertyOptions();
        for (auto opt : vec_opt) {
          if (option.compare(opt) == 0) {
            return std::to_string(prop->getPropOption(opt));
          }
        }
      }
    }
    for (auto prop : size_t_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        auto vec_opt = prop->getPropertyOptions();
        for (auto opt : vec_opt) {
          if (option.compare(opt) == 0) {
            return std::to_string(prop->getPropOption(opt));
          }
        }
      }
    }
    for (auto prop : string_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        auto vec_opt = prop->getPropertyOptions();
        for (auto opt : vec_opt) {
          if (option.compare(opt) == 0) {
            return prop->getPropOption(opt);
          }
        }
      }
    }
    for (auto prop : string_set_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        auto vec_opt = prop->getPropertyOptions();
        for (auto opt : vec_opt) {
          if (option.compare(opt) == 0) {
            std::set<std::string> value_set = prop->getPropOption(opt);
            std::string values = "";
            for (auto val : value_set) {
              values.append(val);
              values.append(" ");
            }
            trim(values);
            return values;
          }
        }
      }
    }
    for (auto prop : string_vec_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        auto vec_opt = prop->getPropertyOptions();
        for (auto opt : vec_opt) {
          if (option.compare(opt) == 0) {
            std::vector<std::string> value_vec = prop->getPropOption(opt);
            std::string values = "";
            for (auto val : value_vec) {
              values.append(val);
              values.append(" ");
            }
            trim(values);
            return values;
          }
        }
      }
    }

    std::string err =
        "Unable to recognize property or option " + property + " " + option;
    throw std::invalid_argument(err);
  }

  // Setup the valid options associated with the parameter
  void setArgPropertyOpt(std::string property, std::string option,
                         const double& val) {
    bool setval = false;
    for (auto prop : double_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        prop->setPropOption(option, val);
        setval = true;
      }
    }
    if (!setval) {
      std::string err =
          "Argument property: " + property + " with option: " +
          ""
          "" +
          option + " of type double is unrecognized for argument " + getName_();
      throw std::invalid_argument(err);
    }
  }

  void setArgPropertyOpt(std::string property, std::string option,
                         const std::set<std::string> & val) {
    bool setval = false;
    for (auto prop = string_set_propobjs_.begin();
         prop != string_set_propobjs_.end(); ++prop) {
      if (property.compare((*prop)->getPropertyName()) == 0) {
        (*prop)->setPropOption(option, val);
        setval = true;
      }
    }
    if (!setval) {
      std::string err = "Argument property: " + property + " with option: " +
                        ""
                        "" +
                        option +
                        " of type set<string> is unrecognized for argument " +
                        ""
                        "" +
                        getName_();
      throw std::invalid_argument(err);
    }
  }

  void setArgPropertyOpt(std::string property, std::string option,
                         const std::vector<std::string> & val) {
    bool setval = false;
    for (auto prop : string_vec_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        prop->setPropOption(option, val);
        setval = true;
      }
    }
    if (!setval) {
      std::string err =
          "Argument property: " + property + " with option: " +
          ""
          "" +
          option + " of type vector<string> is unrecognized for argument " +
          ""
          "" +
          getName_();
      throw std::invalid_argument(err);
    }
  }

  void setArgPropertyOpt(std::string property,const std::string & option,
                         const std::string & val) {
    bool setval = false;
    for (auto prop : string_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        prop->setPropOption(option, val);
        setval = true;
      }
    }
    for (auto prop : string_vec_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        std::vector<std::string> vec_str{val};
        prop->setPropOption(option, vec_str);
        setval = true;
      }
    }
    for (auto prop : string_set_propobjs_) {
      if (property.compare(prop->getPropertyName()) == 0) {
        std::set<std::string> set_str{val};
        prop->setPropOption(option, set_str);
        setval = true;
      }
    }
    if (!setval) {
      std::string err =
          "Argument property: " + property + " with option: " +
          ""
          "" +
          option + " of type string is unrecognized for argument " + getName_();
      throw std::invalid_argument(err);
    }
  }

  void setArgPropertyOpt(std::string property, const std::string & option, int val) {
    bool setval = false;
    for (auto prop : int_propobjs_) {

      if (property.compare(prop->getPropertyName()) == 0) {
        prop->setPropOption(option, val);
        setval = true;
      }
    }
    if (!setval) {
      std::string err = "Argument property: " + property + " with option: " +
                        ""
                        "" +
                        option + " of type int is unrecognized for argument " +
                        getName_();
      throw std::invalid_argument(err);
    }
  }

  bool argValid(const S& value) const {
    checkValid_(value);
    return true;
  }

  void postArgCheck(void) {
    for (auto prop : int_propobjs_) {
      prop->postCheck();
    }
    for (auto prop : double_propobjs_) {
      prop->postCheck();
    }
    for (auto prop : size_t_propobjs_) {
      prop->postCheck();
    }
    for (auto prop : string_propobjs_) {
      prop->postCheck();
    }
    for (auto prop : string_set_propobjs_) {
      prop->postCheck();
    }
    for (auto prop : string_vec_propobjs_) {
      prop->postCheck();
    }
  }
};

}  // namespace catnip
#endif  // _CATNIP_ARGUMENTOBJECT_HPP
