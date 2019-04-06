
#include "../libcatnip/io/arguments/properties/propertystringchoice.hpp"
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {
  cerr << "Testing: PropertyStringChoice" << endl;
  cerr << "Testing: constructor" << endl;
  { PropertyStringChoice propStrChoice; }

  cerr << "Testing: getPropertyName" << endl;
  {
    PropertyStringChoice propStrChoice;
    string name = propStrChoice.getPropertyName();
    assert(name.compare("PROPERTY_STRING_CHOICE") == 0);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {

    PropertyStringChoice propStrChoice;
    auto options = propStrChoice.getPropertyOptions();

    bool choice_enforced = false;
    bool str_choices = false;

    for (auto opt : options) {
      if (opt.compare("STRING_CHOICE_ENFORCED") == 0) {
        choice_enforced = true;
      }
      if (opt.compare("STRING_CHOICES") == 0) {
        str_choices = true;
      }
    }
    assert(choice_enforced);
    assert(str_choices);
  }

  cerr << "Testing: getPropOption" << endl;
  {
    PropertyStringChoice propStrChoice;

    set<string> choice_enforced =
        propStrChoice.getPropOption("STRING_CHOICE_ENFORCED");
    set<string> choices = propStrChoice.getPropOption("STRING_CHOICES");

    string enforced = *(choice_enforced.begin());
    string choice = *(choices.begin());
    assert(enforced.compare("false") == 0);
    assert(choice.compare("NOT_DEFINED") == 0);
  }

  cerr << "Testing: getPropOption" << endl;
  {
    PropertyStringChoice propStrChoice;

    propStrChoice.setPropOption("STRING_CHOICE_ENFORCED", "true");
    set<string> choices{"true", "false"};
    propStrChoice.setPropOption("STRING_CHOICES", choices);

    string choice = "true";
    propStrChoice.propValid(choice);
    choice = "false";
    propStrChoice.propValid(choice);

    choice = "blah";
    bool throwerror = false;
    try {
      propStrChoice.propValid(choice);
    } catch (...) {
      throwerror = true;
    }
    assert(throwerror);

    set<string> allowed_choices = propStrChoice.getPropOption("STRING_CHOICES");
    set<string> choice_on =
        propStrChoice.getPropOption("STRING_CHOICE_ENFORCED");

    bool false_str = false;
    bool true_str = false;
    for (auto item : allowed_choices) {
      if (item.compare("false") == 0) false_str = true;
      if (item.compare("true") == 0) true_str = true;
    }
    assert(false_str);
    assert(true_str);

    string on = *(choice_on.begin());
    assert(on.compare("true") == 0);
  }
  return 0;
}
