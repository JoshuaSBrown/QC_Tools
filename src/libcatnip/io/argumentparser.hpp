#ifndef _CATNIP_ARGUMENTPARSER_HPP
#define _CATNIP_ARGUMENTPARSER_HPP
#include "matrix.hpp"
#include "parameters.hpp"
#include <any>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "arguments/argumentnumerical.hpp"
#include "arguments/argumentfile.hpp"
#include "arguments/argumentstring.hpp"
#include "arguments/argumentswitch.hpp"

namespace catnip {

  std::unique_ptr<ArgumentObject> createArgument(ArgumentType type);

class ArgumentParser {
 private:
  // 1 - flag name
  // 2 - type
  std::map<std::string, std::vector<std::unique_ptr<ArgumentObject>>> arg_;

  // Known rules
  std::set<std::string> argument_types_;

  // Known flags and their description
  std::map<std::string, std::pair<std::string, std::string>> flags_;

  std::map<std::string, bool> defaults_set_;
  // Arguments that are stored when argc and argv are parsed
  std::map<std::string, std::map<std::type_index, std::vector<std::any>>> values_;

  void parseArg_(size_t& index, std::vector<std::string> arguments);

  // Determine if the next value in arguments vector is a recognized flag
  bool nextParameterIsAFlag_(size_t index, std::vector<std::string> arguments);

  size_t maxShortFlagSize;
  size_t maxFullFlagSize;
  size_t maxDescriptionSize;
  size_t maxLineLength = 80;

 public:
  // 1 - short flag
  // 2 - long flag
  // 3 - description
  explicit ArgumentParser(std::set<std::vector<std::string>> flags);

  // Set Defaults for the flags in the case that they are not found
  template<class T>
  void setFlagDefaultValue(std::string flag,const T & val) {
    values_[flag][std::type_index(val)].push_back(val);
    defaults_set_[flag] = true;
  }

  // Add a argument without setting any of the values
  void addFlagArg(std::string flag, ArgumentType argname);
  // Set the rules for the flag and the type it is associated with types
  // include:
  // "FILES"
  // "NUMERICAL"
  // "STRING"
  // "SWITCH"
  // The rules are dependent on the type
  template<class T>
    void ArgumentParser::setFlagArgOpt(
        string flag, 
        ArgumentType argname,
        PropertyType property,
        Option option,
        T val) {

      
      if (arg_.count(flag) == 0) {
        auto arg = createArgument(argname);
        ArInt->setArgPropertyOpt(property, option, val);
        arg_[flag].push_back(std::move(ArInt));
      } else {
        for ( std::unique_ptr<ArgumentObject> & ptr : arg_[flag]){
          ptr->setArgPropertyOpt(property, option, val);
        }
      }
/*
      if (argname == ArgumentType::NUMERICAL) {
        if (arg_.count(flag) == 0) {
          auto ArInt = std::unique_ptr<ArgumentObject>(new ArgumentNumerical);
          ArInt->setArgPropertyOpt(property, option, val);
          arg_[flag].push_back(std::move(ArInt));
        } else {
          for ( std::unique_ptr<ArgumentObject> & ptr : arg_[flag]){
            ptr->setArgPropertyOpt(property, option, val);
          }
        }
      } else if (argname == ArgumentType::STRING) {
        if (arg_.count(flag) == 0) {
          auto ArString = std::unique_ptr<ArgumentObject>(new ArgumentString);
          ArString->setArgPropertyOpt(property, option, val);
          arg_[flag].push_back(std::move(ArString));
        } else {
          for ( std::unique_ptr<ArgumentObject> & ptr : arg_[flag] ){
            ptr->setArgPropertyOpt(property, option, val);
          }
        }
      } else if (argname == ArgumentType::SWITCH) {
        if (switch_arg_.count(flag) == 0) {
          auto ArSwitch = std::unique_ptr<ArgumentObject>(new ArgumentSwitch);
          if (val == 0) {
            ArSwitch->setArgPropertyOpt(property, option, "OFF");
          } else if (val == 1) {
            ArSwitch->setArgPropertyOpt(property, option, "ON");
          } else {
            throw invalid_argument("Unrecognized option for switch " +
                to_string(val));
          }
          arg_[flag].push_back(std::move(ArSwitch));
        } else { // For a switch there will only be one value in the vector
          if (val == 0) {
            arg_[flag].at(0)->setArgPropertyOpt(property, option, "OFF");
          } else if (val == 1) {
            arg_[flag].at(0)->setArgPropertyOpt(property, option, "ON");
          } else {
            throw invalid_argument("Unrecognized option for switch " +
                to_string(val));
          }
        }
      } else if (argname == ArgumentType::FILES) {
        if (file_arg_.count(flag) == 0) {
          auto ArFile = std::unique_ptr<ArgumentObject>(new ArgumentFile);

          ArFile->setArgPropertyOpt(property, option, val);
          arg_[flag].push_back(std::move(ArFile));
        } else {
          for ( std::unique_ptr<ArgumentObject> & ptr : arg_[flag] ){
            ptr->setArgPropertyOpt(property, option, val);
          }
        }
      } else {
        throw invalid_argument("Unrecognized int arg");
      }*/
    }


/*  void setFlagArgOpt(std::string flag, std::string argname,
                     std::string property, std::string option, double val);

  void setFlagArgOpt(std::string flag, std::string argname,
                     std::string property, std::string option, std::string val);

  void setFlagArgOpt(std::string flag, std::string argname,
                     std::string property, std::string option,
                     std::set<std::string> vals);

  void setFlagArgOpt(std::string flag, std::string argname,
                     std::string property, std::string option,
                     std::vector<std::string> vals);

  std::string getFlagArgOptValue(std::string flag, std::string argname,
                                 std::string property, std::string option);
*/
  template<class T>
  std::vector<T> get(std::string flag) {
    std::vector<T> values;
    if ( values[flag].count(type_index(T)) ){
      for ( std::any & val : values[flag][std::type_index(T)] ){
        values.push_back(std::any_cast<T>(val));
      }
    }
    return values;
  }
/*  std::vector<double> getDoubles(std::string flag);
  std::vector<int> getInts(std::string flag);
  std::vector<std::string> getStrs(std::string flag);
  std::vector<size_t> getSize_ts(std::string flag);*/

  void postParseCheck(void);

  void parse(const char* argv[], int argc);
  void showUsage();
};

}  // namespace catnip
#endif  // _CATNIP_ARGUMENTPARSER_HPP
