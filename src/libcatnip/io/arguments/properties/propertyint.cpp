
#include "propertyint.hpp"

#include <limits>

using namespace catnip;
using namespace std;

PropertyInt::PropertyInt() {
  setPropOption_("MIN", numeric_limits<int>::min());
  setPropOption_("MAX", numeric_limits<int>::max());
}

vector<string> PropertyInt::getOpts_(void) const {
  vector<string> options{"MIN", "MAX"};
  return options;
}

void PropertyInt::intValid(const int& val) const {
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
