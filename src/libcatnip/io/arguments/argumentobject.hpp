
#ifndef _CATNIP_ARGUMENTOBJECT_HPP
#define _CATNIP_ARGUMENTOBJECT_HPP

#include "string_support.hpp"
#include "properties/propertyobject.hpp"
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace catnip {

  enum class ArgumentType {
    NUMERICAL,
    FILES,
    STRING,
    SWITCH
  };

// The template is simply for specifying the type of the argument coming
// from the command line
class ArgumentObject {
 protected:
  std::vector<std::unique_ptr<PropertyObject>> propobjs_;

 public:
  virtual ~ArgumentObject(void) {};

  // The argument object by default requires a parameter
  virtual bool requiresParameter(void) { return true; }

  virtual ArgumentType getArgumentType(void) const noexcept = 0; 

  // This is the type the value will be converted to after being read in
  virtual std::type_index getValueType(void) const noexcept = 0;

  std::vector<PropertyType> getProperties(void) {
    std::vector<PropertyType> props;
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      props.push_back(prop->getPropertyType());
    }
    return props;
  }

  std::vector<Option> getPropertyOptions(PropertyType property) {
      for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
        if ( prop->getPropertyType() == property ) {
          return prop->getPropertyOptions();
        }
      }
      return std::vector<Option>();
  }

  /**
   * @brief Gets the options of all properties
   *
   * @return 
   */
  std::vector<Option> getPropertyOptions(void) const {
    std::vector<Option> ops;
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      auto vec_opt = prop->getPropertyOptions();
      ops.insert(ops.end(), vec_opt.begin(), vec_opt.end());
    }
    return ops;
  }

 
  /**
   * @brief Returns the options with value associated with it
   *
   * @return 
   */
  std::map<PropertyType, std::map<Option, std::any>> getPropertyValues(void) {
    std::map<PropertyType, std::map<Option, std::any>> opts_values;
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      PropertyType type = prop->getPropertyType();
      auto vec_opt = prop->getPropertyOptions();
      for (Option opt : vec_opt) {
        auto value = prop->getPropOption<std::any>(opt);
        opts_values[type][opt] = value;
      }
    }
    return opts_values;
  }

  std::map<Option, std::any> getPropertyValues(
      PropertyType property) {

    std::map<Option, std::any> opts_values;
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      if (property == prop->getPropertyType()) {
        std::vector<Option> vec_opt = prop->getPropertyOptions();
        for (const Option & opt : vec_opt) {
          auto value = prop->getPropOption<std::any>(opt);
          opts_values[opt] = value;
        }
      }
    }
    return opts_values;
  }

  template<class T>
  T getPropertyValues(PropertyType property,const Option & option) {
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      if (property == prop->getPropertyType()) {
        std::vector<Option> vec_opt = prop->getPropertyOptions();
        for (const Option & opt : vec_opt) {
          if (option == opt) {
            return prop->getPropOption<T>(opt);
          }
        }
      }
    }

    std::string err =
        "Unable to recognize property or option ";
    throw std::invalid_argument(err);
  }

  // Setup the valid options associated with the parameter
  template<class T>
  void setArgPropertyOpt(PropertyType property, Option option,
                         const T& val) {
    bool setval = false;
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      if (property == prop->getPropertyType()) {
        prop->setPropOption(option, val);
        setval = true;
      }
    }
    if (!setval) {
      std::string err =
          "Argument property: is unrecognized for argument ";
      throw std::invalid_argument(err);
    }
  }

  void setValues(std::map<PropertyType, std::map<Option, std::any>> values) {
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      const PropertyType & type = prop->getPropertyType();
      if(values.count(type)){
        for ( std::pair<Option,std::any> opt_val : values[type] ){
          prop->setPropOption(opt_val.first, opt_val.second);
        }
      }
    }
  }

   void setPropertyValues(
      PropertyType property, std::map<Option, std::any> opts) {

    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      if (property == prop->getPropertyType()) {
        for ( std::pair<Option,std::any> opt_val : opts ){
          prop->setPropOption(opt_val.first, opt_val.second);
        }
        return; 
      }
    }
  }

  template< class T>
  bool argValid(const T& value) const {
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      prop->propValid(value);
    }
    return true;
  }

  void postArgCheck(void) {
    for (const std::unique_ptr<PropertyObject> & prop : propobjs_) {
      prop->postCheck();
    }
  }
};

}  // namespace catnip
#endif  // _CATNIP_ARGUMENTOBJECT_HPP
