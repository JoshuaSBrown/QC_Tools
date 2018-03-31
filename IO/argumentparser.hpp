#ifndef _ARGUMENTPARSER_HPP
#define _ARGUMENTPARSER_HPP
#include <string>
#include <map>
#include <set>
#include <utility>
#include "../MATRIX/matrix.hpp"
#include "../PARAMETERS/parameters.hpp"

#include "ARGUMENTS/argumentdouble.hpp"
#include "ARGUMENTS/argumentint.hpp"
#include "ARGUMENTS/argumentstring.hpp"
#include "ARGUMENTS/argumentfile.hpp"

class ArgumentParser{
  private:

    // 1 - flag name
    // 2 - type
    std::map<std::string,ArgumentString*> str_arg_;
    std::map<std::string,ArgumentInt*> int_arg_;
    std::map<std::string,ArgumentDouble*> double_arg_;
    std::map<std::string,ArgumentFile*> file_arg_;
    
    // Known rules
    std::set<std::string> argument_types_; 

    // Known flags and their description 
    std::map<std::pair<std::string,std::string>,std::string> flags_;

    std::map<std::string,int> int_values_;
    std::map<std::string,double> double_values_;
    std::map<std::string,std::string> string_values_;
    std::map<std::string,size_t> size_t_values_;
    std::map<std::string,bool> bool_values_;

    void parseArg_(size_t & index, std::vector<std::string> arguments);
   
//    void registerRules_(void); 

    size_t maxShortFlagSize;
    size_t maxFullFlagSize;
    size_t maxDescriptionSize;
    size_t maxLineLength = 80;
  public:
    // 1 - short flag
    // 2 - long flag
    // 3 - description
    ArgumentParser(std::set<std::vector<std::string>> flags);

    // Set the rules for the flag and the type it is associated with types 
    // include:
    // "FILE_EXT"
    // "FILE_EXIST"
    // "DOUBLE"
    // "INT"
    // "STRING"
    // The rules are dependent on the type
    void setFlagArgOpt(
      std::string flag,
      std::string argname,
      std::string property,
      std::string option,
      int val);

    void setFlagArgOpt(
      std::string flag,
      std::string argname,
      std::string property,
      std::string option,
      double val);
/*
    void setFlagArgOpt(
      std::string flag,
      std::string argname,
      std::string property,
      std::string option,
      size_t val);
*/
    void setFlagArgOpt(
      std::string flag,
      std::string argname,
      std::string property,
      std::string option,
      std::string val);

    void setFlagArgOpt(
      std::string flag,
      std::string argname,
      std::string property,
      std::string option,
      std::set<std::string> vals);

    void setFlagArgOpt(
      std::string flag,
      std::string argname,
      std::string property,
      std::string option,
      bool val);


    double getDouble(std::string flag);
    int getInt(std::string flag);
    std::string getStr(std::string flag);
    bool getBool(std::string flag);
    size_t getSize_t(std::string flag);

    void parse( const char * argv[], int argc);
    void showUsage();
};

#endif
