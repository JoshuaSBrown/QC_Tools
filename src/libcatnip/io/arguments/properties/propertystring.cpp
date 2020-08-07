#include "propertyobject.hpp"
#include "propertystring.hpp"
#include <memory>

using namespace catnip;
using namespace std;

PropertyString::PropertyString(void) {
  size_t min_len = 0;
  size_t max_len = -1;
  setPropOption_(Option::MIN_LENGTH, min_len);
  setPropOption_(Option::MAX_LENGTH, max_len);
}

void PropertyString::stringValid(const string& val) const {
  if (val.size() > getPropOption<size_t>(Option::MAX_LENGTH)) {
    string err = "The string is larger than allowed " + val +
                 " the "
                 "maximum allowed value is " +
                 to_string(getPropOption<size_t>(Option::MAX_LENGTH));
    throw invalid_argument(err);
  } else if (val.size() < getPropOption<size_t>(Option::MIN_LENGTH)) {
    string err = "The string is smaller than allowed " + val +
                 " the "
                 "maximum allowed value is " +
                 to_string(getPropOption<size_t>(Option::MIN_LENGTH));
    throw invalid_argument(err);
  }
}
