#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <typeindex>
#include <vector>

#include "argumentparser.hpp"

#include "catnip_config.hpp"
#include "string_support.hpp"

using namespace std;

namespace catnip {

  std::unique_ptr<ArgumentObject> createArgument(ArgumentType type) {
    if (type == ArgumentType::NUMERICAL) {
      return std::unique_ptr<ArgumentObject>(new ArgumentNumerical);
    } else if (type == ArgumentType::STRING) {
      return std::unique_ptr<ArgumentObject>(new ArgumentString);
    } else if (type == ArgumentType::SWITCH) {
      return std::unique_ptr<ArgumentObject>(new ArgumentSwitch);
    } else if (type == ArgumentType::FILES) {
      return std::unique_ptr<ArgumentObject>(new ArgumentFile);
    }
    return std::unique_ptr<ArgumentObject>();
  }

  void ArgumentParser::addValue_(std::string flag, std::string value) {
    std::type_index value_type = arg_[flag].at(0)->getValueType(); 
    if (value_type == typeid(double) ){
      values_[flag][value_type].emplace_back(stod(value));
    } else if(value_type == typeid(int) ) {
      values_[flag][value_type].emplace_back(stoi(value));
    } else if(value_type == typeid(std::string) ) {
      values_[flag][value_type].emplace_back(value);
    } else if(value_type == typeid(bool) ){
      if (to_upper(value) == "TRUE" || value == "1" || to_upper(value) == "ON") {
        values_[flag][value_type].emplace_back(true);
      } else if (to_upper(value) == "FALSE" || value == "1" || to_upper(value) == "OFF") {
        values_[flag][value_type].emplace_back(false);
      }
    } else {
      std::runtime_error("Unknown type for argument value");
    }
  }

  ArgumentParser::ArgumentParser(set<vector<string>> flags) {

    vector<string> flag_help;
    flag_help.push_back("--help");
    flag_help.push_back("-h");
    flag_help.push_back("Display help message");
    flags.insert(flag_help);

    maxShortFlagSize = 0;
    maxFullFlagSize = 0;
    maxDescriptionSize = 0;

    for (auto flag : flags) {

      if (flag.size() > 3) {
        throw invalid_argument(
            "Flags may only be passed with a max of three "
            "values: full flag name, abbreviated name, and flag description");
      }

      string full_flag = "NO_FLAG_SPECIFIED";
      string short_flag = "NO_SHORT_FLAG_SPECIFIED";
      string desc = "NO_DESCRIPTION_GIVEN";

      bool description = false;
      bool abbreviation = false;
      bool flag_name = false;
      for (auto item : flag) {
        trim(item);
        string doubledash = firstN(item, 2);
        string singledash = firstN(item, 1);
        if (doubledash.compare("--") == 0) {
          if (flag_name) {
            string err = "A flag name has already been suppied:\n";
            err.append(full_flag);
            err.append("\nNow you are supplying a second one:\n");
            err.append(item);
            throw invalid_argument(err);
          }
          full_flag = item;
          defaults_set_[item] = false;
          flag_name = true;
          if (full_flag.size() > maxFullFlagSize) {
            maxFullFlagSize = full_flag.size();
          }
        } else if (singledash.compare("-") == 0) {
          if (abbreviation) {
            string err = "A flag name has already been suppied:\n";
            err.append(short_flag);
            err.append("\nNow you are supplying a second one:\n");
            err.append(item);
            throw invalid_argument(err);
          }
          short_flag = item;
          abbreviation = true;
          if (short_flag.size() > maxShortFlagSize) {
            maxShortFlagSize = short_flag.size();
          }
        } else {
          if (description) {
            string err = "A description has already been suppied:\n";
            err.append(desc);
            err.append("\nNow you are supplying a second one:\n");
            err.append(item);
            throw invalid_argument(err);
          }
          desc = item;
          description = true;
          if (desc.size() > maxDescriptionSize) {
            maxDescriptionSize = desc.size();
          }
        }
      }
      if (!flag_name) {
        throw invalid_argument("You have failed to provide the full flag name");
      }
      flags_[full_flag] = make_pair(short_flag, desc);
    }
  }

  void ArgumentParser::showUsage(void) {
    cout << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << endl;

    for (auto item : flags_) {
      string full_flag = item.first;
      size_t diff = maxFullFlagSize - full_flag.size();
      cout << full_flag;
      for (size_t i = 0; i <= diff; ++i) {
        cout << " ";
      }
      string short_flag = item.second.first;
      diff = maxShortFlagSize - short_flag.size();
      cout << short_flag;
      for (size_t i = 0; i <= diff; ++i) {
        cout << " ";
      }

      size_t space = maxFullFlagSize + maxShortFlagSize + 2;
      size_t allowed = maxLineLength - space;
      string space_block = "";
      for (size_t sp = 0; sp < space; sp++) {
        space_block.append(" ");
      }

      string desc = item.second.second;
      vector<string> vec_string = splitSt(desc);
      size_t words_size = 0;
      for (auto word : vec_string) {
        words_size += word.size() + 1;
        if (words_size < allowed) {
          cout << word << " ";
        } else {
          cout << endl;
          cout << space_block;
          cout << word << " ";
          words_size = 0;
        }
      }
      cout << endl;
      cout << endl;
    }
    cout << endl;
  }

  void ArgumentParser::addFlagArg(string flag, ArgumentType argname) {

    arg_[flag].push_back(createArgument(argname));
  /*  if (argname == ArgumentType::NUMERICAL) {
        auto ArInt = std::unique_ptr<ArgumentObject>(new ArgumentNumerical);
        int_arg_[flag].push_back(std::move(ArInt));
    } else if (argname == ArgumentType::SWITCH) {
      if (switch_arg_.count(flag) == 0) {
        auto ArSwitch = std::unique_ptr<ArgumentObject>(new ArgumentSwitch);
        switch_arg_[flag].push_back(std::move(ArSwitch));
      } else {
        throw invalid_argument(
            "switch argument has already been added for "
            "flag " +
            flag);
      }
    } else if (argname == ArgumentType::STRING) {
        auto ArString = std::unique_ptr<ArgumentObject>(new ArgumentString);
        str_arg_[flag].push_back(std::move(ArString));
    } else if (argname == ArgumentType::FILES) {
        auto ArFile = std::unique_ptr<ArgumentObject>(new ArgumentFile);
        file_arg_[flag].push_back(std::move(ArFile));
    } else {
      throw invalid_argument(
          "Unrecognized argument being added for flag"
          " argname is " +
          argname + " flag name is " + flag);
    }*/
  }
  /*
  // Will apply values to all properties in the argument vector
  void ArgumentParser::setFlagArgOpt(string flag, string argname, string property,
                                     string option, int val) {

    if (argname.compare("ARGUMENT_INT") == 0) {
      if (int_arg_.count(flag) == 0) {
        auto ArInt = std::unique_ptr<ArgumentInt>(new ArgumentInt);
        ArInt->setArgPropertyOpt(property, option, val);
        int_arg_[flag].push_back(std::move(ArInt));
      } else {
        for ( std::unique_ptr<ArgumentInt> & ptr : int_arg_[flag]){
          ptr->setArgPropertyOpt(property, option, val);
        }
      }
    } else if (argname.compare("ARGUMENT_STRING") == 0) {
      if (str_arg_.count(flag) == 0) {
        auto ArString = std::unique_ptr<ArgumentString>(new ArgumentString);
        ArString->setArgPropertyOpt(property, option, val);
        str_arg_[flag].push_back(std::move(ArString));
      } else {
        for ( std::unique_ptr<ArgumentString> & ptr : str_arg_[flag] ){
          ptr->setArgPropertyOpt(property, option, val);
        }
      }
    } else if (argname.compare("ARGUMENT_SWITCH") == 0) {
      if (switch_arg_.count(flag) == 0) {
        auto ArSwitch = std::unique_ptr<ArgumentSwitch>(new ArgumentSwitch);
        if (val == 0) {
          ArSwitch->setArgPropertyOpt(property, option, "OFF");
        } else if (val == 1) {
          ArSwitch->setArgPropertyOpt(property, option, "ON");
        } else {
          throw invalid_argument("Unrecognized option for switch " +
                                 to_string(val));
        }
        switch_arg_[flag].push_back(std::move(ArSwitch));
      } else { // For a switch there will only be one value in the vector
        if (val == 0) {
          switch_arg_[flag].at(0)->setArgPropertyOpt(property, option, "OFF");
        } else if (val == 1) {
          switch_arg_[flag].at(0)->setArgPropertyOpt(property, option, "ON");
        } else {
          throw invalid_argument("Unrecognized option for switch " +
                                 to_string(val));
        }
      }
    } else if (argname.compare("ARGUMENT_FILE") == 0) {
      if (file_arg_.count(flag) == 0) {
        auto ArFile = std::unique_ptr<ArgumentFile>(new ArgumentFile);

        ArFile->setArgPropertyOpt(property, option, val);
        file_arg_[flag].push_back(std::move(ArFile));
      } else {
        for ( std::unique_ptr<ArgumentFile> & ptr : file_arg_[flag] ){
          ptr->setArgPropertyOpt(property, option, val);
        }
      }
    } else {
      throw invalid_argument("Unrecognized int arg");
    }
  }

  void ArgumentParser::setFlagArgOpt(string flag, string argname, string property,
                                     string option, double val) {

    if (argname.compare("ARGUMENT_DOUBLE") == 0) {
      if (double_arg_.count(flag) == 0) {
        auto ArDouble = std::unique_ptr<ArgumentDouble>(new ArgumentDouble);
        ArDouble->setArgPropertyOpt(property, option, val);
        double_arg_[flag].push_back(std::move(ArDouble));
      } else {
        for ( std::unique_ptr<ArgumentDouble> & ptr : double_arg_[flag]) {
          ptr->setArgPropertyOpt(property, option, val);
        }
      }
    } else if (argname.compare("ARGUMENT_STRING") == 0) {
      if (str_arg_.count(flag) == 0) {
        auto ArString = std::unique_ptr<ArgumentString>(new ArgumentString);
        ArString->setArgPropertyOpt(property, option, val);
        str_arg_[flag].push_back(std::move(ArString));
      } else {
        for ( std::unique_ptr<ArgumentString> & ptr : str_arg_[flag]) {
          ptr->setArgPropertyOpt(property, option, val);
        }
      }
    } else {
      throw invalid_argument("Unrecognized double arg");
    }
  }

  void ArgumentParser::setFlagArgOpt(string flag, string argname, string property,
                                     string option, string val) {

    if (argname.compare("ARGUMENT_FILE") == 0) {
      if (file_arg_.count(flag) == 0) {

        if (property.compare("PROPERTY_FILE_EXT") == 0) {
          set<string> temp{val};
          auto ArFile = std::unique_ptr<ArgumentFile>(new ArgumentFile);
          ArFile->setArgPropertyOpt(property, option, temp);
          file_arg_[flag].push_back(std::move(ArFile));
        } else if (property.compare("PROPERTY_SISTER_FILE_EXT") == 0) {
          vector<string> temp{val};
          auto ArFile = std::unique_ptr<ArgumentFile>(new ArgumentFile);
          ArFile->setArgPropertyOpt(property, option, temp);
          file_arg_[flag].push_back(std::move(ArFile));
        } else {
          auto ArFile = std::unique_ptr<ArgumentFile>(new ArgumentFile);
          ArFile->setArgPropertyOpt(property, option, val);
          file_arg_[flag].push_back(std::move(ArFile));
        }
      } else {
        if (property.compare("PROPERTY_FILE_EXT") == 0) {
          set<string> temp{val};
          for ( std::unique_ptr<ArgumentFile> & ptr : file_arg_[flag]) {
            ptr->setArgPropertyOpt(property, option, temp);
          }
        } else {
          for ( std::unique_ptr<ArgumentFile> & ptr : file_arg_[flag]) {
            ptr->setArgPropertyOpt(property, option, val);
          }
        }
      }
    } else if (argname.compare("ARGUMENT_SWITCH") == 0) {
      if (switch_arg_.count(flag) == 0) {
        auto ArSwitch = std::unique_ptr<ArgumentSwitch>(new ArgumentSwitch);
        if (val.compare("OFF") != 0 && val.compare("ON") != 0) {
          throw invalid_argument("Unrecognized option for switch " + val);
        } else {
          ArSwitch->setArgPropertyOpt(property, option, val);
        }
        switch_arg_[flag].push_back(std::move(ArSwitch));
      } else {
        if (val.compare("OFF") != 0 && val.compare("ON") != 0) {
          throw invalid_argument("Unrecognized option for switch" + val);
        } else {
          for ( std::unique_ptr<ArgumentSwitch> & ptr : switch_arg_[flag]) {
            ptr->setArgPropertyOpt(property, option, val);
          }
        }
      }

    } else if (argname.compare("ARGUMENT_STRING") == 0) {
      if (str_arg_.count(flag) == 0) {
        set<string> temp{val};
        auto ArStr = std::unique_ptr<ArgumentString>(new ArgumentString);
        ArStr->setArgPropertyOpt(property, option, temp);
        str_arg_[flag].push_back(std::move(ArStr));
      } else {
        set<string> temp{val};
        for ( std::unique_ptr<ArgumentString> & ptr : str_arg_[flag]) {
          ptr->setArgPropertyOpt(property, option, temp);
        }
      }
    } else {
      throw invalid_argument("Unrecognized string arg " + argname);
    }
  }

  void ArgumentParser::setFlagArgOpt(string flag, string argname, string property,
                                     string option, set<string> vals) {

    if (argname.compare("ARGUMENT_FILE") == 0) {
      if (file_arg_.count(flag) == 0) {
        auto ArFile = std::unique_ptr<ArgumentFile>(new ArgumentFile);
        ArFile->setArgPropertyOpt(property, option, vals);
        file_arg_[flag].push_back(std::move(ArFile));
      } else {
        for ( std::unique_ptr<ArgumentFile> & ptr : file_arg_[flag]) {
          ptr->setArgPropertyOpt(property, option, vals);
        }
      }
    } else if (argname.compare("ARGUMENT_STRING") == 0) {
      if (str_arg_.count(flag) == 0) {
        auto ArStr = std::unique_ptr<ArgumentString>(new ArgumentString);
        ArStr->setArgPropertyOpt(property, option, vals);
        str_arg_[flag].push_back(std::move(ArStr));
      } else {
        for ( std::unique_ptr<ArgumentString> & ptr : str_arg_[flag]) {
          ptr->setArgPropertyOpt(property, option, vals);
        }
      }
    } else {
      throw invalid_argument("Unrecognized set<string> arg");
    }
  }

  void ArgumentParser::setFlagArgOpt(string flag, string argname, string property,
                                     string option, vector<string> vals) {

    if (argname.compare("ARGUMENT_FILE") == 0) {
      if (file_arg_.count(flag) == 0) {
        auto ArFile = std::unique_ptr<ArgumentFile>(new ArgumentFile);
        ArFile->setArgPropertyOpt(property, option, vals);
        file_arg_[flag].push_back(std::move(ArFile));
      } else {
        for ( std::unique_ptr<ArgumentFile> & ptr : file_arg_[flag]) {
          ptr->setArgPropertyOpt(property, option, vals);
        }
      }
    } else {
      throw invalid_argument("Unrecognized vector<string> arg");
    }
  }
  */
  bool ArgumentParser::nextParameterIsAFlag_(size_t index,
                                             vector<string> arguments) {

    if ((index + 1) < arguments.size()) {
      for (auto flag : flags_) {
        if (flag.first.compare(arguments.at(index + 1)) == 0) {
          return true;
        } else if (flag.second.first.compare(arguments.at(index + 1)) == 0) {
          return true;
        }
      }
    }
    return false;
  }

  void ArgumentParser::parseArg_(size_t& index, vector<string> arguments) {
    string flag = arguments.at(index);
    // If the abbreviated flag is passed we need to resolve the full flag name
    if (flags_.count(flag) == 0) {
      for (auto item : flags_) {
        if (item.second.first.compare(flag) == 0) {
          flag = item.first;
          break;
        }
      }
    }

    bool unrecognized = true;

    // Parse String Arguments
    if (arg_.count(flag) != 0) {

      ArgumentType type = arg_[flag].front()->getArgumentType(); 
      // Switch does not require an argument
      if ( type == ArgumentType::SWITCH && 
          (nextParameterIsAFlag_(index, arguments) ||
           (index + 1) >= arguments.size())){

        unrecognized = false;
        if(defaults_set_[flag] == true ){
          values_[flag][std::type_index(typeid(bool))].at(0) = true;
          addValue_(flag, "TRUE");
        }else {
          addValue_(flag, "TRUE");
        }
      } else { 
        if ((index + 1) >= arguments.size()) {
          string err = "" + flag + " Missing arguments";
          throw runtime_error(err);
        }
        int arg_index = 0;
        do { // Allow reading of multiple arguments
          string argument = arguments.at(index + 1);
          if ( arg_index != 0){
            // Need to duplicate the argument
            addFlagArg(flag, type);
            ArgumentType type = arg_[flag].front()->getArgumentType();
            std::vector<PropertyType> arg_props = arg_[flag].front()->getProperties();
              
            auto values = arg_[flag].front()->getPropertyValues();
            arg_[flag].push_back(createArgument(type));
            arg_[flag].back()->setValues(values);
            // TODO Need to specify index setFlagArgOpt(flag, arg_name, property, option, val);
          }
          arg_[flag].at(arg_index)->argValid(argument);
          unrecognized = false;
          if (defaults_set_.count(flag) ) {
            if(defaults_set_[flag] == true ){
              values_[flag].clear();
              addValue_(flag,argument);
              defaults_set_[flag] = false;
            } else {
              addValue_(flag,argument);
            }
          }
          ++index;
          ++arg_index;
          if( (index+1) >= arguments.size() ) break;
        } while ( arguments.at(index+1).at(0) == '-' );
      }
    }
  /*
    // Parse Switches
    if (switch_arg_.count(flag) != 0) {
      if (!nextParameterIsAFlag_(index, arguments)) {
        if ((index + 1) >= arguments.size()) {
          if (switch_arg_[flag].at(0)->requiresParameter()) {
            string err = "" + flag + " Missing arguments";
            throw runtime_error(err);
          } else {
            unrecognized = false;
            int_values_[flag].at(0) = 1;
          }
        } else {
          string argument = arguments.at(index + 1);
          switch_arg_[flag].at(0)->argValid(argument);
          unrecognized = false;
          string_values_[flag].at(0) = argument;
          if (switch_arg_[flag].at(0)->positive(argument)) {
            if (defaults_set_.count(flag) ) {
              if(defaults_set_[flag] == true ){
                int_values_[flag].at(0) = 1;
                defaults_set_[flag] = false;
              } else {
                int_values_[flag].push_back(1);
              }
            }
          } else {
            if (defaults_set_.count(flag) ) {
              if(defaults_set_[flag] == true ){
                int_values_[flag].at(0) = 0;
                defaults_set_[flag] = false;
              } else {
                int_values_[flag].push_back(0);
              }
            }
          }
        }
      } else {
        unrecognized = false;
        if(defaults_set_[flag] == true ){
          int_values_[flag].at(0) = 1;
          defaults_set_[flag] = false;
        }else {
          int_values_[flag].push_back(1);
        }
      }
    }

    // Parse integer arguments
    if (int_arg_.count(flag) != 0) {
      if (!nextParameterIsAFlag_(index, arguments) ||
          int_arg_[flag].at(0)->requiresParameter()) {
        if ((index + 1) >= arguments.size()) {
          string err = "" + flag + " Missing arguments";
          throw runtime_error(err);
        }

        int arg_index = 0;
        do {
          string argument = arguments.at(index + 1);
          if ( arg_index != 0){
            // Need to duplicate the argument
            std::string arg_name = int_arg_[flag].at(0)->getArgumentName();
            std::vector<std::string> arg_props = int_arg_[flag].at(0)->getProperties();
            std::vector<std::string> arg_prop_opts = int_arg_[flag].at(0)->getPropertyOptions();
            std::map<std::string, std::string> arg_prop_vals = int_arg_[flag].at(0)->getPropertyValues();
          }
          int_arg_[flag].at(arg_index)->argValid(stoi(argument));
          unrecognized = false;
          if (defaults_set_.count(flag) ) {
            if(defaults_set_[flag] == true ){
              string_values_[flag].clear();
              int_values_[flag].push_back(stoi(argument));
              defaults_set_[flag] = false;
            } else {
              int_values_[flag].push_back(stoi(argument));
            }
          }
          ++index;
          ++arg_index;
          if( (index+1) >= arguments.size() ) break;
        } while ( arguments.at(index+1).at(0) == '-' );
      }
    }

    // Parse double arguments
    if (double_arg_.count(flag) != 0) {
      if (!nextParameterIsAFlag_(index, arguments) ||
          double_arg_[flag].at(0)->requiresParameter()) {
        if ((index + 1) >= arguments.size()) {
          string err = "" + flag + " Missing arguments";
          throw runtime_error(err);
        }
        int arg_index = 0;
        do {
          string argument = arguments.at(index + 1);
          if ( arg_index != 0){
            // Need to duplicate the argument
            std::string arg_name = double_arg_[flag].at(0)->getArgumentName();
            std::vector<std::string> arg_props = double_arg_[flag].at(0)->getProperties();
            std::vector<std::string> arg_prop_opts = double_arg_[flag].at(0)->getProperties();
            std::map<std::string, std::string> arg_prop_vals = double_arg_[flag].at(0)->getPropertyValues();
          }
          double_arg_[flag].at(arg_index)->argValid(stod(argument));
          unrecognized = false;
          if (defaults_set_.count(flag) ) {
            if(defaults_set_[flag] == true ){
              string_values_[flag].clear();
              double_values_[flag].push_back(stod(argument));
              defaults_set_[flag] = false;
            } else {
              double_values_[flag].push_back(stod(argument));
            }
          }
          ++index;
          ++arg_index;
          if( (index+1) >= arguments.size() ) break;
        } while ( arguments.at(index+1).at(0) == '-' );
      }
    }

    // Parse file arguments 
    if (file_arg_.count(flag) != 0) {
      if (!nextParameterIsAFlag_(index, arguments) ||
          file_arg_[flag].at(0)->requiresParameter()) {
        if ((index + 1) >= arguments.size()) {
          string err = "" + flag + " Missing arguments";
          throw runtime_error(err);
        }
        int arg_index = 0;
        do {
          string argument = arguments.at(index + 1);
          if ( arg_index != 0){
            // Need to duplicate the argument
            std::string arg_name = file_arg_[flag].at(0)->getArgumentName();
            std::vector<std::string> arg_props = file_arg_[flag].at(0)->getProperties();
            std::vector<std::string> arg_prop_opts = file_arg_[flag].at(0)->getProperties();
            std::map<std::string, std::string> arg_prop_vals = file_arg_[flag].at(0)->getPropertyValues();
          }
          file_arg_[flag].at(arg_index)->argValid(argument);
          unrecognized = false;
          if (defaults_set_.count(flag) ) {
            if(defaults_set_[flag] == true ){
              string_values_[flag].clear();
              string_values_[flag].push_back(argument);
              defaults_set_[flag] = false;
            } else {
              string_values_[flag].push_back(argument);
            }
          }
          ++index;
          ++arg_index;
          if( (index+1) >= arguments.size() ) break;
        } while ( arguments.at(index+1).at(0) == '-' );
      }
    }*/

    if (unrecognized) {
      throw invalid_argument("The flag " + flag + " was unrecognized");
    }
  }


  /*
  std::vector<int> ArgumentParser::getInts(string flag) {

    if (int_values_.count(flag) == 0) {
      if (switch_arg_.count(flag)) {
        if (string_values_.count(flag)) {
          if (switch_arg_[flag]->positive(string_values_[flag])) {
            return std::vector<int> {1};
          } else {
            return std::vector<int> {0};
          }
        }
      }
      string err = "" + flag +
                   " does not contain an int. It is also possible that "
                   "you did not specify a default value";
      throw invalid_argument(err);
    }
    return int_values_[flag];
  }

  std::vector<double> ArgumentParser::getDoubles(string flag) {
    if (double_values_.count(flag) == 0) {
      string err = "" + flag +
                   " does not contain a double. It is also possible that "
                   "you did not specify a default value";
      throw invalid_argument(err);
    }
    return double_values_[flag];
  }

  std::vector<string> ArgumentParser::getStrs(string flag) {
    if (string_values_.count(flag) == 0) {
      if (switch_arg_.count(flag)) {
        if (int_values_.count(flag)) {
          if (switch_arg_[flag]->positive(int_values_[flag])) {
            return std::vector<string> {"ON"};
          } else {
            return std::vector<string> {"OFF"};
          }
        }
      }
      string err = "" + flag +
                   " does not contain a string. It is also possible that "
                   "you did not specify a default value";
      throw invalid_argument(err);
    }
    return string_values_[flag];
  }

  std::vector<size_t> ArgumentParser::getSize_ts(string flag) {
    if (size_t_values_.count(flag) == 0) {
      string err = "" + flag +
                   " does not contain a size_t. It is also possible that "
                   "you did not specify a default value";
      throw invalid_argument(err);
    }
    return size_t_values_[flag];
  }
  */
  void ArgumentParser::postParseCheck(void) {
    //for (std::pair<std::string, std::vector<std::unique_ptr<ArgumentObject>>> & flag_arg : arg_) {
    for (auto & flag_arg : arg_) {
      for ( std::unique_ptr<ArgumentObject> & arg : flag_arg.second ){
        arg->postArgCheck();
      }
    }
  }

  void ArgumentParser::parse(const char* argv[], int argc) {

    // Convert to vector of strings
    vector<string> arguments(argv, argv + argc);

    if (argc <= 1) {
      cout << "Usage: " << arguments.at(0) << " <options(s)> SOURCES";
      showUsage();
      throw runtime_error("Must provide arguments");
    }

    string help_flag = "--help";
    string help_flag_short = "-h";
    string citation_flag = "--citation";
    string citation_flag_short = "-cite";
    string version_flag = "--version";
    string version_flag_short = "-version";

    size_t allowed_args_before_exit = 2;
    for (size_t index = 1; index < arguments.size(); ++index) {
      if (help_flag.compare(arguments.at(index)) == 0 ||
          help_flag_short.compare(arguments.at(index)) == 0) {
        cout << "Usage: " << arguments.at(0) << " <options(s)> SOURCES";
        showUsage();
        if (allowed_args_before_exit == arguments.size()) {
          exit(0);
        }
        ++allowed_args_before_exit;
      } else if (version_flag.compare(arguments.at(index)) == 0 ||
                 version_flag_short.compare(arguments.at(index)) == 0) {
        exit(0);
      } else if (citation_flag.compare(arguments.at(index)) == 0 ||
                 citation_flag_short.compare(arguments.at(index)) == 0) {
        cout << "\nCitation: " << catnip_AUTHOR_SURNAME << ", ";
        cout << catnip_AUTHOR_INITIALS << " (";
        cout << catnip_YEAR_PUBLISHED << "). ";
        cout << catnip_TITLE << " (Version ";
        cout << catnip_VERSION_MAJOR << ".";
        cout << catnip_VERSION_MINOR << "). [Software]. Available from ";
        cout << catnip_URL << ".\n\n";
        if (allowed_args_before_exit == arguments.size()) {
          exit(0);
        }
        ++allowed_args_before_exit;
      } else {
        parseArg_(index, arguments);
      }
    }

    postParseCheck();
  }
}
