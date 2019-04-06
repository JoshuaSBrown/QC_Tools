
#include "../libcatnip/io/arguments/properties/propertyfileexist.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PropertyFileExist" << endl;
  cerr << "Testing: constructor" << endl;
  {
    PropertyFileExist propFileExist1(true);
    PropertyFileExist propFileExist2(false);
  }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyFileExist propFileExist;
    string name = propFileExist.getPropertyName();
    assert(name.compare("PROPERTY_FILE_EXIST") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyFileExist propFileExist;
    auto options = propFileExist.getPropertyOptions();
    string opt = options.at(0);
    assert(opt.compare("FILE_MUST_EXIST") == 0);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyFileExist propFileExist1(false);
    PropertyFileExist propFileExist2(true);
    PropertyFileExist propFileExist3(true);

    bool valid = propFileExist1.propValid("test_propertyfileexist.cpp");
    assert(valid);

    bool excep = false;
    try {
      valid = propFileExist2.propValid("fake");
    } catch (...) {
      excep = true;
    }
    assert(excep);

    valid = propFileExist3.propValid("testfile.pun");
    assert(valid);
  }

  cerr << "Testing: getPropOption" << endl;
  {
    PropertyFileExist propFileExist1(true);
    PropertyFileExist propFileExist2(false);

    bool fileExist = propFileExist1.getPropOption("FILE_MUST_EXIST");
    assert(fileExist);

    fileExist = propFileExist2.getPropOption("FILE_MUST_EXIST");
    assert(fileExist == false);
  }

  return 0;
}
