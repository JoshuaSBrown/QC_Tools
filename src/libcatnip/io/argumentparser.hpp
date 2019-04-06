#ifndef _CATNIP_ARGUMENTPARSER_HPP
#define _CATNIP_ARGUMENTPARSER_HPP
#include "../matrix.hpp"
#include "../parameters.hpp"
#include <map>
#include <set>
#include <string>
#include <utility>

#include "arguments/argumentdouble.hpp"
#include "arguments/argumentfile.hpp"
#include "arguments/argumentint.hpp"
#include "arguments/argumentstring.hpp"
#include "arguments/argumentswitch.hpp"

namespace catnip {

class ArgumentParser {
 private:
  // 1 - flag name
  // 2 - type
  std::map<std::string, ArgumentString*> str_arg_;
  std::map<std::string, ArgumentSwitch*> switch_arg_;
  std::map<std::string, ArgumentInt*> int_arg_;
  std::map<std::string, ArgumentDouble*> double_arg_;
  std::map<std::string, ArgumentFile*> file_arg_;

  // Known rules
  std::set<std::string> argument_types_;

  // Known flags and their description
  std::map<std::string, std::pair<std::string, std::string>> flags_;

  std::map<std::string, bool> defaults_set_;
  // Arguments that are stored when argc and argv are parsed
  std::map<std::string, int> int_values_;
  std::map<std::string, double> double_values_;
  std::map<std::string, std::string> string_values_;
  std::map<std::string, size_t> size_t_values_;

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
  void setFlagDefaultValue(std::string flag,const int & val);
  void setFlagDefaultValue(std::string flag,const size_t & val);
  void setFlagDefaultValue(std::string flag,const double & val);
  void setFlagDefaultValue(std::string flag,const std::string & val);

  // Add a argument without setting any of the values
  void addFlagArg(std::string flag, std::string argname);
  // Set the rules for the flag and the type it is associated with types
  // include:
  // "FILE_EXT"
  // "FILE_EXIST"
  // "DOUBLE"
  // "INT"
  // "STRING"
  // The rules are dependent on the type
  void setFlagArgOpt(std::string flag, std::string argname,
                     std::string property, std::string option, int val);

  void setFlagArgOpt(std::string flag, std::string argname,
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

  double getDouble(std::string flag);
  int getInt(std::string flag);
  std::string getStr(std::string flag);
  size_t getSize_t(std::string flag);

  void postParseCheck(void);

  void parse(const char* argv[], int argc);
  void showUsage();
};

}  // namespace catnip
#endif  // _CATNIP_ARGUMENTPARSER_HPP
