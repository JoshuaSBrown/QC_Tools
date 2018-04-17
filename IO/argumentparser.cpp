#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <vector>
#include <sys/stat.h>

#include "argumentparser.hpp"

#include "../STRING_SUPPORT/string_support.hpp"

using namespace std;

ArgumentParser::ArgumentParser(set<vector<string>> flags){

  vector<string> flag_help;
  flag_help.push_back("--help");
  flag_help.push_back("-h");
  flag_help.push_back("Display help message");
  flags.insert(flag_help);

  maxShortFlagSize = 0;
  maxFullFlagSize = 0;
  maxDescriptionSize = 0;   

  for( auto flag : flags ){

    if(flag.size()>3){
      throw invalid_argument("Flags may only be passed with a max of three "
        "values: full flag name, abbreviated name, and flag description");
    }

    string full_flag = "NO_FLAG_SPECIFIED";
    string short_flag = "NO_SHORT_FLAG_SPECIFIED";
    string desc = "NO_DESCRIPTION_GIVEN";
 
    bool description=false;
    bool abbreviation = false;
    bool flag_name = false;
    for(auto item : flag){
      trim(item);
      string doubledash = firstN(item,2);
      string singledash = firstN(item,1);
      if(doubledash.compare("--")==0){
        if(flag_name){
          string err = "A flag name has already been suppied:\n";
          err.append(full_flag);
          err.append("\nNow you are supplying a second one:\n");
          err.append(item);
          throw invalid_argument(err);
        }
        full_flag = item;
        defaults_set_[item]=false;
        flag_name = true;
        if(full_flag.size()>maxFullFlagSize){
          maxFullFlagSize = full_flag.size();
        }
      }else if(singledash.compare("-")==0){
        if(abbreviation){
          string err = "A flag name has already been suppied:\n";
          err.append(short_flag);
          err.append("\nNow you are supplying a second one:\n");
          err.append(item);
          throw invalid_argument(err);
        }
        short_flag = item;
        abbreviation = true;
        if(short_flag.size()>maxShortFlagSize){
          maxShortFlagSize = short_flag.size();
        }
      }else{
        if(description){
          string err = "A description has already been suppied:\n";
          err.append(desc);
          err.append("\nNow you are supplying a second one:\n");
          err.append(item);
          throw invalid_argument(err);
        }
        desc = item;
        description = true;
        if(desc.size()>maxDescriptionSize){
          maxDescriptionSize = desc.size();
        }
      } 
    }
    if(!flag_name){
      throw invalid_argument("You have failed to provide the full flag name");
    }
    flags_[full_flag] = make_pair(short_flag,desc);
  }

}

void ArgumentParser::showUsage(void){
  cout << endl;
  cout << endl;
  cout << "Options:" << endl;
  cout << endl;

  for(auto item : flags_ ){
    string full_flag = item.first;
    size_t diff = maxFullFlagSize-full_flag.size();
    cout << full_flag;
    for(size_t i=0;i<=diff;++i){
      cout << " ";
    }
    string short_flag = item.second.first;
    diff = maxShortFlagSize-short_flag.size();
    cout << short_flag;
    for(size_t i=0;i<=diff;++i){
      cout << " ";
    }

    size_t space = maxFullFlagSize+maxShortFlagSize+2;
    size_t allowed = maxLineLength-space;
    string space_block = "";
    for(size_t sp=0;sp<space;sp++){
      space_block.append(" ");
    }

    string desc = item.second.second;
    vector<string> vec_string = splitSt(desc);
    size_t words_size = 0;
    for( auto word : vec_string ){
      words_size+=word.size()+1;
      if(words_size<allowed){
        cout << word << " ";
      }else{
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

void ArgumentParser::addFlagArg(
  string flag,
  string argname){

  if(argname.compare("ARGUMENT_INT")==0){
    if(int_arg_.count(flag)==0){
      ArgumentInt * ArInt = new ArgumentInt;
      int_arg_[flag]=ArInt;
    }else{
      throw invalid_argument("Int argument has already been added for "
        "flag "+flag);
    }
  }else if(argname.compare("ARGUMENT_STRING")==0){
    if(str_arg_.count(flag)==0){
      ArgumentString * ArString = new ArgumentString;
      str_arg_[flag]=ArString;
    }else{
      throw invalid_argument("string argument has already been added for "
        "flag "+flag);
    }
  }else if(argname.compare("ARGUMENT_FILE")==0){
    if(file_arg_.count(flag)==0){
      ArgumentFile * ArFile = new ArgumentFile;
      file_arg_[flag]=ArFile;
    }else{
      throw invalid_argument("File argument has already been added for "
        "flag "+flag);
    }
  }else if(argname.compare("ARGUMENT_DOUBLE")==0){
    if(file_arg_.count(flag)==0){
      ArgumentDouble * ArDouble = new ArgumentDouble;
      double_arg_[flag]=ArDouble;
    }else{
      throw invalid_argument("Double argument has already been added for "
        "flag "+flag);
    }
  }else{
    throw invalid_argument("Unrecognized argument being added for flag"
      " argname is "+argname+" flag name is "+flag);
  }

}

void ArgumentParser::setFlagDefaultValue(string flag, int val){
  cerr << "Setting default value for int " << flag << endl;
  int_values_[flag] = val;
  defaults_set_[flag] = true;
}

void ArgumentParser::setFlagDefaultValue(string flag, size_t val){
  size_t_values_[flag] = val;
  defaults_set_[flag] = true;
}

void ArgumentParser::setFlagDefaultValue(string flag, double val){
  double_values_[flag] = val;
  defaults_set_[flag] = true;
}

void ArgumentParser::setFlagDefaultValue(string flag, string val){
  string_values_[flag] = val;
  defaults_set_[flag] = true;
}

void ArgumentParser::setFlagArgOpt(
  string flag, 
  string argname,
  string property,
  string option, 
  int val){

  if(argname.compare("ARGUMENT_INT")==0){
    if(int_arg_.count(flag)==0){
      ArgumentInt * ArInt = new ArgumentInt;
      ArInt->setArgPropertyOpt(property,option,val);
      int_arg_[flag]=ArInt;
    }else{
      int_arg_[flag]->setArgPropertyOpt(property,option,val);
    }
  }else if(argname.compare("ARGUMENT_STRING")==0){
    if(str_arg_.count(flag)==0){
      ArgumentString * ArString = new ArgumentString;
      ArString->setArgPropertyOpt(property,option,val);
      str_arg_[flag]=ArString;
    }else{
      str_arg_[flag]->setArgPropertyOpt(property,option,val);
    }
  }else if(argname.compare("ARGUMENT_FILE")==0){
    if(file_arg_.count(flag)==0){
      ArgumentFile * ArFile = new ArgumentFile;
      
      ArFile->setArgPropertyOpt(property,option,val);
      file_arg_[flag]=ArFile;
    }else{
      file_arg_[flag]->setArgPropertyOpt(property,option,val);
    }
  }else{
    throw invalid_argument("Unrecognized int arg");
  }
}

void ArgumentParser::setFlagArgOpt(
  string flag, 
  string argname,
  string property,
  string option, 
  double val){

  if(argname.compare("ARGUMENT_DOUBLE")==0){
    if(double_arg_.count(flag)==0){
      ArgumentDouble * ArDouble = new ArgumentDouble;  
      ArDouble->setArgPropertyOpt(property,option,val);
      double_arg_[flag]=ArDouble;
    }else{
      double_arg_[flag]->setArgPropertyOpt(property,option,val);
    }
  }else if(argname.compare("ARGUMENT_STRING")==0){
    if(str_arg_.count(flag)==0){
      ArgumentString * ArString = new ArgumentString;
      ArString->setArgPropertyOpt(property,option,val);
      str_arg_[flag]=ArString;
    }else{
      str_arg_[flag]->setArgPropertyOpt(property,option,val);
    }
  }else{
    throw invalid_argument("Unrecognized double arg");
  }
}


void ArgumentParser::setFlagArgOpt(
  string flag, 
  string argname,
  string property,
  string option, 
  string val){

  if(argname.compare("ARGUMENT_FILE")==0){
    if(file_arg_.count(flag)==0){

      if(property.compare("PROPERTY_FILE_EXT")==0){ 
        set<string> temp{val};    
        ArgumentFile * ArFile = new ArgumentFile;
        ArFile->setArgPropertyOpt(property,option,temp);
        file_arg_[flag] = ArFile;
      }else if(property.compare("PROPERTY_SISTER_FILE_EXT")==0){
        vector<string> temp{val};    
        ArgumentFile * ArFile = new ArgumentFile;
        ArFile->setArgPropertyOpt(property,option,temp);
        file_arg_[flag] = ArFile;
      }else{
        ArgumentFile * ArFile = new ArgumentFile;
        ArFile->setArgPropertyOpt(property,option,val);
        file_arg_[flag] = ArFile;
      }
    }else{
      if(property.compare("PROPERTY_FILE_EXT")==0){ 
        set<string> temp{val};    
        file_arg_[flag]->setArgPropertyOpt(property,option,temp);
      }else {
        file_arg_[flag]->setArgPropertyOpt(property,option,val);
      }
    }
  }else if(argname.compare("ARGUMENT_STRING")==0){
    if(str_arg_.count(flag)==0){
      set<string> temp{val};
      ArgumentString * ArStr = new ArgumentString;
      ArStr->setArgPropertyOpt(property,option,temp);  
      str_arg_[flag] = ArStr;
    }else{
      set<string> temp{val};
      str_arg_[flag]->setArgPropertyOpt(property,option,temp);
    }
  }else{
    throw invalid_argument("Unrecognized string arg "+argname);
  }
}

void ArgumentParser::setFlagArgOpt(
  string flag, 
  string argname,
  string property,
  string option, 
  set<string> vals){

  if(argname.compare("ARGUMENT_FILE")==0){
    if(file_arg_.count(flag)==0){
      ArgumentFile * ArFile = new ArgumentFile;
      ArFile->setArgPropertyOpt(property,option,vals);
      file_arg_[flag] = ArFile;
    }else{
      file_arg_[flag]->setArgPropertyOpt(property,option,vals);
    }
  }else if(argname.compare("ARGUMENT_STRING")==0){
    if(str_arg_.count(flag)==0){
      ArgumentString * ArStr = new ArgumentString;
      ArStr->setArgPropertyOpt(property,option,vals);
      str_arg_[flag] = ArStr;
    }else{
      str_arg_[flag]->setArgPropertyOpt(property,option,vals);
    }
  }else{
    throw invalid_argument("Unrecognized set<string> arg");
  }
}

void ArgumentParser::setFlagArgOpt(
  string flag, 
  string argname,
  string property,
  string option, 
  vector<string> vals){

  if(argname.compare("ARGUMENT_FILE")==0){
    if(file_arg_.count(flag)==0){
      ArgumentFile * ArFile = new ArgumentFile;
      ArFile->setArgPropertyOpt(property,option,vals);
      file_arg_[flag] = ArFile;
    }else{
      file_arg_[flag]->setArgPropertyOpt(property,option,vals);
    }
  }else{
    throw invalid_argument("Unrecognized vector<string> arg");
  }
}

void ArgumentParser::parseArg_(size_t & index, vector<string> arguments ){
  string flag = arguments.at(index);
  // If the abbreviated flag is passed we need to resolve the full flag name
  if(flags_.count(flag)==0){
    for(auto item : flags_ ){
      if(item.second.first.compare(flag)==0){
        flag = item.first; 
        break;
      }
    }
  }

  bool unrecognized = true;
  if(str_arg_.count(flag)!=0){
    if((index+1)>=arguments.size()){
      string err = ""+flag+" Missing arguments";
      throw runtime_error(err);
    }
    string argument = arguments.at(index+1);
    str_arg_[flag]->argValid(argument);
    unrecognized = false;
    string_values_[flag]=argument;
  }
  if(int_arg_.count(flag)!=0){
    if((index+1)>=arguments.size()){
      string err = ""+flag+" Missing arguments";
      throw runtime_error(err);
    }
    string argument = arguments.at(index+1);
    int_arg_[flag]->argValid(stoi(argument));
    unrecognized = false;
    int_values_[flag]=stoi(argument);
  }
  if(double_arg_.count(flag)!=0){
    if((index+1)>=arguments.size()){
      string err = ""+flag+" Missing arguments";
      throw runtime_error(err);
    }
    string argument = arguments.at(index+1);
    double_arg_[flag]->argValid(stod(argument));
    unrecognized = false;
    double_values_[flag]=stod(argument);
  }
  if(file_arg_.count(flag)!=0){
    if((index+1)>=arguments.size()){
      string err = ""+flag+" Missing arguments";
      throw runtime_error(err);
    }
    string argument = arguments.at(index+1);
    file_arg_[flag]->argValid(argument);
    unrecognized = false;
    string_values_[flag]=argument;
  }

  if(unrecognized){
    throw invalid_argument("The flag "+flag+" was unrecognized");
  }
  ++index;
}

string ArgumentParser::getFlagArgOptValue(
  string flag,
  string argname,
  string property,
  string option){

  if(str_arg_.count(flag)!=0){ 
    auto argStr = str_arg_[flag];
    auto argName = argStr->getArgumentName();
    if(argName.compare(argname)==0){
      return argStr->getPropertyValues(property,option);
    }
  }
  if(file_arg_.count(flag)!=0){ 
    auto argFile = file_arg_[flag];
    auto argName = argFile->getArgumentName();
    if(argName.compare(argname)==0){
      return argFile->getPropertyValues(property,option);
    }
  }
  if(int_arg_.count(flag)!=0){ 
    auto argInt = int_arg_[flag];
    auto argName = argInt->getArgumentName();
    if(argName.compare(argname)==0){
      return argInt->getPropertyValues(property,option);
    }
  }

  throw invalid_argument("Unrecognized flag or argument "+flag+" "+argname);
}

int ArgumentParser::getInt(string flag){
  if(int_values_.count(flag)==0){
    string err = ""+flag+" does not contain a int";
    throw invalid_argument(err);
  }
  return int_values_[flag];
}

double ArgumentParser::getDouble(string flag){
  if(double_values_.count(flag)==0){
    string err = ""+flag+" does not contain a double";
    throw invalid_argument(err);
  }
  return double_values_[flag];
}

string ArgumentParser::getStr(string flag){
  if(string_values_.count(flag)==0){
    string err = ""+flag+" does not contain a string";
    throw invalid_argument(err);
  }
  return string_values_[flag];
}

size_t ArgumentParser::getSize_t(string flag){
  if(size_t_values_.count(flag)==0){
    string err = ""+flag+" does not contain a size_t";
    throw invalid_argument(err);
  }
  return size_t_values_[flag];
}

void ArgumentParser::postParseCheck(void){

  for(auto str_arg : str_arg_ ){
    str_arg.second->postArgCheck();
  }
  for(auto int_arg : int_arg_ ){
    int_arg.second->postArgCheck();
  }
  for(auto double_arg : double_arg_ ){
    double_arg.second->postArgCheck();
  }
  for(auto file_arg : file_arg_ ){
    file_arg.second->postArgCheck();
  }
}

void ArgumentParser::parse(const char * argv[], int argc){

  // Convert to vector of strings
  vector<string> arguments(argv,argv+argc);

  if(argc <= 1){
	  cout << "Usage: " << arguments.at(0) << " <options(s)> SOURCES";
    showUsage();
    throw runtime_error("Must provide arguments");
  }

  string help_flag = "--help";
  string help_flag_short = "-h";

  for( size_t index=1; index<arguments.size();++index){
    if(help_flag.compare(arguments.at(index))==0 || 
       help_flag_short.compare(arguments.at(index))==0){
	    cout << "Usage: " << arguments.at(0) << " <options(s)> SOURCES";
      showUsage();
      if(2==arguments.size()){
        exit(0);
      }
    }else{
      parseArg_(index,arguments);    
    }
  }

  postParseCheck();
}
