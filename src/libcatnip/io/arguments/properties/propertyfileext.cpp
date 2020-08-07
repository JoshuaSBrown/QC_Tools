
#include "propertyfileext.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertyFileExt::PropertyFileExt(string ext) {
  checkExt(ext);
  setPropOption_(Option::ALLOWED_VALUES, ext);
}

PropertyFileExt::PropertyFileExt(set<string> exts) {
  for (const std::string & ext : exts) {
    checkExt(ext);
  }
  setPropOption_(Option::ALLOWED_VALUES, exts);
}

void PropertyFileExt::extSupported(const string& ext) const {
  checkExt(ext);
  auto exts_ = getPropOption<std::set<std::string>>(Option::ALLOWED_VALUES);
  for (const std::string & ext_ : exts_) {
    if (ext_[0] == '*') {
      return;
    } else if (ext_.compare(ext) == 0) {
      return;
    }
  }
  string err = "The file ext " + ext + " is unsupported";
  throw invalid_argument(err);
}

void PropertyFileExt::checkExt(const string& ext) const {
  if (ext.compare("") == 0) {
    return;
  } else if (ext[0] == '*') {
    return;
  } else {
    string excess = grabStrBeforeLastOccurance(ext, ".");
    if (excess.compare("") != 0) {
      throw invalid_argument(
          "An extension must be of type '.ext','*','', "
          "you have used " +
          ext);
    }
  }
  return;
}

void PropertyFileExt::setPropOption(Option option,
                                    std::set<std::string> vars) {
  PropertyObject::setPropOption(option, vars);
}

void PropertyFileExt::setPropOption(Option option, std::string var) {
  set<string> vars;
  vars.insert(var);
  PropertyObject::setPropOption(option, vars);
}

bool PropertyFileExt::propValid(const any & fileNamePath) {
  string fileName = lastStringInPath(any_cast<std::string>(fileNamePath));
  string ext = grabStrAfterLastOccuranceInclusive(fileName, ".");
  extSupported(ext);
  return true;
}
