#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../parameters.hpp"
#include "../string_support.hpp"
#include "io.hpp"

using namespace std;

namespace catnip {

unique_ptr<ArgumentParser> prepareParser(void) {

  // Setup flags
  vector<string> flag1;
  flag1.push_back("--pun_P");
  flag1.push_back("-p_P");
  string desc =
      "File contains the atomic orbital coefficients of the dimer of "
      "both monomer units. Must have the .pun/.orb/.7 file extension. This "
      "file "
      "is in the same format as the fort.7 file that comes out of a Gaussian "
      "run.";
  flag1.push_back(desc);

  vector<string> flag2;
  flag2.push_back("--pun_1");
  flag2.push_back("-p_1");
  desc =
      "File contains the atomic orbital coefficients of monomer 1. Must have "
      "the .pun/.orb/.7 file extension. This file is in the same format as the "
      "fort.7 file that comes out of a Gaussian run.";
  flag2.push_back(desc);

  vector<string> flag3;
  flag3.push_back("--pun_2");
  flag3.push_back("-p_2");
  desc =
      "File contains the atomic orbital coefficients of monomer 2. Must have "
      "the .pun/.orb/.7 file extension. This file is in the same format as the "
      "fort.7 file that comes out of a Gaussian run.";
  flag3.push_back(desc);

  vector<string> flag4;
  flag4.push_back("--log_P");
  flag4.push_back("-l_P");
  desc =
      "File contains the log output from a guassian run of a dimer system. "
      "It is used to determine the orbital energies, the HOMO and LUMO levels "
      "and the number of basis functions per atom if provided. The file must "
      "have"
      " the .log extension to be read.";
  flag4.push_back(desc);

  vector<string> flag5;
  flag5.push_back("--log_1");
  flag5.push_back("-l_1");
  desc =
      "File contains the log output from a guassian run of monomer 1. It is "
      "used to determine the orbital energies, the HOMO and LUMO levels and "
      "the "
      "number of basis functions per atom if provided. The file must have the "
      ".log extension to be read.";
  flag5.push_back(desc);

  vector<string> flag6;
  flag6.push_back("--log_2");
  flag6.push_back("-l_2");
  desc =
      "File contains the log output from a guassian run of monomer 2. It is "
      "used to determine the orbital energies, the HOMO and LUMO levels and "
      "the "
      "number of basis functions per atom if provided. The file must have the "
      ".log extension to be read.";
  flag6.push_back(desc);

  vector<string> flag7;
  flag7.push_back("--spin_P");
  flag7.push_back("-s_P");
  desc =
      "Specify which spin should be used for the dimer when running the "
      "transfer integral calculations. By default the same spin will be used "
      "for monomer 1, 2 and the dimer. The options are alpha or beta.";
  flag7.push_back(desc);

  vector<string> flag8;
  flag8.push_back("--spin_1");
  flag8.push_back("-s_1");
  desc =
      "Specify which spin should be used for monomer 1 when running the "
      "transfer integral calculations. By default the same spin will be used "
      "for monomer 1, 2 and the dimer. The options are alpha or beta.";
  flag8.push_back(desc);

  vector<string> flag9;
  flag9.push_back("--spin_2");
  flag9.push_back("-s_2");
  desc =
      "Specify which spin should be used for monomer 2 when running the "
      "transfer integral calculations. By default the same spin will be used "
      "for monomer 1, 2 and the dimer. The options are alpha or beta.";
  flag9.push_back(desc);

  vector<string> flag11;
  flag11.push_back("--orbital_type_1");
  flag11.push_back("-orb_ty_1");
  desc =
      "Specify which orbital type will be used for monomer 1. By default "
      "the HOMO will be used. Your options are HOMO or LUMO.";
  flag11.push_back(desc);

  vector<string> flag12;
  flag12.push_back("--orbital_type_2");
  flag12.push_back("-orb_ty_2");
  desc =
      "Specify which orbital type will be used for monomer 2. By default "
      "the HOMO will be used. Your options are HOMO or LUMO.";
  flag12.push_back(desc);

  vector<string> flag14;
  flag14.push_back("--orbital_num_1");
  flag14.push_back("-orb_num_1");
  desc =
      "Specify which orbital will be used for monomer 1. If you specified an "
      "orbital "
      "type of HOMO you must use 0 to indicate HOMO or a negative number. E.g. "
      "a value of -1 would access the HOMO-1. If the LUMO is specified 0 - "
      "represents the LUMO and is the default. To access another orbital "
      "specify"
      " a positive number. E.g. 4 would access the LUMO+4.";
  flag14.push_back(desc);

  vector<string> flag15;
  flag15.push_back("--orbital_num_2");
  flag15.push_back("-orb_num_2");
  desc =
      "Specify which orbital will be used for monomer 2. If you specified an "
      "orbital "
      "type of HOMO you must use 0 to indicate HOMO or a negative number. E.g. "
      "a value of -1 would access the HOMO-1. If the LUMO is specified 0 - "
      "represents the LUMO and is the default. To access another orbital "
      "specify"
      " a positive number. E.g. 4 would access the LUMO+4.";
  flag15.push_back(desc);

  vector<string> flag16;
  flag16.push_back("--counter_poise");
  flag16.push_back("-cp");
  desc =
      "Specify that the counter poise correction is being taken into account"
      " this means that there should be the same number of basis functions for "
      "the dimer as there are for each of the monomers.";
  flag16.push_back(desc);

  vector<string> flag17;
  flag17.push_back("--citation");
  flag17.push_back("-cite");
  desc = "Print software citation.";
  flag17.push_back(desc);

  vector<string> flag18;
  flag18.push_back("--version");
  flag18.push_back("-v");
  desc = "Print the version";
  flag18.push_back(desc);

  vector<string> flag19;
  flag19.push_back("--all");
  flag19.push_back("-a");
  desc = "Print all transfer integrals, in matrix form";
  flag19.push_back(desc);

  vector<string> flag20;
  flag20.push_back("--log");
  flag20.push_back("-l");
  desc = "Provide all .log files and allow catnip to figure the rest out.";
  flag20.push_back(desc);

  vector<string> flag21;
  flag21.push_back("--pun");
  flag21.push_back("-p");
  desc = "Provide all .pun/.7/.orb files and allow catnip to figure the rest out.";
  flag21.push_back(desc);

  set<vector<string>> flags;
  flags.insert(flag1);
  flags.insert(flag2);
  flags.insert(flag3);
  flags.insert(flag4);
  flags.insert(flag5);
  flags.insert(flag6);
  flags.insert(flag7);
  flags.insert(flag8);
  flags.insert(flag9);
  flags.insert(flag11);
  flags.insert(flag12);
  flags.insert(flag14);
  flags.insert(flag15);
  flags.insert(flag16);
  flags.insert(flag17);
  flags.insert(flag18);
  flags.insert(flag19);
  flags.insert(flag20);
  flags.insert(flag21);

  unique_ptr<ArgumentParser> ArgPars(new ArgumentParser(flags));

  // Setup rules for handling flags
  set<string> exts{".orb", ".7", ".pun"};
  string ext_log = ".log";

  {
    ArgPars->setFlagArgOpt("--log", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--log", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--log", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--pun", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);
  }

  // Setting up rules guiding pun files
  {
    ArgPars->setFlagArgOpt("--pun_P", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun_P", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun_P", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--pun_1", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun_1", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun_1", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--pun_2", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun_2", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun_2", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);

  }

  set<string> exts_log{".log"};
  vector<string> exts_other{".orb", ".7", ".pun"};
  // Setting up rules guiding log files

  // Setting rules guiding log files
  {
    ArgPars->setFlagArgOpt("--log_P", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, exts_log);
    ArgPars->setFlagArgOpt("--log_P", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, exts_other);
    ArgPars->setFlagArgOpt("--log_P", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--log_1", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, exts_log);
    ArgPars->setFlagArgOpt("--log_1", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, exts_other);
    ArgPars->setFlagArgOpt("--log_1", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--log_2", ArgumentType::FILES, PropertyType::FILE_EXT,
                           Option::ALLOWED_VALUES, exts_log);
    ArgPars->setFlagArgOpt("--log_2", ArgumentType::FILES, PropertyType::SISTER_FILE,
                           Option::ALLOWED_VALUES, exts_other);
    ArgPars->setFlagArgOpt("--log_2", ArgumentType::FILES, PropertyType::FILE_EXISTS,
                           Option::MUST_EXIST, 0);
  }

  set<string> spin_opts{"Alpha", "Beta"};

  // Setting rules guiding spin
  {
    ArgPars->setFlagArgOpt("--spin_P", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--spin_P", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ALLOWED_VALUES,
                           spin_opts);

    ArgPars->setFlagArgOpt("--spin_1", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--spin_1", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ALLOWED_VALUES,
                           spin_opts);

    ArgPars->setFlagArgOpt("--spin_2", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--spin_2", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ALLOWED_VALUES,
                           spin_opts);

    ArgPars->setFlagDefaultValue("--spin_P", "Alpha");
    ArgPars->setFlagDefaultValue("--spin_1", "Alpha");
    ArgPars->setFlagDefaultValue("--spin_2", "Alpha");
  }

  // Setting rules guidling orbital type
  set<string> orb_opts{"HOMO", "LUMO"};
  {
    ArgPars->setFlagArgOpt("--orbital_type_1", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--orbital_type_1", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ALLOWED_VALUES,
                           orb_opts);

    ArgPars->setFlagArgOpt("--orbital_type_2", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--orbital_type_2", ArgumentType::STRING,
                           PropertyType::STRING_CHOICE, Option::ALLOWED_VALUES,
                           orb_opts);

    ArgPars->setFlagDefaultValue("--orbital_type_1", "HOMO");
    ArgPars->setFlagDefaultValue("--orbital_type_2", "HOMO");
  }

  // Set argument allowed values for counterpoise
  {
    ArgPars->setFlagArgOpt("--counter_poise", ArgumentType::SWITCH,
                           PropertyType::SWITCH, Option::VALUE, "OFF");

    // By default the flag counter poise is turned off
    ArgPars->setFlagDefaultValue("--counter_poise", "OFF");
  }

  // Set argument for allowing printing of all transfer integrals
  {
    ArgPars->setFlagArgOpt("--all", ArgumentType::SWITCH,
        PropertyType::SWITCH, Option::VALUE, "OFF");

    // By default the flag counter poise is turned off
    ArgPars->setFlagDefaultValue("--all", "OFF");
  }

  {
    ArgPars->setFlagArgOpt("--citation", ArgumentType::SWITCH, PropertyType::SWITCH,
                           Option::VALUE, "OFF");

    // By default the flag citation is turned off
    ArgPars->setFlagDefaultValue("--citation", "OFF");
  }

  {
    ArgPars->setFlagArgOpt("--version", ArgumentType::SWITCH, PropertyType::SWITCH,
                           Option::VALUE, "OFF");

    // By default the flag citation is turned off
    ArgPars->setFlagDefaultValue("--version", "OFF");
  }

  // Set rules guiding orbital numbers
  // Use default settings for min and max numbers
  {
    ArgPars->addFlagArg("--orbital_num_1", ArgumentType::NUMERICAL);
    ArgPars->addFlagArg("--orbital_num_2", ArgumentType::NUMERICAL);

    int orb_num = 0;
    ArgPars->setFlagDefaultValue("--orbital_num_1", orb_num);
    ArgPars->setFlagDefaultValue("--orbital_num_2", orb_num);
  }
  return ArgPars;
}

unique_ptr<Parameters> prepareParameters(unique_ptr<ArgumentParser>& ArgParse) {

  map<string, string> flag_arg;
  map<string, string> flag_sister_arg;

  vector<string> flags{"--pun_P", "--pun_1", "--pun_2",
                       "--log_P", "--log_1", "--log_2"};

  vector<string> flags_sister{"--log_P", "--log_1", "--log_2",
                              "--pun_P", "--pun_1", "--pun_2"};

  for (size_t i = 0; i < flags.size(); ++i) {
    auto flag = flags.at(i);
    auto flag_sister = flags_sister.at(i);
    ArgumentType argu = ArgumentType::FILES;
    PropertyType prop = PropertyType::FILE_EXISTS;
    Option opt = Option::DOES_EXIST;

    string fileName;
    std::vector<bool> exists = ArgParse->get<bool>(flag, argu, prop, opt);
    if ( exists ) {
      flag_arg[flag] = ArgParse->getStrs(flag).at(0);
    }
    prop = PropertyType::SISTER_FILE;
    opt = Option::VALUE;
    std::string line = ArgParse->get<std::string>(flag, argu, prop, opt);
    opt = Option::FILE_PATH_NAME;
    std::string line2 = ArgParse->get<std::string>(flag, argu, prop, opt);
    auto sisters_exist = splitSt(line);
    vector<string> sister_file_paths = splitSt(line2);
    int countFiles = 0;
    for (auto sister_exists : sisters_exist) {
      if (sister_exists.compare("true") == 0) {
        flag_sister_arg[flag_sister] = sister_file_paths.at(countFiles);
      }
      ++countFiles;
    }
  }

  unique_ptr<Parameters> Par(new Parameters);

  bool generic_file_pun_input = true;
  bool generic_file_log_input = true;
  // Run file related checks
  if ( flag_arg.count("--pun_P") || 
      flag_arg.count("--pun_1") ||
      flag_arg.count("--pun_2") ) {
    generic_file_pun_input = false;
    if ( flag_arg.count("--pun")) {
      throw runtime_error("Cannot specify (--pun_P, --pun_1, --pun_2) options "
          "as well as --pun. These options are mutually exclusive");
    }
  }

  if ( flag_arg.count("--log_P") || 
      flag_arg.count("--log_1") ||
      flag_arg.count("--log_2") ) {
    generic_file_log_input = false;
    if ( flag_arg.count("--log")) {
      throw runtime_error("Cannot specify (--log_P, --log_1, --log_2) options "
          "as well as --log. These options are mutually exclusive");
    }
  }

  // Read file related flags
  if ( generic_file_pun_input == false ){
    if (flag_arg.count("--pun_P") != 0) {
      Par->setPunP(flag_arg["--pun_P"]);
    } else if (flag_sister_arg.count("--pun_P") != 0) {
      Par->setPunP(flag_sister_arg["--pun_P"]);
    } else {
      throw runtime_error("Unable to find file for flag --pun_P");
    }
    if (flag_arg.count("--pun_1") != 0) {
      Par->setPun1(flag_arg["--pun_1"]);
    } else if (flag_sister_arg.count("--pun_1") != 0) {
      Par->setPun1(flag_sister_arg["--pun_1"]);
    } else {
      throw runtime_error("Unable to find file for flag --pun_1");
    }
    if (flag_arg.count("--pun_2") != 0) {
      Par->setPun2(flag_arg["--pun_2"]);
    } else if (flag_sister_arg.count("--pun_2") != 0) {
      Par->setPun2(flag_sister_arg["--pun_2"]);
    } else {
      throw runtime_error("Unable to find file for flag --pun_2");
    }
  } else {

  }
  if( generic_file_log_input == false ){
    if (flag_arg.count("--log_P") != 0) {
      Par->setLogP(flag_arg["--log_P"]);
    } else if (flag_sister_arg.count("--log_P") != 0) {
      Par->setLogP(flag_sister_arg["--log_P"]);
    } else {
      throw runtime_error("Unable to find file for flag --log_P");
    }
    if (flag_arg.count("--log_1") != 0) {
      Par->setLog1(flag_arg["--log_1"]);
    } else if (flag_sister_arg.count("--log_1") != 0) {
      Par->setLog1(flag_sister_arg["--log_1"]);
    } else {
      throw runtime_error("Unable to find file for flag --log_1");
    }
    if (flag_arg.count("--log_2") != 0) {
      Par->setLog2(flag_arg["--log_2"]);
    } else if (flag_sister_arg.count("--log_2") != 0) {
      Par->setLog2(flag_sister_arg["--log_2"]);
    } else {
      throw runtime_error("Unable to find file for flag --log_2");
    }
  } else {

  }
  // Read spin related flags, do not need if statement because default values
  // are defined
  Par->setSpinP(ArgParse->getStrs("--spin_P").at(0));
  Par->setSpin1(ArgParse->getStrs("--spin_1").at(0));
  Par->setSpin2(ArgParse->getStrs("--spin_2").at(0));

  // Determine if we are doing a counterpoise calculation
  Par->setCounterPoise((ArgParse->getInts("--counter_poise")).at(0));
  Par->setPrintSwitch(ArgParse->getInts("--all").at(0));
  Par->setCitation(ArgParse->getInts("--citation").at(0));
  // Read Orbital related flags
  {
    string orb_typ_1 = ArgParse->getStrs("--orbital_type_1").at(0);
    Par->setOrbType1(orb_typ_1);
    if (orb_typ_1.compare("HOMO") == 0) {
      int MO = ArgParse->getInts("--orbital_num_1").at(0);
      if (MO > 0) {
        throw invalid_argument("HOMO orbital must be 0 or a negative number");
      }
      Par->setOrbNum1(MO);
    } else if (orb_typ_1.compare("LUMO") == 0) {
      int MO = ArgParse->getInts("--orbital_num_1").at(0);
      if (MO < 0) {
        throw invalid_argument("LUMO orbital must be 0 or a positive number");
      }
      Par->setOrbNum1(MO);
    }
  }

  {
    string orb_typ_2 = ArgParse->getStrs("--orbital_type_2").at(0);
    Par->setOrbType2(orb_typ_2);
    if (orb_typ_2.compare("HOMO") == 0) {
      int MO = ArgParse->getInts("--orbital_num_2").at(0);
      if (MO > 0) {
        throw invalid_argument("HOMO orbital must be 0 or a negative number");
      }
      Par->setOrbNum2(MO);
    } else if (orb_typ_2.compare("LUMO") == 0) {
      int MO = ArgParse->getInts("--orbital_num_2").at(0);
      if (MO < 0) {
        throw invalid_argument("LUMO orbital must be 0 or a positive number");
      }
      Par->setOrbNum2(MO);
    }
  }
  return Par;
}

}  // namespace catnip
