
#include "propertyfileext.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertyFileExt::PropertyFileExt(void) {
  set<string> exts_;
  exts_.insert("*");
  setPropOption_("ALLOWED_FILE_EXT", exts_);
}

PropertyFileExt::PropertyFileExt(string ext) {
  checkExt(ext);
  set<string> exts_;
  exts_.insert(ext);
  setPropOption_("ALLOWED_FILE_EXT", exts_);
}

PropertyFileExt::PropertyFileExt(set<string> exts) {
  set<string> exts_;
  for (auto ext : exts) {
    checkExt(ext);
    exts_.insert(ext);
  }
  setPropOption_("ALLOWED_FILE_EXT", exts_);
}

vector<string> PropertyFileExt::getOpts_(void) const {
  vector<string> options{"ALLOWED_FILE_EXT"};
  return options;
}

void PropertyFileExt::extSupported(const string& ext) const {
  checkExt(ext);
  auto exts_ = getPropOption("ALLOWED_FILE_EXT");
  for (auto ext_ : exts_) {
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

void PropertyFileExt::setPropOption(std::string option,
                                    std::set<std::string> vars) {
  PropertyObject::setPropOption(option, vars);
}

void PropertyFileExt::setPropOption(std::string option, std::string var) {
  set<string> vars;
  vars.insert(var);
  PropertyObject::setPropOption(option, vars);
}

bool PropertyFileExt::propValid(const string& fileNamePath) {
  string fileName = lastStringInPath(fileNamePath);
  string ext = grabStrAfterLastOccuranceInclusive(fileName, ".");
  extSupported(ext);
  return true;
}
