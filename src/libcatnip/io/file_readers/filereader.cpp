#include "filereader.hpp"
#include "../../string_support.hpp"
#include <iostream>
#include <set>
#include <sys/stat.h>

using namespace catnip;
using namespace std;

// Public member functions

string FileReader::getExt() const { return fileExt_(); }

void FileReader::read() {
  open();
  string line;
  while (getline(fid_, line)) {
    string tag = startSection_(line);
    if (!tag.empty()) {
      readSection_(tag);
    }
    pos_ = fid_.tellg();
  }
  close();
}

// Private member functions

void FileReader::open() {
  if (!fileExist_()) {
    throw invalid_argument("File " + fileName_ + " does not exist.");
  }
  fid_.open(fileName_);
  fileOpen_ = true;
}

void FileReader::close() {
  fid_.close();
  fileOpen_ = false;
}

void FileReader::registerSections_() { checkSections_(); }

// Basically check that for a given tag both a section reader
// as well as a section header pair exist
void FileReader::checkSections_() {
  set<std::string> tags;
  for (pair<const string, set<string>> &tag_and_header : sectionHeaders_) {
    tags.insert(tag_and_header.first);
  }
  for (auto pr : sectionReaders_) {
    tags.insert(pr.first);
  }
  for (auto t : tags) {
    if (sectionReaders_.count(t) == 0) {
      throw runtime_error("SectionReader function is missing for tag " + t);
    }
    if (sectionHeaders_.count(t) == 0) {
      throw runtime_error("SectionHeader pattern is missing for tag " + t);
    }
  }
}

string FileReader::fileExt_() const {
  string path = lastStringInPath(fileName_);
  path = trimmed(path);
  auto index = path.find(".");
  if (index == string::npos) throw invalid_argument("File has no extension");
  return path.substr(index);
}

bool FileReader::fileExist_() const {
  struct stat buffer;
  return (stat(fileName_.c_str(), &buffer) == 0);
}

void FileReader::readSection_(string tag) {
  void *ptr = static_cast<void *>(this);
  sectionReaders_[tag](ptr);
}

string FileReader::startSection_(const string &line) {
  for (pair<const string, set<string>> &tag_and_header : sectionHeaders_) {
    for (const string &header : tag_and_header.second) {
      if (foundSubStrInStr(line, header)) {
        return tag_and_header.first;
      }
    }
  }
  return "";
}
