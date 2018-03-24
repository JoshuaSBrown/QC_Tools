
#include <string>
#include <map>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../STRING_SUPPORT/string_support.hpp"
#include "logreader.hpp"

using namespace std;

LogReader::LogReader(string fileName) : FileReader(fileName){
  cout << "Checking File Name " << fileName_ << endl;
  validFileName_();
  cout << "Registering Sections" << endl;
  registerSections_();
}

void LogReader::registerSections_(){
  cout << "Derived Class registerSections functions" << endl;
  sectionHeaders_["AOFunction"] = "     Gross orbital populations:";
  sectionReaders_["AOFunction"] = &LogReader::AOFunctionSectionReader;
  FileReader::registerSections_();
  cout << "Size of sectionHeaders " << sectionHeaders_.size() << endl;
}

void LogReader::validFileName_(){  
  string ext = fileExt_();
  if(ext.compare(".log")!=0){
    throw invalid_argument("A log file must have the .log extension");
  }
}

void LogReader::AOFunctionSectionReader(void * ptr){
  LogReader * LR_ptr = static_cast<LogReader *>(ptr);

  cout << "Rreading AOFunction " << endl;
  int atom_num = 1;
  string end_pattern = "         Condensed to atoms";
  string line;
  getline(LR_ptr->fid_,line);
  while(getline(LR_ptr->fid_,line))
  {
    istringstream iss(line);
    string AOnum;
    iss >> AOnum;
    string elemType; 
    iss >> elemType;
    
    string orbType;
    iss >> orbType; 

    vector<double> orbVals;

    // The atom line size is a different size from the other lines
    // because it contains the element type at the begginning of the line
    // All the lines that follow after this line are are the atomic 
    // orbitals belonging to this element, until another line with the
    // element is introduced. 
    while(!iss.eof()){
      string Tot_Alpha_Beta_Spin;
      iss >> Tot_Alpha_Beta_Spin;
      orbVals.push_back(stod(Tot_Alpha_Beta_Spin));
    }
    map<string,vector<double>> orbMap;
    orbMap[orbType] = orbVals;
    LR_ptr->orb_[make_pair(atom_num,elemType)]  = orbMap;
    ++atom_num;
  
    getline(LR_ptr->fid_,line);
    elemType = line.at(9)+line.at(10);
    trim(elemType);

    while(elemType.empty()){
      istringstream iss2(line);
      iss2 >> AOnum;
      iss2 >> orbType;

      orbVals.clear();
      while(!iss2.eof()){
        string Tot_Alpha_Beta_Spin;
        iss2 >> Tot_Alpha_Beta_Spin;
        orbVals.push_back(stod(Tot_Alpha_Beta_Spin));
      }

      map<std::string,std::vector<double>> orbMap;
      orbMap[orbType] = orbVals;
      LR_ptr->orb_[make_pair(atom_num,elemType)] = orbMap;
      ++atom_num;

      getline(LR_ptr->fid_,line);
      elemType = line.at(9)+line.at(10);
      trim(elemType);

      if(foundSubStrInStr(line,end_pattern)) break;
    }

    if(foundSubStrInStr(line,end_pattern)) break;
  }

  return;
}
