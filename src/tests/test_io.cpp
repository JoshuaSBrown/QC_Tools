

#include "../libcatnip/io/argumentparser.hpp"
#include "../libcatnip/io/io.hpp"
#include "../libcatnip/matrix.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(int argc, const char* argv[]) {

  auto ArgParse = prepareParser();

  ArgParse->parse(argv, argc);
  vector<string> flags{"--pun_P", "--pun_1", "--pun_2",
                       "--log_P", "--log_1", "--log_2"};

  for (auto flag : flags) {
    cout << endl;
    string argu = "ARGUMENT_FILE";
    string prop = "PROPERTY_FILE_EXT";
    string opt = "ALLOWED_FILE_EXT";
    auto val = ArgParse->getFlagArgOptValue(flag, argu, prop, opt);
    cout << flag << " " << argu << " " << prop << " " << opt << " " << val
         << endl;

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
         << endl;
  }

  auto Par = prepareParameters(ArgParse);
  return 0;
}
