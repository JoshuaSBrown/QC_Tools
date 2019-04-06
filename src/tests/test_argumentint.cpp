
#include "../libcatnip/io/arguments/argumentint.hpp"
#include <cassert>
#include <limits>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: argumentint" << endl;
  cerr << "Testing: constructor" << endl;
  { ArgumentInt argInt; }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentInt argInt;
    string name = "ARGUMENT_INT";
    assert(name.compare(argInt.getArgumentName()) == 0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentInt argInt;
    auto props = argInt.getProperties();

    bool int_prop = false;

    for (auto prop : props) {
      if (prop.compare("PROPERTY_INT") == 0) {
        int_prop = true;
      }
    }
    assert(int_prop);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentInt argInt;
    auto prop_opts = argInt.getPropertyOptions();

    bool opt_min = false;
    bool opt_max = false;

    for (auto opt : prop_opts) {
      if (opt.compare("MIN") == 0) {
        opt_min = true;
      }
      if (opt.compare("MAX") == 0) {
        opt_max = true;
      }
    }

    assert(opt_min);
    assert(opt_max);
  }

  cerr << "Testing: getArgPropertyValues" << endl;
  {
    ArgumentInt argInt;
    auto prop_values = argInt.getPropertyValues();

    bool opt_min = false;
    bool opt_max = false;
    bool opt_min_val = false;
    bool opt_max_val = false;

    for (auto val : prop_values) {
      if (val.first.compare("MIN") == 0) {
        opt_min = true;
        if (val.second.compare(to_string(numeric_limits<int>::min())) == 0) {
          opt_min_val = true;
        }
      }
      if (val.first.compare("MAX") == 0) {
        opt_max = true;
        if (val.second.compare(to_string(numeric_limits<int>::max())) == 0) {
          opt_max_val = true;
        }
      }
    }

    assert(opt_min);
    assert(opt_max);
    assert(opt_min_val);
    assert(opt_max_val);
  }

  return 0;
}
