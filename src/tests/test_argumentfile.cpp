
#include "../libcatnip/io/arguments/argumentfile.hpp"
#include <cassert>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: argumentfile" << endl;
  cerr << "Testing: constructor" << endl;
  { ArgumentFile argFile; }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentFile argFile;
    string name = "ARGUMENT_FILE";
    assert(name.compare(argFile.getArgumentName()) == 0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentFile argFile;
    auto props = argFile.getProperties();

    bool file_exist = false;
    bool file_ext = false;
    bool sister_file = false;

    for (auto prop : props) {
      if (prop.compare("PROPERTY_FILE_EXIST") == 0) {
        file_exist = true;
      }
      if (prop.compare("PROPERTY_FILE_EXT") == 0) {
        file_ext = true;
      }
      if (prop.compare("PROPERTY_SISTER_FILE") == 0) {
        sister_file = true;
      }
    }
    assert(file_exist);
    assert(file_ext);
    assert(sister_file);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentFile argFile;
    auto prop_opts = argFile.getPropertyOptions();

    bool file_must_exist = false;
    bool sis_file_name = false;
    bool sis_file_path = false;
    bool sis_file_path_name = false;
    bool sis_file_exists = false;
    bool allowed_ext = false;
    bool allowed_sister_ext = false;
    for (auto opt : prop_opts) {
      if (opt.compare("ALLOWED_FILE_EXT") == 0) {
        allowed_ext = true;
      }
      if (opt.compare("ALLOWED_SISTER_FILE_EXT") == 0) {
        allowed_sister_ext = true;
      }
      if (opt.compare("FILE_MUST_EXIST") == 0) {
        file_must_exist = true;
      }
      if (opt.compare("SISTER_FILE_NAME") == 0) {
        sis_file_name = true;
      }
      if (opt.compare("SISTER_FILE_PATH") == 0) {
        sis_file_path = true;
      }
      if (opt.compare("SISTER_FILE_PATH_NAME") == 0) {
        sis_file_path_name = true;
      }
      if (opt.compare("SISTER_FILE_EXISTS") == 0) {
        sis_file_exists = true;
      }
    }

    assert(file_must_exist);
    assert(allowed_ext);
    assert(allowed_sister_ext);
    assert(sis_file_name);
    assert(sis_file_path);
    assert(sis_file_path_name);
    assert(sis_file_exists);
  }

  cerr << "Testing: getArgPropertyValues" << endl;
  {
    ArgumentFile argFile;
    auto prop_values = argFile.getPropertyValues();

    bool file_must_exist = false;
    bool allowed_ext = false;
    bool file_must_exist_val = false;
    bool allowed_ext_val = false;

    for (auto val : prop_values) {
      if (val.first.compare("ALLOWED_FILE_EXT") == 0) {
        allowed_ext = true;
        if (val.second[0] == '*') {
          allowed_ext_val = true;
        }
      }
      if (val.first.compare("FILE_MUST_EXIST") == 0) {
        file_must_exist = true;
        if (val.second.compare("0") == 0) {
          file_must_exist_val = true;
        }
      }
    }

    assert(file_must_exist);
    assert(file_must_exist_val);

    assert(allowed_ext);
    assert(allowed_ext_val);
  }

  cerr << "Testing: setArgPropertyValues" << endl;
  {
    ArgumentFile argFile;
    auto prop_values = argFile.getPropertyValues();

    bool file_must_exist = false;
    bool allowed_ext = false;
    bool file_must_exist_val = false;
    bool allowed_ext_val = false;

    for (auto val : prop_values) {
      if (val.first.compare("ALLOWED_FILE_EXT") == 0) {
        allowed_ext = true;
        if (val.second[0] == '*') {
          allowed_ext_val = true;
        }
      }
      if (val.first.compare("FILE_MUST_EXIST") == 0) {
        file_must_exist = true;
        if (val.second.compare("0") == 0) {
          file_must_exist_val = true;
        }
      }
    }

    assert(file_must_exist);
    assert(file_must_exist_val);

    assert(allowed_ext);
    assert(allowed_ext_val);

    set<string> exts{".pun", ".7", ".orb"};
    argFile.setArgPropertyOpt("PROPERTY_FILE_EXT", "ALLOWED_FILE_EXT", exts);

    auto str_exts =
        argFile.getPropertyValues("PROPERTY_FILE_EXT", "ALLOWED_FILE_EXT");
    cerr << "extensions " << str_exts << endl;
  }

  return 0;
}
