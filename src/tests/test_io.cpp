

#include <mamap/argumentparser.hpp>
#include "io/io.hpp"
#include "parameters.hpp"
#include "matrix.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(int argc, const char* argv[]) {

  auto arg_parser = prepareParser();

  arg_parser->parse(argv, argc);
  
  auto parameters = prepareParameters(std::move(arg_parser))

/*
  for (auto flag : flags) {
    cout << endl;
    ArgumentType argu = ArgumentType::FILES;
    PropertyType prop = PropertyType::FILE_EXT;
    Option opt = Option::ALLOWED_VALUES;
    auto values = ArgParse->get<string>(flag, argu, prop, opt);
    for ( std::string & val : values ){
      cout << val << " ";
    }
    cout << endl;
    prop = "PROPERTY_SISTER_FILE";
    opt = "ALLOWED_SISTER_FILE_EXT";
    auto val1 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val1
         << endl;

    opt = "SISTER_FILE_NAME";
    auto val2 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val2
         << endl;
    opt = "SISTER_FILE_PATH";
    auto val3 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val3
         << endl;
    opt = "SISTER_FILE_PATH_NAME";
    auto val4 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val4
         << endl;
    opt = "SISTER_FILE_EXISTS";
    auto val5 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val5
         << endl;

    prop = "PROPERTY_FILE_EXIST";
    opt = "FILE_MUST_EXIST";
    auto val6 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val6
         << endl;
    opt = "FILE_DOES_EXIST";
    auto val7 = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val7
         << endl;*/
  }

  auto Par = prepareParameters(ArgParse);
  return 0;
}
