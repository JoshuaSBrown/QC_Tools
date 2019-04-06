
#include "propertystringchoice.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertyStringChoice::PropertyStringChoice(void) {
  set<string> set_var1{"false"};
  setPropOption_("STRING_CHOICE_ENFORCED", set_var1);
  string str = "NOT_DEFINED";
  set<string> set_var2{str};
  setPropOption_("STRING_CHOICES", set_var2);
}

vector<string> PropertyStringChoice::getOpts_(void) const {
  vector<string> options;
  options.push_back("STRING_CHOICE_ENFORCED");
  options.push_back("STRING_CHOICES");
  return options;
}

void PropertyStringChoice::setPropOption(std::string option, std::string var) {

  if (option.compare("STRING_CHOICE_ENFORCED") == 0) {
    if (var.compare("false") == 0 || var.compare("true") == 0) {
      set<string> set_var{var};
      setPropOption_(option, set_var);
      return;
    } else {
      throw invalid_argument("The option " + option +
                             " can only be set to values"
                             " of true or false you have set it to " +
                             var);
    }
  } else if (option.compare("STRING_CHOICES") == 0) {
    set<string> set_var{var};
    setPropOption_(option, set_var);
  }
  throw invalid_argument("Unrecognized option value combo " + option + " " +
                         var);
}

void PropertyStringChoice::setPropOption(std::string option,
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
}

bool PropertyStringChoice::propValid(const string& string_choice) {

  set<string> opt_val = getPropOption("STRING_CHOICE_ENFORCED");
  string val = *(opt_val.begin());
  if (val.compare("true") == 0) {
    set<string> str_choices = getPropOption("STRING_CHOICES");
    if (str_choices.count(string_choice) > 0) return true;
    throw invalid_argument("The value " + string_choice +
                           " is not a valid option");
  }
  return true;
}
