
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "io/arguments/properties/propertyfileext.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

TEST_CASE("Property File Extension","[unit]") {

  cerr << "Testing: PropertyFileExt" << endl;
  cerr << "Testing: constructor" << endl;
  {
    PropertyFileExt propFileExt1("*");
    PropertyFileExt propFileExt2("");
    PropertyFileExt propFileExt3(".jpg");
    CHECK_THROWS(PropertyFileExt propFileExt4("ffda.fd"));
    set<string> exts = {".png", ".gjf"};
    PropertyFileExt propFileExt5(exts);
  }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyFileExt propFileExt;
    PropertyType type = propFileExt.getPropertyType();
    REQUIRE(type == PropertyType::FILE_EXT);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyFileExt propFileExt;
    auto options = propFileExt.getPropertyOptions();
    Option opt = options.at(0);
    REQUIRE(opt == Option::ALLOWED_VALUES);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyFileExt propFileExt(".jpg");
    bool valid = propFileExt.propValid("dir/file.jpg");
    CHECK(valid);
    CHECK_THROWS(propFileExt.propValid("dir/file.jp"));

    PropertyFileExt propFileExt2("*");
    valid = propFileExt2.propValid("dir/file.jpg");
    CHECK(valid);
    valid = propFileExt2.propValid("dir/file.jp");
    CHECK(valid);

    set<string> exts = {".png", ".gjf"};
    PropertyFileExt propFileExt3(exts);
    valid = propFileExt3.propValid("Dir2/Path/File.png");
    CHECK(valid);
    valid = propFileExt3.propValid("Dir2/Path/File.gjf");
    CHECK(valid);
    CHECK_THROWS(propFileExt3.propValid("dir/file.com"));
  }
}
