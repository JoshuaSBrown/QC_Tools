
#include "propertyfileexist.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertyFileExist::PropertyFileExist(void) {
  setPropOption_(Option::MUST_EXIST, bool(false));
  setPropOption_(Option::DOES_EXIST, bool(false));
}

PropertyFileExist::PropertyFileExist(bool fileMustExist) {
  setPropOption_(Option::MUST_EXIST, fileMustExist);
}

bool PropertyFileExist::fileExist(const string& fileNamePath) const {
  struct stat buf;
  return (stat(fileNamePath.c_str(), &buf) == 0);
}

bool PropertyFileExist::propValid(const std::any& fileNamePath) {
  std::string file_name_path = any_cast<std::string>(fileNamePath);
  bool must_exist = any_cast<bool>(options_[Option::MUST_EXIST]);
  if (must_exist) {
    if (!fileExist(file_name_path)) {
      string err = "" + file_name_path + " does not exist";
      throw invalid_argument(err);
    }
    setPropOption_(Option::DOES_EXIST, bool(true));
  } else {
    if (fileExist(file_name_path)) {
      setPropOption_(Option::DOES_EXIST, bool(true));
    }
  }
  return true;
}

void PropertyFileExist::postCheck(void) const {
  auto must_exist = getPropOption<bool>(Option::MUST_EXIST);
  auto exist = getPropOption<bool>(Option::DOES_EXIST);
  if (must_exist && exist == 0) {
    throw runtime_error("A necessary file is missing!");
  }
}
