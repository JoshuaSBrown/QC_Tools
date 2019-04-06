
#include "../libcatnip/io/arguments/properties/propertyfileext.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PropertyFileExt" << endl;
  cerr << "Testing: constructor" << endl;
  {
    PropertyFileExt propFileExt1("*");
    PropertyFileExt propFileExt2("");
    PropertyFileExt propFileExt3(".jpg");
    bool excep = false;
    try {
      PropertyFileExt propFileExt4("ffda.fd");
    } catch (...) {
      excep = true;
    }
    assert(excep);

    set<string> exts = {".png", ".gjf"};
    PropertyFileExt propFileExt5(exts);
  }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyFileExt propFileExt;
    string name = propFileExt.getPropertyName();
    assert(name.compare("PROPERTY_FILE_EXT") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyFileExt propFileExt;
    auto options = propFileExt.getPropertyOptions();
    string opt = options.at(0);
    assert(opt.compare("ALLOWED_FILE_EXT") == 0);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyFileExt propFileExt(".jpg");
    bool valid = propFileExt.propValid("dir/file.jpg");
    assert(valid);
    bool excep = false;
    try {
      propFileExt.propValid("dir/file.jp");
    } catch (...) {
      excep = true;
    }
    assert(excep);

    PropertyFileExt propFileExt2("*");
    valid = propFileExt2.propValid("dir/file.jpg");
    assert(valid);
    valid = propFileExt2.propValid("dir/file.jp");
    assert(valid);

    set<string> exts = {".png", ".gjf"};
    PropertyFileExt propFileExt3(exts);
    valid = propFileExt3.propValid("Dir2/Path/File.png");
    assert(valid);
    valid = propFileExt3.propValid("Dir2/Path/File.gjf");
    assert(valid);
    excep = false;
    try {
      propFileExt3.propValid("dir/file.com");
    } catch (...) {
      excep = true;
    }
    assert(excep);
  }
  return 0;
}
