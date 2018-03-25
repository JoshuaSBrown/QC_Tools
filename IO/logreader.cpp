
#include <string>
#include <map>
#include <cmath>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../STRING_SUPPORT/string_support.hpp"
#include "logreader.hpp"

using namespace std;

LogReader::LogReader(string fileName) : FileReader(fileName){
  validFileName_();
  registerSections_();
}

void LogReader::registerSections_(){
  sectionHeaders_["AOFunction"] = "     Gross orbital populations:";
  sectionReaders_["AOFunction"] = &LogReader::AOFunctionSectionReader;
  sectionHeaders_["Overlap"] = " *** Overlap ***";
  sectionReaders_["Overlap"] = &LogReader::OverlapSectionReader;
  FileReader::registerSections_();
}

void LogReader::validFileName_(){  
  string ext = fileExt_();
  if(ext.compare(".log")!=0){
    throw invalid_argument("A log file must have the .log extension");
  }
}

void LogReader::AOFunctionSectionReader(void * ptr){
  LogReader * LR_ptr = static_cast<LogReader *>(ptr);

  int atom_num = 1;
  string end_pattern = "         Condensed to atoms";
  string line;
  getline(LR_ptr->fid_,line);
  getline(LR_ptr->fid_,line);
  bool sectionEnd = false;
  while(sectionEnd==false)
  {
    istringstream iss(line);
    string AOnum;
    iss >> AOnum;
    string elemType; 
    iss >> elemType;
    iss >> elemType;
 
    string orbType;
    iss >> orbType; 
    vector<double> orbVals;
    while(!iss.eof()){
      string Tot_Alpha_Beta_Spin;
      iss >> Tot_Alpha_Beta_Spin;
      orbVals.push_back(stod(Tot_Alpha_Beta_Spin));
    }
    LR_ptr->orb_[make_pair(atom_num,elemType)][orbType]  = orbVals;
  
    getline(LR_ptr->fid_,line);
    string elemType2 = "";
    elemType2.append(string(1,line.at(9)));
    elemType2.append(string(1,line.at(10)));
    trim(elemType2);
   
    while(!isAlphabetical(elemType2)){
      istringstream iss2(line);
      iss2 >> AOnum;
      iss2 >> orbType;
    
      orbVals.clear();
      while(!iss2.eof()){
        string Tot_Alpha_Beta_Spin;
        iss2 >> Tot_Alpha_Beta_Spin;
        orbVals.push_back(stod(Tot_Alpha_Beta_Spin));
      }

      LR_ptr->orb_[make_pair(atom_num,elemType)][orbType] = orbVals;

      getline(LR_ptr->fid_,line);

      elemType2 = "";
      elemType2.append(string(1,line.at(9)));
      elemType2.append(string(1,line.at(10)));
      trim(elemType2);

      if(foundSubStrInStr(line,end_pattern)){
        sectionEnd = true;
        break;
      }
    }

    ++atom_num;
    //getline(LR_ptr->fid_,line);
    if(foundSubStrInStr(line,end_pattern)){
      sectionEnd = true;
    }
  }

  return;
}

void LogReader::OverlapSectionReader(void * ptr){
  LogReader * LR_ptr = static_cast<LogReader *>(ptr);

  // Phase one read in the first group of coordinates and count
  // to determien the number of AO coefficients there are
  int countCoef = 0;
  bool endFirstSection = false;
  // Skip first line simply has column numbers
  string line;
  getline(LR_ptr->fid_,line);
  string countC;

  // First group of coordinates will be stored in vector<vector<double>> 
  vector<vector<double>>  first_coefs;
  while(!endFirstSection){
    getline(LR_ptr->fid_,line);
    istringstream iss(line);
    iss >> countC;
    int countCint = stoi(countC);
    if(countCint!=(countCoef+1)){
      endFirstSection=true;
    }else{
      ++countCoef;
      vector<double> row;
       
      // Safe to read in coordinates of first couple of columns
      while(!iss.eof()){
        string s_coef;
        iss >> s_coef;
        string val = grabStrBeforeFirstOccurance(s_coef,"D");
        string exp = grabStrAfterFirstOccurance(s_coef,"D");
        row.push_back(stod(val)*pow(10.0,stod(exp)));
      }
      first_coefs.push_back(row);
    }
  }

  // Create a matrix and place all the current values in there
  Matrix * mat_S = new Matrix(countCoef,countCoef);
  for(size_t row_ind=0; row_ind<first_coefs.size();++row_ind){
    vector<double> row = first_coefs.at(row_ind);
    size_t col_ind = 1;
    for( auto val : row ){
      mat_S->set_elem(val,row_ind+1,col_ind);
      // Because diagonally symetric
      if(row_ind+1!=col_ind){
        mat_S->set_elem(val,col_ind,row_ind+1);
      }
      ++col_ind;
    }
  }

  int sectionReads = countCoef/5;
  if(countCoef%5>0){
    ++sectionReads;
  }
 
  int section = 2;
  int currentSectionStart= 5;
  while(section<=sectionReads){
    
    int sectionCoef = currentSectionStart;
    while(sectionCoef<countCoef){
      getline(LR_ptr->fid_,line);
      istringstream iss(line);
      string dummy;
      iss >> dummy;
      int localCoefCount = 1;
      while(!iss.eof()){
        string s_coef;
        iss >> s_coef;
        string val = grabStrBeforeFirstOccurance(s_coef,"D");
        string expon = grabStrAfterFirstOccurance(s_coef,"D");
        double value = stod(val)*pow(10.0,stod(expon));
        mat_S->set_elem(value,sectionCoef+1,currentSectionStart+localCoefCount);
        if((sectionCoef+1)!=(currentSectionStart+localCoefCount)){
        
          mat_S->set_elem(value,currentSectionStart+localCoefCount,sectionCoef+1);
        }
        ++localCoefCount;
      }
      ++sectionCoef;
    }
    ++section;
    currentSectionStart+=5;
    getline(LR_ptr->fid_,line);
  }
  LR_ptr->S_ = mat_S;
  return;
}
