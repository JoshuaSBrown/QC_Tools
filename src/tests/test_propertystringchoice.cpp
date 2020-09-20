
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "io/arguments/properties/propertystringchoice.hpp"
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

TEST_CASE("Property String Choice","[unit]") {
  { PropertyStringChoice propStrChoice; }
}

TEST_CASE("getPropertyType","[unit]") { 
  PropertyStringChoice propStrChoice;
  PropertyType type = propStrChoice.getPropertyType();
  REQUIRE(type == PropertyType::STRING_CHOICE);
}

TEST_CASE("getPropertyOptions","[unit]") { 

  PropertyStringChoice propStrChoice;
  auto options = propStrChoice.getPropertyOptions();

  bool choice_enforced = false;
  bool str_choices = false;

  for (auto opt : options) {
    if (opt == Option::ENFORCED) {
      choice_enforced = true;
    }
    if (opt == Option::ALLOWED_VALUES) {
      str_choices = true;
    }
  }
  REQUIRE(choice_enforced);
  REQUIRE(str_choices);
}

TEST_CASE("getPropOption","[unit]"){ 
  PropertyStringChoice propStrChoice;

  set<bool> choice_enforced =
    propStrChoice.getPropOption<set<bool>>(Option::ENFORCED);
  set<string> choices = propStrChoice.getPropOption<set<string>>(Option::ALLOWED_VALUES);
/*
  bool enforced = *(choice_enforced.begin());
  string choice = *(choices.begin());
  REQUIRE(enforced == false);
  REQUIRE(choice == "NOT_DEFINED");*/
}

TEST_CASE("setPropOption", "[unit]"){
  PropertyStringChoice propStrChoice;

  propStrChoice.setPropOption(Option::ENFORCED, "true");
  set<string> choices{"true", "false"};
  propStrChoice.setPropOption(Option::ALLOWED_VALUES, choices);

  string choice = "true";
  propStrChoice.propValid(choice);
  choice = "false";
  propStrChoice.propValid(choice);

  choice = "blah";
  REQUIRE_THROWS(propStrChoice.propValid(choice));

  set<string> allowed_choices = propStrChoice.getPropOption<set<string>>(Option::ALLOWED_VALUES);
  set<string> choice_on =
    propStrChoice.getPropOption<set<string>>(Option::ALLOWED_VALUES);

  bool false_str = false;
  bool true_str = false;
  for (auto item : allowed_choices) {
    if (item.compare("false") == 0) false_str = true;
    if (item.compare("true") == 0) true_str = true;
  }
  REQUIRE(false_str);
  REQUIRE(true_str);

  string on = *(choice_on.begin());
  REQUIRE(on.compare("true") == 0);
}
