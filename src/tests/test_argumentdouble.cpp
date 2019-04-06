
#include "../libcatnip/io/arguments/argumentdouble.hpp"
#include <cassert>
#include <limits>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: argumentdouble" << endl;
  cerr << "Testing: constructor" << endl;
  { ArgumentDouble argDouble; }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentDouble argDouble;
    string name = "ARGUMENT_DOUBLE";
    assert(name.compare(argDouble.getArgumentName()) == 0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentDouble argDouble;
    auto props = argDouble.getProperties();

    bool double_prop = false;

    for (auto prop : props) {
      if (prop.compare("PROPERTY_DOUBLE") == 0) {
        double_prop = true;
      }
    }
    assert(double_prop);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentDouble argDouble;
    auto prop_opts = argDouble.getPropertyOptions();

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
    ArgumentDouble argDouble;
    auto prop_values = argDouble.getPropertyValues();

    bool opt_min = false;
    bool opt_max = false;
    bool opt_min_val = false;
    bool opt_max_val = false;

    for (auto val : prop_values) {
      if (val.first.compare("MIN") == 0) {
        opt_min = true;
        if (val.second.compare(to_string(numeric_limits<double>::lowest())) ==
            0) {
          opt_min_val = true;
        }
      }
      if (val.first.compare("MAX") == 0) {
        opt_max = true;
        if (val.second.compare(to_string(numeric_limits<double>::max())) == 0) {
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
