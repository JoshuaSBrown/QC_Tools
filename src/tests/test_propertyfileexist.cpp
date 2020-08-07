
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "io/arguments/properties/propertyfileexist.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

TEST_CASE("Property File Exists","[unit]") {

  cerr << "Testing: PropertyFileExist" << endl;
  cerr << "Testing: constructor" << endl;
  {
    PropertyFileExist propFileExist1(true);
    PropertyFileExist propFileExist2(false);
  }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyFileExist propFileExist;
    PropertyType type = propFileExist.getPropertyType();
    CHECK(type == PropertyTYpe::FILE_EXIST);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyFileExist propFileExist;
    auto options = propFileExist.getPropertyOptions();
    Option opt = options.at(0);
    CHECK(opt == Option::MUST_EXIST);
  }

  cerr << "Testing: propValid" << endl;
  {
    PropertyFileExist propFileExist1(false);
    PropertyFileExist propFileExist2(true);
    PropertyFileExist propFileExist3(true);

    bool valid = propFileExist1.propValid("test_propertyfileexist.cpp");
    CHECK(valid);

    CHECK_THROWS(propFileExist2.propValid("fake"));

    valid = propFileExist3.propValid("testfile.pun");
    CHECK(valid);
  }

  cerr << "Testing: getPropOption" << endl;
  {
    PropertyFileExist propFileExist1(true);
    PropertyFileExist propFileExist2(false);

    bool fileExist = propFileExist1.getPropOption(Option::MUST_EXIST);
    CHECK(fileExist);

    fileExist = propFileExist2.getPropOption(Option::MUST_EXIST);
    CHECK(fileExist == false);
  }

}
