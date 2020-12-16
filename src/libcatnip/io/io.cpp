#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <mamap/arguments/argumenttypes.hpp>
#include <mamap/arguments/properties/propertyoptions.hpp>
#include <mamap/argumentparser.hpp>
#include <mamap/string_support.hpp>

#include "parameters.hpp"
#include "io.hpp"

#include <mamap/argumentparser.hpp>

using namespace std;

namespace catnip {

unique_ptr<mamap::ArgumentParser> prepareParser(void) {

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
  desc = "Provide all .log files and allow catnip to figure the rest out. ";
  desc += "\nCaveats\n";
  desc += "1. if the --pun file flag is not used the pun/orb/.7 files must ";
  desc += "appear in the same folder as the log files and have the same core ";
  desc += "file name as the log file.";
  desc += "2. if the --pun file flag is also provided the same number of files ";
  desc += "must be provided as is provided with the --log flag, and they must ";
  desc += "either be entered in the same order or they must have the same core";
  desc += " file name.";
  flag20.push_back(desc);

  vector<string> flag21;
  flag21.push_back("--pun");
  flag21.push_back("-p");
  desc = "Provide all .punt/.7/.orb files and allow catnip to figure the rest out. ";
  desc += "\nCaveats\n";
  desc += "1. if the --log file flag is not used the .log files must ";
  desc += "appear in the same folder as the .pun/.7/.orb files and have the same core ";
  desc += "file name as the .pun/.7/.orb file.";
  desc += "2. if the --log file flag is also provided the same number of files ";
  desc += "must be provided as is provided with the --pun flag, and they must ";
  desc += "either be entered in the same order or they must have the same core";
  desc += " file name.";
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

  unique_ptr<mamap::ArgumentParser> ArgPars(new mamap::ArgumentParser(flags));

  // Setup rules for handling flags
  set<string> exts{".orb", ".7", ".pun"};
  string ext_log = ".log";

  {
    ArgPars->setFlagArgOpt("--log", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--log", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--log", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, true);

    ArgPars->setFlagArgOpt("--pun", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, true);
  }

  // Setting up rules guiding pun files
  {
    ArgPars->setFlagArgOpt("--pun_P", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun_P", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun_P", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--pun_1", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun_1", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun_1", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--pun_2", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::ALLOWED_VALUES, exts);
    ArgPars->setFlagArgOpt("--pun_2", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, ext_log);
    ArgPars->setFlagArgOpt("--pun_2", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, 0);

  }

  set<string> exts_log{".log"};
  vector<string> exts_other{".orb", ".7", ".pun"};
  // Setting up rules guiding log files

  // Setting rules guiding log files
  {
    ArgPars->setFlagArgOpt("--log_P", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, exts_log);
    ArgPars->setFlagArgOpt("--log_P", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, exts_other);
    ArgPars->setFlagArgOpt("--log_P", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--log_1", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, exts_log);
    ArgPars->setFlagArgOpt("--log_1", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, exts_other);
    ArgPars->setFlagArgOpt("--log_1", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, 0);

    ArgPars->setFlagArgOpt("--log_2", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXT,
                           mamap::Option::ALLOWED_VALUES, exts_log);
    ArgPars->setFlagArgOpt("--log_2", mamap::ArgumentType::FILES, mamap::PropertyType::SISTER_FILE,
                           mamap::Option::ALLOWED_VALUES, exts_other);
    ArgPars->setFlagArgOpt("--log_2", mamap::ArgumentType::FILES, mamap::PropertyType::FILE_EXISTS,
                           mamap::Option::MUST_EXIST, 0);
  }

  set<string> spin_opts{"Alpha", "Beta"};

  // Setting rules guiding spin
  {
    ArgPars->setFlagArgOpt("--spin_P", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--spin_P", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ALLOWED_VALUES,
                           spin_opts);

    ArgPars->setFlagArgOpt("--spin_1", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--spin_1", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ALLOWED_VALUES,
                           spin_opts);

    ArgPars->setFlagArgOpt("--spin_2", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--spin_2", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ALLOWED_VALUES,
                           spin_opts);

    ArgPars->setFlagDefaultValue("--spin_P", "Alpha");
    ArgPars->setFlagDefaultValue("--spin_1", "Alpha");
    ArgPars->setFlagDefaultValue("--spin_2", "Alpha");
  }

  // Setting rules guidling orbital type
  set<string> orb_opts{"HOMO", "LUMO"};
  {
    ArgPars->setFlagArgOpt("--orbital_type_1", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--orbital_type_1", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ALLOWED_VALUES,
                           orb_opts);

    ArgPars->setFlagArgOpt("--orbital_type_2", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ENFORCED,
                           "true");
    ArgPars->setFlagArgOpt("--orbital_type_2", mamap::ArgumentType::STRING,
                           mamap::PropertyType::STRING_CHOICE, mamap::Option::ALLOWED_VALUES,
                           orb_opts);

    ArgPars->setFlagDefaultValue("--orbital_type_1", "HOMO");
    ArgPars->setFlagDefaultValue("--orbital_type_2", "HOMO");
  }

  // Set argument allowed values for counterpoise
  {
    ArgPars->setFlagArgOpt("--counter_poise", mamap::ArgumentType::SWITCH,
                           mamap::PropertyType::SWITCH, mamap::Option::VALUE, "OFF");

    // By default the flag counter poise is turned off
    ArgPars->setFlagDefaultValue("--counter_poise", "OFF");
  }

  // Set argument for allowing printing of all transfer integrals
  {
    ArgPars->setFlagArgOpt("--all", mamap::ArgumentType::SWITCH,
        mamap::PropertyType::SWITCH, mamap::Option::VALUE, "OFF");

    // By default the flag counter poise is turned off
    ArgPars->setFlagDefaultValue("--all", "OFF");
  }

  {
    ArgPars->setFlagArgOpt("--citation", mamap::ArgumentType::SWITCH, mamap::PropertyType::SWITCH,
                           mamap::Option::VALUE, "OFF");

    // By default the flag citation is turned off
    ArgPars->setFlagDefaultValue("--citation", "OFF");
  }

  {
    ArgPars->setFlagArgOpt("--version", mamap::ArgumentType::SWITCH, mamap::PropertyType::SWITCH,
                           mamap::Option::VALUE, "OFF");

    // By default the flag citation is turned off
    ArgPars->setFlagDefaultValue("--version", "OFF");
  }

  // Set rules guiding orbital numbers
  // Use default settings for min and max numbers
  {
    ArgPars->addFlagArg("--orbital_num_1", mamap::ArgumentType::NUMERIC);
    ArgPars->addFlagArg("--orbital_num_2", mamap::ArgumentType::NUMERIC);

    int orb_num = 0;
    ArgPars->setFlagDefaultValue("--orbital_num_1", orb_num);
    ArgPars->setFlagDefaultValue("--orbital_num_2", orb_num);
  }
  return ArgPars;
}

unique_ptr<Parameters> prepareParameters(unique_ptr<mamap::ArgumentParser>& ArgParse) {

  map<string, std::vector<string>> flag_arg;
  map<string, std::vector<string>> flag_sister_arg;

  vector<string> flags{"--pun_P", "--pun_1", "--pun_2","--pun",
                       "--log_P", "--log_1", "--log_2","--log"};

  vector<string> flags_sister{"--log_P", "--log_1", "--log_2","--log",
                              "--pun_P", "--pun_1", "--pun_2","--pun"};

  // Determine if each log or pun file has a sister file, and if it does store
  // it in case that particular file it not passed in. 
  for (size_t i = 0; i < flags.size(); ++i) {
    // 1. Get the possible fileNames of the sister files
    // When a sister file is calcualted it takes the core file name and 
    // generates the appropriate substitutes. E.g. if -logs flag is provided
    // --logs core.log 
    //
    // then the sister file names will be calculated as
    // core.pun
    // core.7
    // core.orb
    //
    // But to determine if any of these files exist we have to get the value
    // of the DOES_EXIST option
    const std::string & flag = flags.at(i);
    const std::string & sister_flag = flags_sister.at(i);

    // Should return a vector of vector of strings
    // The first vector is for each argument passed to the flag
    // the second vector is the option values associated with each argument
    // E.g. for the --pun flag
    //
    // --log dir1/file1.log dir2/file2.log dir3/file3.log
    //
    // There are 3 arguments passed to --log 
    //
    // Each argument will have a set of potential sister files associate with
    // it
    //
    // dir1/file1.log   :   dir1/file1.log dir1/file1.7 dir1/file1.orb
    // dir2/file2.log   :   dir2/file2.log dir2/file2.7 dir2/file2.orb
    // dir3/file3.log   :   dir3/file3.log dir3/file3.7 dir3/file3.orb
    auto arg_potential_sister_files = 
      ArgParse->getFlagArgOptValues<std::vector<std::string>>(
        flag,
        mamap::ArgumentType::FILES,
        mamap::PropertyType::SISTER_FILE,
        mamap::Option::FILE_PATH_NAME);

    // Does the file passed in as an argument actually exist, using the example
    // above
    //
    // --log dir1/file1.log dir2/file2.log dir3/file3.log
    //
    // This would check that dir1/file1.log dir2/file2.log and dir3/file3.log
    // exists. will return a vector of bools
    auto arg_potential_files_exist = 
      ArgParse->getFlagArgOptValues<bool>(
          flag,
          mamap::ArgumentType::FILES,
          mamap::PropertyType::FILE_EXISTS,
          mamap::Option::DOES_EXIST);

    // 2. Determine which files exist and choose a suitable substitute
    //
    // Should return a vector of vectors of bools
    auto arg_potential_sister_files_exist = 
      ArgParse->getFlagArgOptValues<std::vector<bool>>(
        flag,
        mamap::ArgumentType::FILES,
        mamap::PropertyType::SISTER_FILE,
        mamap::Option::DOES_EXIST);

    // 3. Get the arguments associated with a particular flag
    // Using the example above this would be
    // dir1/file1.log dir2/file2.log dir3/file3.log
    std::vector<std::string> file_names = ArgParse->get<std::string>(flag);

    size_t num_args = ArgParse->getNumArgs(flag);

    for( size_t arg_ind = 0; arg_ind <num_args; ++arg_ind){
      // Vector of bools, that specefies if each potential sister file 
      // of the argument actually exists
      std::vector<bool> sister_files_exists = 
        arg_potential_sister_files_exist.at(arg_ind);
      // Vector of string, that each potential sister file 
      // of the argument 
      std::vector<std::string> potential_sister_files = 
        arg_potential_sister_files.at(arg_ind);

      for( size_t k = 0; k<potential_sister_files.size(); ++k){
        if( sister_files_exists.at(k) ) {
          // Only grab the first one that exists
          flag_sister_arg[sister_flag].push_back(potential_sister_files.at(k));
          break;
        }
      
        // 3. Get the actual argument passed in, check if the file exists
        if( arg_potential_files_exist.at(arg_ind) ) {
          flag_arg[flag].push_back(file_names.at(arg_ind)); 
        } 
      }
      
    } 

  }

  unique_ptr<Parameters> Par(new Parameters);

  bool generic_file_pun_input = true;
  bool generic_file_log_input = true;
  // Run file related checks
  
  // 1. Check that:
  // --pun_P 
  // --pun_1
  // --pun_2
  //
  // are not also submitted with --pun, which should be mutually exclusive
  if ( flag_arg.count("--pun_P") || 
      flag_arg.count("--pun_1") ||
      flag_arg.count("--pun_2") ) {
    generic_file_pun_input = false;
    if ( flag_arg.count("--pun")) {
      throw runtime_error("Cannot specify (--pun_P, --pun_1, --pun_2) options "
          "as well as --pun. These options are mutually exclusive");
    }
  }

  // 2. Check that:
  // --log_P 
  // --log_1
  // --log_2
  //
  // are not also submitted with --log, which should be mutually exclusive
  if ( flag_arg.count("--log_P") || 
      flag_arg.count("--log_1") ||
      flag_arg.count("--log_2") ) {
    generic_file_log_input = false;
    if ( flag_arg.count("--log")) {
      throw runtime_error("Cannot specify (--log_P, --log_1, --log_2) options "
          "as well as --log. These options are mutually exclusive");
    }
  }


  size_t num_logs =0;
  size_t num_puns =0;

  // 3. Using the dimer flags
  // --pun_P
  // --pun_1
  // --pun_2 
  //
  // Check that the files were found, if the pun flags were not
  // found attempt to generate the file from a sister file.  
  if ( generic_file_pun_input == false ){
    if (flag_arg.count("--pun_P") != 0) {
      Par->setPunP(flag_arg.at("--pun_P").at(0));
    } else if (flag_sister_arg.count("--pun_P") != 0) {
      Par->setPunP(flag_sister_arg.at("--pun_P").at(0));
    } else {
      throw runtime_error("Unable to find file for flag --pun_P");
    }
    if (flag_arg.count("--pun_1") != 0) {
      Par->setPun1(flag_arg.at("--pun_1").at(0));
    } else if (flag_sister_arg.count("--pun_1") != 0) {
      Par->setPun1(flag_sister_arg.at("--pun_1").at(0));
    } else {
      throw runtime_error("Unable to find file for flag --pun_1");
    }
    if (flag_arg.count("--pun_2") != 0) {
      Par->setPun2(flag_arg.at("--pun_2").at(0));
    } else if (flag_sister_arg.count("--pun_2") != 0) {
      Par->setPun2(flag_sister_arg.at("--pun_2").at(0));
    } else {
      throw runtime_error("Unable to find file for flag --pun_2");
    }
  } else {
    // 4. Using the generic flag pun
    // Check that files were passed in with the pun flag if not attempt 
    // to use sister files that were generated by looking at the log files
    if (flag_arg.count("--pun") != 0) {
      Par->setPuns(flag_arg.at("--pun"));
      num_puns = flag_arg.at("--pun").size();
    } else if (flag_sister_arg.count("--pun") != 0) {
      Par->setPuns(flag_sister_arg.at("--pun"));
      num_puns = flag_sister_arg.at("--pun").size();
    } else {
      throw runtime_error("Unable to find file for flag --pun");
    }

  }

  if( generic_file_log_input == false ){
    if (flag_arg.count("--log_P") != 0) {
      Par->setLogP(flag_arg.at("--log_P").at(0));
    } else if (flag_sister_arg.count("--log_P") != 0) {
      Par->setLogP(flag_sister_arg.at("--log_P").at(0));
    } else {
      throw runtime_error("Unable to find file for flag --log_P");
    }
    if (flag_arg.count("--log_1") != 0) {
      Par->setLog1(flag_arg.at("--log_1").at(0));
    } else if (flag_sister_arg.count("--log_1") != 0) {
      Par->setLog1(flag_sister_arg.at("--log_1").at(0));
    } else {
      throw runtime_error("Unable to find file for flag --log_1");
    }
    if (flag_arg.count("--log_2") != 0) {
      Par->setLog2(flag_arg.at("--log_2").at(0));
    } else if (flag_sister_arg.count("--log_2") != 0) {
      Par->setLog2(flag_sister_arg.at("--log_2").at(0));
    } else {
      throw runtime_error("Unable to find file for flag --log_2");
    }
  } else {
    // 4. Using the generic flag log
    // Check that files were passed in with the pun flag if not attempt 
    // to use sister files that were generated by looking at the pun files
    if (flag_arg.count("--log") != 0) {
      Par->setLogs(flag_arg.at("--log"));
      num_logs = flag_arg.at("--log").size();
    } else if (flag_sister_arg.count("--log") != 0) {
      Par->setLogs(flag_sister_arg.at("--log"));
      num_logs = flag_sister_arg.at("--log").size();
    } else {
      throw runtime_error("Unable to find file for flag --log");
    }

  }


  // If we are using the generic file options --pun and --log and both flags
  // are provided the flags must have an equal number of arguments, otherwise
  // it becomes impossible to match the appropriate log file with the
  // appropriate pun file
  //
  // E.g. I can't have 
  //
  // --pun File1.pun File3.pun --log File1.log File2,log File3.log
  //
  // Because I don't know which pun file is missing
  //
  // The file names passed to these flags must be in order as well
  //
  // --pun File1.pun File3.pun File2.pun --log File1.log File2.log File3.log
  //
  // Will not work because I will not be able to identify that File2 and File3
  // are not associated with each other the input must look like this
  //
  // --pun file1.pun file2.pun file3.pun --log file1.log file2.log file3.log
  //
  // though it does not matter the order they are passed in E.g. this is ok
  //
  // --pun file2.pun file3.pun file1.pun --log file2.log file3.log file1.log
  //
  // As long as the order is consistent between --log and --pun flags
  if ( generic_file_log_input && generic_file_pun_input) {

    if(num_logs != num_puns ) {
      std::string error_msg = "There must be an equal number of log and pun files";
      error_msg += "\nThe following files have been detected\n\nLogs .log :";
      for ( std::string & log_file : Par->getLogs() ) {
        error_msg += log_file + "\n";
      }
      error_msg += "\n\nPuns pun/.7/.orb :";
      for ( std::string & pun_file : Par->getPuns() ) {
        error_msg += pun_file + "\n";
      }
      throw std::runtime_error(error_msg);
    }
  }

  // If --pun or --log flags are used then non of the other flags are 
  // appropriate
  if( generic_file_pun_input || generic_file_log_input ) {
    if( ArgParse->getNumArgs("--spin_P") != 0){
      throw std::runtime_error("--spin_P command cannot be used with the --pun and --log flags");
    }
    if( ArgParse->getNumArgs("--spin_1") != 0){
      throw std::runtime_error("--spin_1 command cannot be used with the --pun and --log flags");
    }
    if( ArgParse->getNumArgs("--spin_2") != 0){
      throw std::runtime_error("--spin_2 command cannot be used with the --pun and --log flags");
    }
    if( ArgParse->getNumArgs("--orbital_type_1") != 0){
      throw std::runtime_error("--orbital_type_1 command cannot be used with the --pun and --log flags");
    }
    if( ArgParse->getNumArgs("--orbital_type_2") != 0){
      throw std::runtime_error("--orbital_type_2 command cannot be used with the --pun and --log flags");
    }
    if( ArgParse->getNumArgs("--orbital_num_1") != 0){
      throw std::runtime_error("--orbital_num_1 command cannot be used with the --pun and --log flags");
    }
    if( ArgParse->getNumArgs("--orbital_num_2") != 0){
      throw std::runtime_error("--orbital_num_2 command cannot be used with the --pun and --log flags");
    }
  } else {
    // Read spin related flags, do not need if statement because default values
    // are defined
    Par->setSpinP(ArgParse->get<std::string>("--spin_P").at(0));
    Par->setSpin1(ArgParse->get<std::string>("--spin_1").at(0));
    Par->setSpin2(ArgParse->get<std::string>("--spin_2").at(0));

    // Read Orbital related flags
    {
      string orb_typ_1 = ArgParse->get<std::string>("--orbital_type_1").at(0);
      Par->setOrbType1(orb_typ_1);
      if (orb_typ_1.compare("HOMO") == 0) {
        int MO = ArgParse->get<int>("--orbital_num_1").at(0);
        if (MO > 0) {
          throw invalid_argument("HOMO orbital must be 0 or a negative number");
        }
        Par->setOrbNum1(MO);
      } else if (orb_typ_1.compare("LUMO") == 0) {
        int MO = ArgParse->get<int>("--orbital_num_1").at(0);
        if (MO < 0) {
          throw invalid_argument("LUMO orbital must be 0 or a positive number");
        }
        Par->setOrbNum1(MO);
      }
    }

    {
      string orb_typ_2 = ArgParse->get<std::string>("--orbital_type_2").at(0);
      Par->setOrbType2(orb_typ_2);
      if (orb_typ_2.compare("HOMO") == 0) {
        int MO = ArgParse->get<int>("--orbital_num_2").at(0);
        if (MO > 0) {
          throw invalid_argument("HOMO orbital must be 0 or a negative number");
        }
        Par->setOrbNum2(MO);
      } else if (orb_typ_2.compare("LUMO") == 0) {
        int MO = ArgParse->get<int>("--orbital_num_2").at(0);
        if (MO < 0) {
          throw invalid_argument("LUMO orbital must be 0 or a positive number");
        }
        Par->setOrbNum2(MO);
      }
    }
  }
  // Determine if we are doing a counterpoise calculation
  Par->setCounterPoise((ArgParse->get<int>("--counter_poise")).at(0));
  Par->setPrintSwitch(ArgParse->get<int>("--all").at(0));
  Par->setCitation(ArgParse->get<int>("--citation").at(0));
  return Par;
}

}  // namespace catnip
