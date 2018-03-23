
#include <sets>

#include "../STRING_SUPPORT/string_support.hpp"
#include "filereader.hpp"

using namespace std;

// Public member functions

FileReader::FileReader(string fileName){
  fileName_ = fileName;
  validFileName();
  registerSectionReaders();
}

string FileReader::getExt(){
  return fileExt_();
}

void FileReader::read(){
  open();
  string line;
  while(getline(fid,line)){
    string tag = triggerMatch(line);
    readSection(tag);
  }
  close();
}

// Private member functions

ifstream FileReader::open(){
  if(file_exist(const_cast<char*>(fileName.c_str()))!=0){
    throw invalid_argument("File "+fileName_+" does not exist.");
  }
  fid_.open(fileName);
  fileOpen_ = true;
}

void FileReader::close(){
  fid.close();
  fileOpen_ = false;
}

void FileReader::registerSections(){
  checkSections();
}

// Basically check that for a given tag both a section reader
// as well as a section header pair exist
void FileReader::checkSections(){
  set<std::string> tags;
  for( auto pr : sectionsHeaders_){
    tags.insert(pr.first);
  }
  for( auto pr : sectionReaders_){
    tags.insert(pr.first);
  }
  for( auto t : tags ){
    if(sectionReaders_.count(t)==0){
      throw runtime_error("SectionReader function is missing for tag "+tag);
    }
    if(sectionHeaders_.count(t)==0){
      throw runtime_error("SectionHeader pattern is missing for tag "+tag);
    }
  }
}

string FileReader::fileExt_(){
  string path = lastStringInPath(fileName_);
  path = trimmed(path);
  return path.substr(path.find("."));
}

string FileReader::triggerMatch(string line){
  size_t found;
  int flag = 0;
  string tag;
  for(auto sec : sections_ ){
    flag = static_cast<int>(found=line.find(sec.second));
    if(flag!=-1){
      tag = seg.first;
    }
  } 
  return tag;
}

void FileReader::readSection(string tag){
  sectionReaders_[tag].(); 
}


