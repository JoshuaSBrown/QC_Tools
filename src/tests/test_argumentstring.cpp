
#include "../libcatnip/io/arguments/argumentstring.hpp"
#include <cassert>
#include <limits>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: argumentstring" << endl;
  cerr << "Testing: constructor" << endl;
  { ArgumentString argString; }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentString argString;
    string name = "ARGUMENT_STRING";
    assert(name.compare(argString.getArgumentName()) == 0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentString argString;
    auto props = argString.getProperties();

    bool string_prop = false;
    bool string_choice = false;

    for (auto prop : props) {
      if (prop.compare("PROPERTY_STRING") == 0) {
        string_prop = true;
      }
      if (prop.compare("PROPERTY_STRING_CHOICE") == 0) {
        string_choice = true;
      }
    }
    assert(string_prop);
    assert(string_choice);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentString argString;
    auto prop_opts = argString.getPropertyOptions();

    bool opt_min = false;
    bool opt_max = false;
    bool opt_enforced = false;
    bool opt_choices = false;

    for (auto opt : prop_opts) {
      if (opt.compare("MIN_LENGTH") == 0) {
        opt_min = true;
      }
      if (opt.compare("MAX_LENGTH") == 0) {
        opt_max = true;
      }
      if (opt.compare("STRING_CHOICE_ENFORCED") == 0) {
        opt_enforced = true;
      }
      if (opt.compare("STRING_CHOICES") == 0) {
        opt_choices = true;
      }
    }

    assert(opt_min);
    assert(opt_max);
    assert(opt_enforced);
    assert(opt_choices);
  }

  cerr << "Testing: getArgPropertyValues" << endl;
  {
    ArgumentString argString;
    auto prop_values = argString.getPropertyValues();

    bool opt_min = false;
    bool opt_max = false;
    bool opt_enforced = false;
    bool opt_choices = false;

    bool opt_min_val = false;
    bool opt_max_val = false;
    bool opt_enforced_val = false;
    bool opt_choices_val = false;

    for (auto val : prop_values) {
      if (val.first.compare("MIN_LENGTH") == 0) {
        opt_min = true;
        if (val.second.compare(to_string(0)) == 0) {
          opt_min_val = true;
        }
      }
      if (val.first.compare("MAX_LENGTH") == 0) {
        opt_max = true;
        if (val.second.compare(to_string((size_t)-1)) == 0) {
          opt_max_val = true;
        }
      }
      if (val.first.compare("STRING_CHOICE_ENFORCED") == 0) {
        opt_enforced = true;
        if (val.second.compare("false") == 0) {
          opt_enforced_val = true;
        }
      }
      if (val.first.compare("STRING_CHOICES") == 0) {
        opt_choices = true;
        if (val.second.compare("NOT_DEFINED") == 0) {
          opt_choices_val = true;
        }
      }
    }

    assert(opt_min);
    assert(opt_max);
    assert(opt_min_val);
    assert(opt_max_val);
    assert(opt_enforced);
    assert(opt_choices);
    assert(opt_enforced_val);
    assert(opt_choices_val);
  }

  return 0;
}
