
#include "../libcatnip/io/arguments/properties/propertysisterfile.hpp"
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {
  fstream fs;
  fs.open("testfile.pun", ios::out);
  fs.close();

  cerr << "Testing: PropertySisterFile" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertySisterFile propSisterFile1; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertySisterFile propSisterFile;
    string name = propSisterFile.getPropertyName();
    assert(name.compare("PROPERTY_SISTER_FILE") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertySisterFile propSisterFile;
    auto options = propSisterFile.getPropertyOptions();

    bool allowed_file = false;
    bool sister_name = false;
    bool sister_path = false;
    bool sister_name_path = false;
    bool sister_exists = false;

    for (auto opt : options) {
      if (opt.compare("ALLOWED_SISTER_FILE_EXT") == 0) {
        allowed_file = true;
      }
      if (opt.compare("SISTER_FILE_NAME") == 0) {
        sister_name = true;
      }
      if (opt.compare("SISTER_FILE_PATH") == 0) {
        sister_path = true;
      }
      if (opt.compare("SISTER_FILE_PATH_NAME") == 0) {
        sister_name_path = true;
      }
      if (opt.compare("SISTER_FILE_EXISTS") == 0) {
        sister_exists = true;
      }
    }
    assert(allowed_file);
    assert(sister_name);
    assert(sister_path);
    assert(sister_name_path);
    assert(sister_exists);
  }

  cerr << "Testing: getPropOption" << endl;
  {
    PropertySisterFile propSisterFile1;

    vector<string> allowed_ext =
        propSisterFile1.getPropOption("ALLOWED_SISTER_FILE_EXT");
    vector<string> file_name =
        propSisterFile1.getPropOption("SISTER_FILE_NAME");
    vector<string> file_path =
        propSisterFile1.getPropOption("SISTER_FILE_PATH");
    vector<string> file_path_name =
        propSisterFile1.getPropOption("SISTER_FILE_PATH_NAME");
    vector<string> fileExist =
        propSisterFile1.getPropOption("SISTER_FILE_EXISTS");

    assert(allowed_ext.at(0).compare("NOT_DEFINED") == 0);
    assert(file_name.at(0).compare("NOT_DEFINED") == 0);
    assert(file_path.at(0).compare("NOT_DEFINED") == 0);
    assert(file_path_name.at(0).compare("NOT_DEFINED") == 0);
    assert(fileExist.at(0).compare("false") == 0);
  }

  cerr << "Testing: getPropOption" << endl;
  {
    PropertySisterFile propSisterFile1;

    propSisterFile1.setPropOption("ALLOWED_SISTER_FILE_EXT", ".pun");

    string fileName = "testfile.log";
    propSisterFile1.propValid(fileName);
    vector<string> allowed_ext =
        propSisterFile1.getPropOption("ALLOWED_SISTER_FILE_EXT");
    vector<string> file_name =
        propSisterFile1.getPropOption("SISTER_FILE_NAME");
    vector<string> file_path =
        propSisterFile1.getPropOption("SISTER_FILE_PATH");
    vector<string> file_path_name =
        propSisterFile1.getPropOption("SISTER_FILE_PATH_NAME");
    vector<string> fileExist =
        propSisterFile1.getPropOption("SISTER_FILE_EXISTS");

    assert(allowed_ext.at(0).compare(".pun") == 0);

    assert(file_name.at(0).compare("testfile.pun") == 0);
    assert(file_path.at(0).compare("") == 0);
    assert(file_path_name.at(0).compare("testfile.pun") == 0);
    assert(fileExist.at(0).compare("true") == 0);
  }
  return 0;
}
