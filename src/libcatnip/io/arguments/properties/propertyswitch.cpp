
#include "propertyswitch.hpp"

#include <limits>

using namespace catnip;
using namespace std;

PropertySwitch::PropertySwitch() { setPropOption_(Option::DEFAULT, bool(false)); }

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

bool PropertySwitch::propValid(const std::any & value) {
  if( value.type() == typeid(int)){
    int int_val = any_cast<int>(value);
    switchValid(int_val); 
  }else if(value.type() == typeid(std::string)){
    std::string str_val = any_cast<std::string>(value);
    switchValid(str_val); 
  }else if(value.type() != typeid(bool)) {
    throw invalid_argument("Unrecognized type passed to switch, only recognizes"
        " int, string and bool types");
  }
  return true;
}
