
#include "propertysisterfile.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <set>
#include <vector>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertySisterFile::PropertySisterFile(void) {
  string var = "NOT_DEFINED";
  vector<string> vec_var{var};
  setPropOption_(Option::ALLOWED_VALUES, vec_var);
  setPropOption_(Option::FILE_NAME, vec_var);
  setPropOption_(Option::FILE_PATH, vec_var);
  setPropOption_(Option::FILE_PATH_NAME, vec_var);
  vector<string> vec_var2{"false"};
  setPropOption_(Option::DOES_EXIST, vec_var2);
}

bool PropertySisterFile::fileExist(const string& fileNamePath) const {
  struct stat buf;
  return (stat(fileNamePath.c_str(), &buf) == 0);
}

void PropertySisterFile::extSupported(const string& ext) const {
  checkExt(ext);
  auto exts_ = getPropOption<std::set<std::string>>(Option::ALLOWED_VALUES);
  for (auto ext_ : exts_) {
    if (ext_.compare(ext) == 0) {
      return;
    }
  }
  string err = "The file ext " + ext + " is unsupported";
  throw invalid_argument(err);
}

void PropertySisterFile::checkExt(const string& ext) const {
  string excess = grabStrBeforeLastOccurance(ext, ".");
  if (excess.compare("") != 0) {
    throw invalid_argument(
        "An extension must be of type '.ext'"
        "you have used " +
        ext);
  }
  return;
}

bool PropertySisterFile::propValid(const std::any & name_and_path) {

  std::string fileNamePath = any_cast<std::string>(name_and_path); 

  string fileName = lastStringInPath(fileNamePath);
  string path = getPath(fileNamePath);

  string sisterFileCore = grabStrBeforeLastOccurance(fileName, ".");
  auto sister_exts = getPropOption<std::set<std::string>>(Option::ALLOWED_VALUES);

  vector<string> fileNames;
  vector<string> filePaths;
  vector<string> filePathNames;
  vector<string> fileExists;

  for (auto sister_ext : sister_exts) {

    if (sister_ext.compare("NOT_DEFINED") != 0) {
      string sisterFileName = sisterFileCore + sister_ext;
      string sisterPath = path + sisterFileName;

      fileNames.push_back(sisterFileName);
      filePaths.push_back(path);
      filePathNames.push_back(sisterPath);
      if (fileExist(sisterPath)) {
        fileExists.push_back("true");
      } else {
        fileExists.push_back("false");
      }
    }
  }

  setPropOption_(Option::FILE_NAME, fileNames);
  setPropOption_(Option::FILE_PATH, filePaths);
  setPropOption_(Option::FILE_PATH_NAME, filePathNames);
  setPropOption_(Option::DOES_EXIST, fileExists);
  return true;
}
