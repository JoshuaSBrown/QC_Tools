
#include "propertydouble.hpp"

#include <limits>

using namespace catnip;
using namespace std;

PropertyDouble::PropertyDouble() {
  setPropOption_("MIN", numeric_limits<double>::lowest());
  setPropOption_("MAX", numeric_limits<double>::max());
}

vector<string> PropertyDouble::getOpts_(void) const {
  vector<string> options{"MIN", "MAX"};
  return options;
}

void PropertyDouble::doubleValid(const double& val) {
  if (val < getPropOption("MIN")) {
    string err = "The value is smaller than allowed " + to_string(val) +
                 " the "
                 "minimum allowed value is " +
                 to_string(getPropOption("MIN"));
    throw invalid_argument(err);
  } else if (val > getPropOption("MAX")) {
    string err = "The value is greater than allowed " + to_string(val) +
                 " the "
                 "maximum allowed value is " +
                 to_string(getPropOption("MAX"));
    throw invalid_argument(err);
  }
}
