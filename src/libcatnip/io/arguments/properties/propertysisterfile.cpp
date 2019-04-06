
#include "propertysisterfile.hpp"
#include "../../../string_support.hpp"
#include <stdexcept>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

PropertySisterFile::PropertySisterFile(void) {
  string var = "NOT_DEFINED";
  vector<string> vec_var{var};
  setPropOption_("ALLOWED_SISTER_FILE_EXT", vec_var);
  setPropOption_("SISTER_FILE_NAME", vec_var);
  setPropOption_("SISTER_FILE_PATH", vec_var);
  setPropOption_("SISTER_FILE_PATH_NAME", vec_var);
  vector<string> vec_var2{"false"};
  setPropOption_("SISTER_FILE_EXISTS", vec_var2);
}

vector<string> PropertySisterFile::getOpts_(void) const {
  vector<string> options;
  options.push_back("ALLOWED_SISTER_FILE_EXT");
  options.push_back("SISTER_FILE_NAME");
  options.push_back("SISTER_FILE_PATH");
  options.push_back("SISTER_FILE_PATH_NAME");
  options.push_back("SISTER_FILE_EXISTS");
  return options;
}

bool PropertySisterFile::fileExist(const string& fileNamePath) const {
  struct stat buf;
  return (stat(fileNamePath.c_str(), &buf) == 0);
}

void PropertySisterFile::extSupported(const string& ext) const {
  checkExt(ext);
  auto exts_ = getPropOption("ALLOWED_SISTER_FILE_EXT");
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

void PropertySisterFile::setPropOption(std::string option,
                                       const std::string& var) {

  if (option.compare("ALLOWED_SISTER_FILE_EXT") == 0) {
    checkExt(var);
    vector<string> vec_var{var};
    setPropOption_(option, vec_var);
    return;
  }
  if (option.compare("SISTER_FILE_EXISTS") == 0 ||
      option.compare("SISTER_FILE_NAME") == 0 ||
      option.compare("SISTER_FILE_PATH") == 0 ||
      option.compare("SISTER_FILE_PATH_NAME") == 0) {
    throw invalid_argument(
        "This option is determined internally you do not have permission to "
        "change it");
  }
  throw invalid_argument("Unrecognized option value combo " + option + " " +
                         var);
}

void PropertySisterFile::setPropOption(std::string option,
                                       vector<string> vec_vars) {

  if (option.compare("ALLOWED_SISTER_FILE_EXT") == 0) {
    vector<string> fileNames;
    vector<string> filePaths;
    vector<string> filePathNames;
    vector<string> fileExists;
    string notdef = "NOT_DEFINED";

    for (auto var : vec_vars) {
      checkExt(var);
      fileNames.push_back(notdef);
      filePaths.push_back(notdef);
      filePathNames.push_back(notdef);
      fileExists.push_back("false");
    }
    setPropOption_(option, vec_vars);
    return;
  }
  if (option.compare("SISTER_FILE_EXISTS") == 0 ||
      option.compare("SISTER_FILE_NAME") == 0 ||
      option.compare("SISTER_FILE_PATH") == 0 ||
      option.compare("SISTER_FILE_PATH_NAME") == 0) {
    throw invalid_argument(
        "This option is determined internally you do not have permission to "
        "change it");
  }

  string vars = "";
  for (auto var : vec_vars) {
    vars.append(var);
    vars.append(" ");
  }
  trim(vars);
  throw invalid_argument("Unrecognized option value combo " + option + " " +
                         vars);
}

bool PropertySisterFile::propValid(const string& fileNamePath) {

  string fileName = lastStringInPath(fileNamePath);
  string path = getPath(fileNamePath);

  string sisterFileCore = grabStrBeforeLastOccurance(fileName, ".");
  auto sister_exts = getPropOption("ALLOWED_SISTER_FILE_EXT");

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
  setPropOption_("SISTER_FILE_NAME", fileNames);
  setPropOption_("SISTER_FILE_PATH", filePaths);
  setPropOption_("SISTER_FILE_PATH_NAME", filePathNames);
  setPropOption_("SISTER_FILE_EXISTS", fileExists);
  return true;
}
