
#include "propertyswitch.hpp"

#include <limits>

using namespace catnip;
using namespace std;

PropertySwitch::PropertySwitch() { setPropOption_("DEFAULT", "OFF"); }

vector<string> PropertySwitch::getOpts_(void) const {
  vector<string> options{"DEFAULT"};
  return options;
}

void PropertySwitch::switchValid(int val) const {
  if (val < 0 || val > 1) {
    string err =
        "Switches are only allowed to be on or off. By default they "
        "are set to off. An integer may be provided to the switch to "
        "toggle the behavior, however the integer is only allowed to "
        "be a 1 or a 0 you have provided a " +
        to_string(val);
    throw invalid_argument(err);
  }
}

void PropertySwitch::switchValid(std::string val) const {
  if (val.compare("0") != 0 && val.compare("1") != 0 &&
      val.compare("ON") != 0 && val.compare("OFF") != 0 &&
      val.compare("TRUE") != 0 && val.compare("FALSE") != 0) {
    string err =
        "Switches are only allowed to be on or off. By default they "
        "are set to off. A string may be provided to the switch to "
        "toggle the behavior, however the string is only allowed to "
        "be a TRUE/FALSE/ON/OFF you have provided a " +
        val;
    throw invalid_argument(err);
  }
}
