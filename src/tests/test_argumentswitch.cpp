
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../libcatnip/io/arguments/argumentswitch.hpp"
#include <cassert>
#include <limits>

using namespace catnip;
using namespace std;

TEST_CASE("Argument Switch","[unit]") {

  cerr << "Testing: argumentswitch" << endl;
  cerr << "Testing: constructor" << endl;
  { ArgumentSwitch argSwitch; }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentSwitch argSwitch;
    string name = "ARGUMENT_SWITCH";
    assert(name.compare(argSwitch.getArgumentName()) == 0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentSwitch argSwitch;
    auto props = argSwitch.getProperties();

    bool int_prop = false;

    for (auto prop : props) {
      if (prop.compare("PROPERTY_SWITCH") == 0) {
        int_prop = true;
      }
    }
    assert(int_prop);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentSwitch argSwitch;
    auto prop_opts = argSwitch.getPropertyOptions();

    bool opt_def = false;

    for (auto opt : prop_opts) {
      if (opt.compare("DEFAULT") == 0) {
        opt_def = true;
      }
    }

    assert(opt_def);
  }

  cerr << "Testing: getArgPropertyValues" << endl;
  {
    ArgumentSwitch argSwitch;
    auto prop_values = argSwitch.getPropertyValues();

    bool opt_def = false;
    bool opt_def_val = false;

    for (auto val : prop_values) {
      if (val.first.compare("DEFAULT") == 0) {
        opt_def = true;
        if (val.second.compare("OFF") == 0) {
          opt_def_val = true;
        }
      }
    }

    assert(opt_def);
    assert(opt_def_val);
  }

}
