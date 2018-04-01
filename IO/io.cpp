#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <memory>

#include "io.hpp"
#include "../STRING_SUPPORT/string_support.hpp"
#include "../PARAMETERS/parameters.hpp"
using namespace std;

unique_ptr<ArgumentParser> prepareParser(void){

  // Setup flags  
  vector<string> flag1;
  flag1.push_back("--pun_P");
  flag1.push_back("-p_P");
  string desc ="File contains the atomic orbital coefficients of the dimer of "
    "both monomer units. Must have the .pun/.orb/.7 file extension. This file "
    "is in the same format as the fort.7 file that comes out of a Gaussian fun";
  flag1.push_back(desc);

  vector<string> flag2;
  flag2.push_back("--pun_1");
  flag2.push_back("-p_1");
  desc ="File contains the atomic orbital coefficients of monomer 1. Must have "
    "the .pun/.orb/.7 file extension. This file is in the same format as the "
    "fort.7 file that comes out of a Gaussian fun";
  flag2.push_back(desc);

  vector<string> flag3;
  flag3.push_back("--pun_2");
  flag3.push_back("-p_2");
  desc ="File contains the atomic orbital coefficients of monomer 2. Must have "
    "the .pun/.orb/.7 file extension. This file is in the same format as the "
    "fort.7 file that comes out of a Gaussian fun";
  flag3.push_back(desc);

  vector<string> flag4;
  flag4.push_back("--log_P");
  flag4.push_back("-l_P");
  desc = "File contains the log output from a guassian run of a dimer system. "
    "It is used to determine the orbital energies, the HOMO and LUMO levels "
    "and the number of basis functions per atom if provided. The file must have"
    " the .log extension to be read.";
  flag4.push_back(desc);

  vector<string> flag5;
  flag5.push_back("--log_1");
  flag5.push_back("-l_1");
  desc = "File contains the log output from a guassian run of monomer 1. It is "
    "used to determine the orbital energies, the HOMO and LUMO levels and the "
    "number of basis functions per atom if provided. The file must have the "
    ".log extension to be read.";
  flag5.push_back(desc);

  vector<string> flag6;
  flag6.push_back("--log_2");
  flag6.push_back("-l_2");
  desc = "File contains the log output from a guassian run of monomer 2. It is "
    "used to determine the orbital energies, the HOMO and LUMO levels and the "
    "number of basis functions per atom if provided. The file must have the "
    ".log extension to be read.";
  flag6.push_back(desc);

  set<vector<string>> flags = { 
    flag1,
    flag2,
    flag3,
    flag4,
    flag5,
    flag6};

  unique_ptr<ArgumentParser> ArgPars(new ArgumentParser(flags));

  // Setup rules for handling flags
  set<string> exts{".orb",".7",".pun"};
  string ext_log = ".log";

  ArgPars->setFlagArgOpt("--pun_P","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);
  ArgPars->setFlagArgOpt("--pun_P","ARGUMENT_FILE","PROPERTY_SISTER_FILE","ALLOWED_SISTER_FILE_EXT",ext_log);
  ArgPars->setFlagArgOpt("--pun_P","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",0);
  cerr << endl;

  ArgPars->setFlagArgOpt("--pun_1","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);
  auto vals = ArgPars->getFlagArgOptValue("--pun_P","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT");
  cerr << "vals " << vals << endl;
  ArgPars->setFlagArgOpt("--pun_1","ARGUMENT_FILE","PROPERTY_SISTER_FILE","ALLOWED_SISTER_FILE_EXT",ext_log);
  ArgPars->setFlagArgOpt("--pun_1","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",0);

  ArgPars->setFlagArgOpt("--pun_2","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);
  ArgPars->setFlagArgOpt("--pun_2","ARGUMENT_FILE","PROPERTY_SISTER_FILE","ALLOWED_SISTER_FILE_EXT",ext_log);
  ArgPars->setFlagArgOpt("--pun_2","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",0);

  set<string> exts_log{".log"};
  vector<string> exts_other{".orb",".7",".pun"};

  ArgPars->setFlagArgOpt("--log_P","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts_log);
  ArgPars->setFlagArgOpt("--log_P","ARGUMENT_FILE","PROPERTY_SISTER_FILE","ALLOWED_SISTER_FILE_EXT",exts_other);
  ArgPars->setFlagArgOpt("--log_P","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",0);

  ArgPars->setFlagArgOpt("--log_1","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts_log);
  ArgPars->setFlagArgOpt("--log_1","ARGUMENT_FILE","PROPERTY_SISTER_FILE","ALLOWED_SISTER_FILE_EXT",exts_other);
  ArgPars->setFlagArgOpt("--log_1","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",0);

  ArgPars->setFlagArgOpt("--log_2","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts_log);
  ArgPars->setFlagArgOpt("--log_2","ARGUMENT_FILE","PROPERTY_SISTER_FILE","ALLOWED_SISTER_FILE_EXT",exts_other);
  ArgPars->setFlagArgOpt("--log_2","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",0);

  return ArgPars;
}

unique_ptr<Parameters> prepareParameters(unique_ptr<ArgumentParser>& ArgParse){

  map<string,string> flag_arg;  
  map<string,string> flag_sister_arg;

  vector<string> flags{"--pun_P","--pun_1","--pun_2","--log_P","--log_1","--log_2"};
  vector<string> flags_sister{"--log_P","--log_1","--log_2","--pun_P","--pun_1","--pun_2"};

  for(size_t i=0;i<flags.size();++i ){
    auto flag = flags.at(i);
    auto flag_sister = flags_sister.at(i);
    string argu = "ARGUMENT_FILE";
    string prop = "PROPERTY_FILE_EXIST";
    string opt = "FILE_DOES_EXIST";

    string fileName;
    auto exists = ArgParse->getFlagArgOptValue(flag,argu,prop,opt);
    if(exists.compare("1")==0){
      flag_arg[flag] = ArgParse->getStr(flag);
    }
    prop = "PROPERTY_SISTER_FILE";
    opt = "SISTER_FILE_EXISTS";
    auto line = ArgParse->getFlagArgOptValue(flag,argu,prop,opt);
    auto sisters_exist = splitSt(line); 
    for(auto sister_exists : sisters_exist){
      if(sister_exists.compare("true")==0){
        opt = "SISTER_FILE_PATH_NAME";
        flag_sister_arg[flag_sister] = ArgParse->getFlagArgOptValue(flag,argu,prop,opt);
      }
    }
  }

  unique_ptr<Parameters> Par(new Parameters);
  if(flag_arg.count("--pun_P")!=0){
    Par->setPunP(flag_arg["--pun_P"]);
  }else if(flag_sister_arg.count("--pun_P")!=0){
    Par->setPunP(flag_sister_arg["--pun_P"]);
  }else{
    throw runtime_error("Unable to find file for flag --pun_P");
  }
  if(flag_arg.count("--pun_1")!=0){
    Par->setPun1(flag_arg["--pun_1"]);
  }else if(flag_sister_arg.count("--pun_1")!=0){
    Par->setPun1(flag_sister_arg["--pun_1"]);
  }else{
    throw runtime_error("Unable to find file for flag --pun_1");
  }
  if(flag_arg.count("--pun_2")!=0){
    Par->setPun2(flag_arg["--pun_2"]);
  }else if(flag_sister_arg.count("--pun_2")!=0){
    Par->setPun2(flag_sister_arg["--pun_2"]);
  }else{
    throw runtime_error("Unable to find file for flag --pun_2");
  }
  if(flag_arg.count("--log_P")!=0){
    Par->setLogP(flag_arg["--log_P"]);
  }else if(flag_sister_arg.count("--log_P")!=0){
    Par->setLogP(flag_sister_arg["--log_P"]);
  }else{
    throw runtime_error("Unable to find file for flag --log_P");
  }
  if(flag_arg.count("--log_1")!=0){
    Par->setLog1(flag_arg["--log_1"]);
  }else if(flag_sister_arg.count("--log_1")!=0){
    Par->setLog1(flag_sister_arg["--log_1"]);
  }else{
    throw runtime_error("Unable to find file for flag --log_1");
  }
  if(flag_arg.count("--log_2")!=0){
    Par->setLog2(flag_arg["--log_2"]);
  }else if(flag_sister_arg.count("--log_2")!=0){
    Par->setLog2(flag_sister_arg["--log_2"]);
  }else{
    throw runtime_error("Unable to find file for flag --log_2");
  }
  return Par;
}
