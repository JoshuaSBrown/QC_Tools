
#include "propertystringchoice.hpp"
#include "../../../string_support.hpp"
#include <set>
#include <stdexcept>
#include <sys/stat.h>
#include <typeinfo>

using namespace std;

namespace catnip {
  namespace {
    std::set<bool> getSetBoolEnforced_(const Option option, std::map<const Option, std::any> options_){
      set<bool> values;
      // Because the default is a set of strings
      set<string> stored_values = std::any_cast<set<string>>(options_.at(option));
      for ( std::string val : stored_values){
        if( to_upper(val) == "TRUE" ) {
          values.insert(true);
        } else if( to_upper(val) == "FALSE") {
          values.insert(false);
        }
      } 
      return values;
    }

    std::set<string> getSetStrEnforced_(const Option option, std::map<const Option, std::any> options_){
      // Because the default is a set of strings
      set<string> stored_values = std::any_cast<set<string>>(options_.at(option));
      return stored_values;
    }

    std::set<int> getSetIntEnforced_(const Option option, std::map<const Option, std::any> options_){
      set<int> values;
      // Because the default is a set of strings
      set<string> stored_values = std::any_cast<set<string>>(options_.at(option));
      for ( std::string val : stored_values){
        if( to_upper(val) == "TRUE" ) {
          values.insert(1);
        } else if( to_upper(val) == "FALSE") {
          values.insert(0);
        }
      } 
      return values;
    }

    std::set<string> getSetStrAllowedValues_(const Option option, std::map<const Option, std::any> options_){
      // Because the default is a set of strings
      set<string> stored_values = std::any_cast<set<string>>(options_.at(option));
      return stored_values;
    }

    std::vector<string> getVectorStrAllowedValues_(const Option option, std::map<const Option, std::any> options_){
      vector<string> values;
      // Because the default is a set of strings
      set<string> stored_values = std::any_cast<set<string>>(options_.at(option));
      for ( string val : stored_values ) {
        values.push_back(val);
      }
      return values;
    }
  }

  PropertyStringChoice::PropertyStringChoice(void) {
    set<string> set_var1{"false"};
    setPropOption_(Option::ENFORCED, set_var1);

    //  setDefaultOptionType_(Option::ENFORCED, type_index(typeid(set<string>)));
    default_option_type_[Option::ENFORCED] = type_index(typeid(set<string>));

    allowed_option_types_[Option::ENFORCED].push_back(type_index(typeid(set<string>)));
    allowed_option_types_[Option::ENFORCED].push_back(type_index(typeid(set<bool>)));
    allowed_option_types_[Option::ENFORCED].push_back(type_index(typeid(set<int>)));

    set_bool_convert_[Option::ENFORCED] = &catnip::getSetBoolEnforced_;
    set_str_convert_[Option::ENFORCED] = &catnip::getSetStrEnforced_;
    set_int_convert_[Option::ENFORCED] = &catnip::getSetIntEnforced_;

    string str = "NOT_DEFINED";
    set<string> set_var2{str};
    setPropOption_(Option::ALLOWED_VALUES, set_var2);

    default_option_type_[Option::ALLOWED_VALUES] = type_index(typeid(set<string>));

    allowed_option_types_[Option::ALLOWED_VALUES].push_back(type_index(typeid(set<string>)));
    allowed_option_types_[Option::ALLOWED_VALUES].push_back(type_index(typeid(vector<string>)));

    set_str_convert_[Option::ALLOWED_VALUES] = &catnip::getSetStrAllowedValues_;
    vector_str_convert_[Option::ALLOWED_VALUES] = &catnip::getVectorStrAllowedValues_;

  }

  /*void PropertyStringChoice::setPropOption( option,
    set<string> set_vars) {

    if (option.compare("STRING_CHOICES") == 0) {
    setPropOption_(option, set_vars);
    return;
    } else if (option.compare("STRING_CHOICE_ENFORCED") == 0) {
    if (set_vars.size() > 1) {
    string err = "";
    for (auto var : set_vars) {
    err.append(var);
    err.append(" ");
    }
    trim(err);
    throw invalid_argument(
    "Option STRING_CHOICE_ENFORCED only accepts one "
    "value either true or false you have passed the following values " +
    err);
    }

    string var = *(set_vars.begin());
    if (var.compare("false") == 0 || var.compare("true") == 0) {
    setPropOption_(option, set_vars);
    return;
    } else {
    throw invalid_argument("The option " + option +
    " can only be set to values"
    " of true or false you have set it to " +
    var);
    }
    }
    string vars = "";
    for (auto var : set_vars) {
    vars.append(var);
    vars.append(" ");
    }
    trim(vars);
    throw invalid_argument("Unrecognized option value combo " + option + " " +
    vars);
    }*/

  bool PropertyStringChoice::propValid(const any& choice) {

    std::string string_choice = any_cast<std::string>(choice);
    set<string> opt_val = getPropOption<set<string>>(Option::ENFORCED);
    string val = *(opt_val.begin());
    if (val.compare("true") == 0) {
      set<string> str_choices = getPropOption<set<string>>(Option::ALLOWED_VALUES);
      if (str_choices.count(string_choice) > 0) return true;
      throw invalid_argument("The value " + string_choice +
          " is not a valid option");
    }
    return true;
  }
}
