
#include "propertystringchoice.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertyStringChoice::PropertyStringChoice(void) {
  set<string> set_var1{"false"};
  setPropOption_(Option::ENFORCED, set_var1);
  string str = "NOT_DEFINED";
  set<string> set_var2{str};
  setPropOption_(Option::ALLOWED_VALUES, set_var2);
}

/*void PropertyStringChoice::setPropOption( option,
                                         set<string> set_vars) {

  if (option.compare("STRING_CHOICES") == 0) {
    setPropOption_(option, set_vars);
    return;
  } else if (option.compare("STRING_CHOICE_ENFORCED") == 0) {
    if (set_vars.size() > 1) {
      string err = "";
      for (auto var : set_vars) {
        err.append(var);
        err.append(" ");
      }
      trim(err);
      throw invalid_argument(
          "Option STRING_CHOICE_ENFORCED only accepts one "
          "value either true or false you have passed the following values " +
          err);
    }

    string var = *(set_vars.begin());
    if (var.compare("false") == 0 || var.compare("true") == 0) {
      setPropOption_(option, set_vars);
      return;
    } else {
      throw invalid_argument("The option " + option +
                             " can only be set to values"
                             " of true or false you have set it to " +
                             var);
    }
  }
  string vars = "";
  for (auto var : set_vars) {
    vars.append(var);
    vars.append(" ");
  }
  trim(vars);
  throw invalid_argument("Unrecognized option value combo " + option + " " +
                         vars);
}*/

bool PropertyStringChoice::propValid(const any& choice) {

  std::string string_choice = any_cast<std::string>(choice);
  set<string> opt_val = getPropOption<set<string>>(Option::ENFORCED);
  string val = *(opt_val.begin());
  if (val.compare("true") == 0) {
    set<string> str_choices = getPropOption<set<string>>(Option::ALLOWED_VALUES);
    if (str_choices.count(string_choice) > 0) return true;
    throw invalid_argument("The value " + string_choice +
                           " is not a valid option");
  }
  return true;
}
