
#include "propertyfileexist.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertyFileExist::PropertyFileExist(void) {
  setPropOption_("FILE_MUST_EXIST", 0);
  setPropOption_("FILE_DOES_EXIST", 0);
}

PropertyFileExist::PropertyFileExist(int fileMustExist) {
  setPropOption_("FILE_MUST_EXIST", fileMustExist);
}

vector<string> PropertyFileExist::getOpts_(void) const {
  vector<string> options{"FILE_MUST_EXIST", "FILE_DOES_EXIST"};
  return options;
}

bool PropertyFileExist::fileExist(const string& fileNamePath) const {
  struct stat buf;
  return (stat(fileNamePath.c_str(), &buf) == 0);
}

bool PropertyFileExist::propValid(const string& fileNamePath) {
  if (getPropOption("FILE_MUST_EXIST")) {
    if (!fileExist(fileNamePath)) {
      string err = "" + fileNamePath + " does not exist";
      throw invalid_argument(err);
    }
    setPropOption_("FILE_DOES_EXIST", 1);
  } else {
    if (fileExist(fileNamePath)) {
      setPropOption_("FILE_DOES_EXIST", 1);
    }
  }
  return true;
}

void PropertyFileExist::postCheck(void) const {
  auto must_exist = getPropOption("FILE_MUST_EXIST");
  auto exist = getPropOption("FILE_DOES_EXIST");
  if (must_exist && exist == 0) {
    throw runtime_error("A necessary file is missing!");
  }
}
