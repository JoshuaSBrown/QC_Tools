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

  unique_ptr<ArgumentParser> ArgPars(new ArgumentParser(flags));

  // Setup rules for handling flags
  set<string> exts{".orb", ".7", ".pun"};
  string ext_log = ".log";

  // Setting up rules guiding pun files
  {
    ArgPars->setFlagArgOpt("--pun_P", "ARGUMENT_FILE", "PROPERTY_FILE_EXT",
                           "ALLOWED_FILE_EXT", exts);
    ArgPars->setFlagArgOpt("--pun_P", "ARGUMENT_FILE", "PROPERTY_SISTER_FILE",
                           "ALLOWED_SISTER_FILE_EXT", ext_log);
    ArgPars->setFlagArgOpt("--pun_P", "ARGUMENT_FILE", "PROPERTY_FILE_EXIST",
                           "FILE_MUST_EXIST", 0);

    ArgPars->setFlagArgOpt("--pun_1", "ARGUMENT_FILE", "PROPERTY_FILE_EXT",
                           "ALLOWED_FILE_EXT", exts);
    ArgPars->setFlagArgOpt("--pun_1", "ARGUMENT_FILE", "PROPERTY_SISTER_FILE",
                           "ALLOWED_SISTER_FILE_EXT", ext_log);
    ArgPars->setFlagArgOpt("--pun_1", "ARGUMENT_FILE", "PROPERTY_FILE_EXIST",
                           "FILE_MUST_EXIST", 0);

    ArgPars->setFlagArgOpt("--pun_2", "ARGUMENT_FILE", "PROPERTY_FILE_EXT",
                           "ALLOWED_FILE_EXT", exts);
    ArgPars->setFlagArgOpt("--pun_2", "ARGUMENT_FILE", "PROPERTY_SISTER_FILE",
                           "ALLOWED_SISTER_FILE_EXT", ext_log);
    ArgPars->setFlagArgOpt("--pun_2", "ARGUMENT_FILE", "PROPERTY_FILE_EXIST",
                           "FILE_MUST_EXIST", 0);
  }

  set<string> exts_log{".log"};
  vector<string> exts_other{".orb", ".7", ".pun"};
  // Setting up rules guiding log files

  // Setting rules guiding log files
  {
    ArgPars->setFlagArgOpt("--log_P", "ARGUMENT_FILE", "PROPERTY_FILE_EXT",
                           "ALLOWED_FILE_EXT", exts_log);
    ArgPars->setFlagArgOpt("--log_P", "ARGUMENT_FILE", "PROPERTY_SISTER_FILE",
                           "ALLOWED_SISTER_FILE_EXT", exts_other);
    ArgPars->setFlagArgOpt("--log_P", "ARGUMENT_FILE", "PROPERTY_FILE_EXIST",
                           "FILE_MUST_EXIST", 0);

    ArgPars->setFlagArgOpt("--log_1", "ARGUMENT_FILE", "PROPERTY_FILE_EXT",
                           "ALLOWED_FILE_EXT", exts_log);
    ArgPars->setFlagArgOpt("--log_1", "ARGUMENT_FILE", "PROPERTY_SISTER_FILE",
                           "ALLOWED_SISTER_FILE_EXT", exts_other);
    ArgPars->setFlagArgOpt("--log_1", "ARGUMENT_FILE", "PROPERTY_FILE_EXIST",
                           "FILE_MUST_EXIST", 0);

    ArgPars->setFlagArgOpt("--log_2", "ARGUMENT_FILE", "PROPERTY_FILE_EXT",
                           "ALLOWED_FILE_EXT", exts_log);
    ArgPars->setFlagArgOpt("--log_2", "ARGUMENT_FILE", "PROPERTY_SISTER_FILE",
                           "ALLOWED_SISTER_FILE_EXT", exts_other);
    ArgPars->setFlagArgOpt("--log_2", "ARGUMENT_FILE", "PROPERTY_FILE_EXIST",
                           "FILE_MUST_EXIST", 0);
  }

  set<string> spin_opts{"Alpha", "Beta"};

  // Setting rules guiding spin
  {
    ArgPars->setFlagArgOpt("--spin_P", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICE_ENFORCED",
                           "true");
    ArgPars->setFlagArgOpt("--spin_P", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICES",
                           spin_opts);

    ArgPars->setFlagArgOpt("--spin_1", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICE_ENFORCED",
                           "true");
    ArgPars->setFlagArgOpt("--spin_1", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICES",
                           spin_opts);

    ArgPars->setFlagArgOpt("--spin_2", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICE_ENFORCED",
                           "true");
    ArgPars->setFlagArgOpt("--spin_2", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICES",
                           spin_opts);

    ArgPars->setFlagDefaultValue("--spin_P", "Alpha");
    ArgPars->setFlagDefaultValue("--spin_1", "Alpha");
    ArgPars->setFlagDefaultValue("--spin_2", "Alpha");
  }

  // Setting rules guidling orbital type
  set<string> orb_opts{"HOMO", "LUMO"};
  {
    ArgPars->setFlagArgOpt("--orbital_type_1", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICE_ENFORCED",
                           "true");
    ArgPars->setFlagArgOpt("--orbital_type_1", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICES",
                           orb_opts);

    ArgPars->setFlagArgOpt("--orbital_type_2", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICE_ENFORCED",
                           "true");
    ArgPars->setFlagArgOpt("--orbital_type_2", "ARGUMENT_STRING",
                           "PROPERTY_STRING_CHOICE", "STRING_CHOICES",
                           orb_opts);

    ArgPars->setFlagDefaultValue("--orbital_type_1", "HOMO");
    ArgPars->setFlagDefaultValue("--orbital_type_2", "HOMO");
  }

  // Set argument allowed values for counterpoise
  {
    ArgPars->setFlagArgOpt("--counter_poise", "ARGUMENT_SWITCH",
                           "PROPERTY_SWITCH", "DEFAULT", "OFF");

    // By default the flag counter poise is turned off
    ArgPars->setFlagDefaultValue("--counter_poise", "OFF");
  }

  {
    ArgPars->setFlagArgOpt("--citation", "ARGUMENT_SWITCH", "PROPERTY_SWITCH",
                           "DEFAULT", "OFF");

    // By default the flag citation is turned off
    ArgPars->setFlagDefaultValue("--citation", "OFF");
  }

  {
    ArgPars->setFlagArgOpt("--version", "ARGUMENT_SWITCH", "PROPERTY_SWITCH",
                           "DEFAULT", "OFF");

    // By default the flag citation is turned off
    ArgPars->setFlagDefaultValue("--version", "OFF");
  }

  // Set rules guiding orbital numbers
  // Use default settings for min and max numbers
  {
    ArgPars->addFlagArg("--orbital_num_1", "ARGUMENT_INT");
    ArgPars->addFlagArg("--orbital_num_2", "ARGUMENT_INT");

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
    string argu = "ARGUMENT_FILE";
    string prop = "PROPERTY_FILE_EXIST";
    string opt = "FILE_DOES_EXIST";

    string fileName;
    auto exists = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    if (exists.compare("1") == 0) {
      flag_arg[flag] = ArgParse->getStr(flag);
    }
    prop = "PROPERTY_SISTER_FILE";
    opt = "SISTER_FILE_EXISTS";
    auto line = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    opt = "SISTER_FILE_PATH_NAME";
    auto line2 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
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
  // Read file related flags
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

  // Read spin related flags, do not need if statement because default values
  // are defined
  Par->setSpinP(ArgParse->getStr("--spin_P"));
  Par->setSpin1(ArgParse->getStr("--spin_1"));
  Par->setSpin2(ArgParse->getStr("--spin_2"));

  // Determine if we are doing a counterpoise calculation
  Par->setCounterPoise(ArgParse->getInt("--counter_poise"));
  Par->setCitation(ArgParse->getInt("--citation"));
  // Read Orbital related flags
  {
    string orb_typ_1 = ArgParse->getStr("--orbital_type_1");
    Par->setOrbType1(orb_typ_1);
    if (orb_typ_1.compare("HOMO") == 0) {
      int MO = ArgParse->getInt("--orbital_num_1");
      if (MO > 0) {
        throw invalid_argument("HOMO orbital must be 0 or a negative number");
      }
      Par->setOrbNum1(MO);
    } else if (orb_typ_1.compare("LUMO") == 0) {
      int MO = ArgParse->getInt("--orbital_num_1");
      if (MO < 0) {
        throw invalid_argument("LUMO orbital must be 0 or a positive number");
      }
      Par->setOrbNum1(MO);
    }
  }

  {
    string orb_typ_2 = ArgParse->getStr("--orbital_type_2");
    Par->setOrbType2(orb_typ_2);
    if (orb_typ_2.compare("HOMO") == 0) {
      int MO = ArgParse->getInt("--orbital_num_2");
      if (MO > 0) {
        throw invalid_argument("HOMO orbital must be 0 or a negative number");
      }
      Par->setOrbNum2(MO);
    } else if (orb_typ_2.compare("LUMO") == 0) {
      int MO = ArgParse->getInt("--orbital_num_2");
      if (MO < 0) {
        throw invalid_argument("LUMO orbital must be 0 or a positive number");
      }
      Par->setOrbNum2(MO);
    }
  }
  return Par;
}

}  // namespace catnip
