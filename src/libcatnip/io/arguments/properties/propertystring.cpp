
#include "propertystring.hpp"
#include <memory>

using namespace catnip;
using namespace std;

PropertyString::PropertyString(void) {
  setPropOption_("MIN_LENGTH", 0);
  setPropOption_("MAX_LENGTH", (size_t)-1);
}

vector<string> PropertyString::getOpts_(void) const {
  vector<string> options{"MIN_LENGTH", "MAX_LENGTH"};
  return options;
}

void PropertyString::stringValid(const string& val) const {
  if (val.size() > getPropOption("MAX_LENGTH")) {
    string err = "The string is larger than allowed " + val +
                 " the "
                 "maximum allowed value is " +
                 to_string(getPropOption("MAX_LENGTH"));
    throw invalid_argument(err);
  } else if (val.size() < getPropOption("MIN_LENGTH")) {
    string err = "The string is smaller than allowed " + val +
                 " the "
                 "maximum allowed value is " +
                 to_string(getPropOption("MIN_LENGTH"));
    throw invalid_argument(err);
  }
}
