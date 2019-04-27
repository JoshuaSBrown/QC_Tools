
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <Eigen/Dense>
#include "../../log.hpp"
#include "../../string_support.hpp"
#include "logreader.hpp"

using namespace catnip;
using namespace std;

LogReader::LogReader(const string &fileName) : FileReader(fileName) {
  validFileName_();
  registerSections_();
}

void LogReader::registerSections_() {
  sectionHeaders_["AOFunction"] =
      set<string>{"     Gross orbital populations:"};
  sectionHeaders_["Overlap"] = set<string>{" *** Overlap ***"};
  sectionHeaders_["OEAlpha"] = set<string>{" Alpha  occ. eigenvalues --"};
  sectionHeaders_["OEBeta"] = set<string>{"  Beta  occ. eigenvalues --"};
  sectionHeaders_["Coord"] = set<string>{"Center     Atomic      Atomic",
                                         "Center     Atomic     Atomic"};

  sectionReaders_["AOFunction"] = &LogReader::AOFunctionSectionReader;
  sectionReaders_["Overlap"] = &LogReader::OverlapSectionReader;
  sectionReaders_["OEAlpha"] = &LogReader::OrbitalEnergiesAlphaSectionReader;
  sectionReaders_["OEBeta"] = &LogReader::OrbitalEnergiesBetaSectionReader;
  sectionReaders_["Coord"] = &LogReader::CoordSectionReader;

  FileReader::registerSections_();
}

void LogReader::validFileName_() {
  string ext = fileExt_();
  if (ext.compare(".log") != 0) {
    throw invalid_argument("A log file must have the .log extension");
  }
}

vector<int> LogReader::getBasisFuncCount() {
  vector<int> basisFuncCount;
  for (auto values : orb_) {
    basisFuncCount.push_back(values.second.size());
  }
  return basisFuncCount;
}

void LogReader::AOFunctionSectionReader(void *ptr) {
  LOG("Reading atomic basis functions from .log file", 1);
  LogReader *LR_ptr = static_cast<LogReader *>(ptr);

  int atom_num = 1;
  string end_pattern = "         Condensed to atoms";
  string line;
  getline(LR_ptr->fid_, line);
  getline(LR_ptr->fid_, line);
  bool sectionEnd = false;
  while (sectionEnd == false) {
    istringstream iss(line);
    string AOnum;
    iss >> AOnum;
    string elemType;
    iss >> elemType;
    iss >> elemType;

    string orbType;
    iss >> orbType;
    vector<double> orbVals;
    while (!iss.eof()) {
      string Tot_Alpha_Beta_Spin;
      iss >> Tot_Alpha_Beta_Spin;
      orbVals.push_back(stod(Tot_Alpha_Beta_Spin));
    }
    LR_ptr->orb_[make_pair(atom_num, elemType)][orbType] = orbVals;

    getline(LR_ptr->fid_, line);
    string elemType2 = "";
    elemType2.append(string(1, line.at(9)));
    elemType2.append(string(1, line.at(10)));
    trim(elemType2);

    while (!isAlphabetical(elemType2)) {
      istringstream iss2(line);
      iss2 >> AOnum;
      iss2 >> orbType;

      orbVals.clear();
      while (!iss2.eof()) {
        string Tot_Alpha_Beta_Spin;
        iss2 >> Tot_Alpha_Beta_Spin;
        orbVals.push_back(stod(Tot_Alpha_Beta_Spin));
      }

      LR_ptr->orb_[make_pair(atom_num, elemType)][orbType] = orbVals;

      getline(LR_ptr->fid_, line);

      istringstream iss3(line);
      iss3 >> elemType2;
      iss3 >> elemType2;
      iss3 >> elemType2;
      trim(elemType2);

      if (foundSubStrInStr(line, end_pattern)) {
        sectionEnd = true;
        break;
      }
    }

    ++atom_num;
    // getline(LR_ptr->fid_,line);
    if (foundSubStrInStr(line, end_pattern)) {
      sectionEnd = true;
    }
  }

  LOG("Success reading atomic basis functions from .log file", 2);
  return;
}

void LogReader::OverlapSectionReader(void *ptr) {
  LOG("Reading Overlap Coefficients from .log file", 1);
  LogReader *LR_ptr = static_cast<LogReader *>(ptr);

  // Phase one read in the first group of coordinates and count
  // to determien the number of AO coefficients there are
  int countCoef = 0;
  bool endFirstSection = false;
  // Skip first line simply has column numbers
  string line;
  getline(LR_ptr->fid_, line);
  string countC;

  // First group of coordinates will be stored in vector<vector<double>>
  vector<vector<double>> first_coefs;
  while (!endFirstSection) {
    getline(LR_ptr->fid_, line);
    istringstream iss(line);
    iss >> countC;
    int countCint = stoi(countC);
    if (countCint != (countCoef + 1)) {
      endFirstSection = true;
    } else {
      ++countCoef;
      vector<double> row;

      // Safe to read in coordinates of first couple of columns
      while (!iss.eof()) {
        string s_coef;
        iss >> s_coef;
        string val = grabStrBeforeFirstOccurance(s_coef, "D");
        string exp = grabStrAfterFirstOccurance(s_coef, "D");
        row.push_back(stod(val) * pow(10.0, stod(exp)));
      }
      first_coefs.push_back(row);
    }
  }

  // Create a matrix and place all the current values in there
  
  //Matrix *mat_S = new Matrix(countCoef, countCoef);
  Eigen::MatrixXd matrix_S(countCoef,countCoef);
  for (size_t row_ind = 0; row_ind < first_coefs.size(); ++row_ind) {
//    vector<double> row = first_coefs.at(row_ind);
    matrix_S.row(row_ind) << first_coefs.at(row_ind);
 //   size_t col_ind = 1;
//    size_t col_ind = 0;
 //   for (double & val : row) {
 //     mat_S->set_elem(val, row_ind + 1, col_ind);
  //    matrix_S(row_ind,col_ind) = val;
      // Because diagonally symetric
//      if (row_ind + 1 != col_ind) {
   //   if (row_ind != col_ind) {
        //mat_S->set_elem(val, col_ind, row_ind + 1);
   //     matrix_S(col_ind,row_ind) = val;
    //  }
    //  ++col_ind;
    //}
  }

  int sectionReads = countCoef / 5;
  if (countCoef % 5 > 0) {
    ++sectionReads;
  }

  int section = 2;
  int currentSectionStart = 5;
  while (section <= sectionReads) {

    int sectionCoef = currentSectionStart;
    while (sectionCoef < countCoef) {
      getline(LR_ptr->fid_, line);
      istringstream iss(line);
      string dummy;
      iss >> dummy;
      //int localCoefCount = 1;
      int localCoefCount = 0;
      while (!iss.eof()) {
        string s_coef;
        iss >> s_coef;
        string val = grabStrBeforeFirstOccurance(s_coef, "D");
        string expon = grabStrAfterFirstOccurance(s_coef, "D");
        double value = stod(val) * pow(10.0, stod(expon));
       
        //mat_S->set_elem(value, sectionCoef + 1,
        //                currentSectionStart + localCoefCount);
        matrix_S(sectionCoef,currentSectionStart + localCoefCount) = value;
        //if ((sectionCoef + 1) != (currentSectionStart + localCoefCount)) {
        if ((sectionCoef) != (currentSectionStart + localCoefCount)) {
          matrix_S(currentSectionStart + localCoefCount,sectionCoef) = value;
          //mat_S->set_elem(value, currentSectionStart + localCoefCount,
          //               sectionCoef + 1);
        }
        ++localCoefCount;
      }
      ++sectionCoef;
    }
    ++section;
    currentSectionStart += 5;
    getline(LR_ptr->fid_, line);
  }
  LR_ptr->S_ = mat_S;
  LOG("Success reading Overlap coefficients from .log file", 2);
  return;
}

void LogReader::ReadOrbEnergies(const string &orb_type) {

  LOG("Reading Orbital Energies from .log file", 1);
  string line;
  homoLevel[orb_type] = 0;
  // May be multiple energy sections we only wanted the latest one
  OREnergies[orb_type].clear();
  while (getline(fid_, line)) {

    bool occFound;
    bool virtFound;

    occFound =
        foundSubStrInStr(line, " " + orb_type + "  occ. eigenvalues -- ");
    virtFound =
        foundSubStrInStr(line, " " + orb_type + " virt. eigenvalues -- ");

    if (occFound || virtFound) {

      auto vec_str = splitSt(line);
      for (size_t inc = 4; inc < vec_str.size(); inc++) {
        OREnergies[orb_type].push_back((double)atof(vec_str.at(inc).c_str()));
        if (occFound) homoLevel[orb_type]++;
      }
    } else {
      break;
    }
    pos_ = fid_.tellg();
  }
  // Reset the fid because Beta will come right after alpha
  fid_.seekg(pos_);
  LOG("Success reading Orbital Energies from .log file", 2);
}

void LogReader::OrbitalEnergiesAlphaSectionReader(void *ptr) {
  LogReader *LR_ptr = static_cast<LogReader *>(ptr);
  string line;
  LR_ptr->fid_.seekg(LR_ptr->pos_);
  LR_ptr->ReadOrbEnergies("Alpha");
}

void LogReader::OrbitalEnergiesBetaSectionReader(void *ptr) {
  LogReader *LR_ptr = static_cast<LogReader *>(ptr);
  string line;
  LR_ptr->fid_.seekg(LR_ptr->pos_);
  LR_ptr->ReadOrbEnergies("Beta");
}

void LogReader::CoordSectionReader(void *ptr) {
  LOG("Reading Coordinates from .log file", 1);
  LogReader *LR_ptr = static_cast<LogReader *>(ptr);

  // Clear contents only want the most uptodate coordinates
  LR_ptr->xyz.clear();
  vector<double> X;
  vector<double> Y;
  vector<double> Z;

  string end_pattern =
      "-----------------------------------"
      "----------------------------------";
  string line;
  // Skip the 2nd and third line
  getline(LR_ptr->fid_, line);
  getline(LR_ptr->fid_, line);
  getline(LR_ptr->fid_, line);

  // While the line does not match the end of the table read in the coordinates
  while (!foundSubStrInStr(line, end_pattern)) {
    auto vec_str = splitSt(line);
    X.push_back(stod(vec_str.at(3)));
    Y.push_back(stod(vec_str.at(4)));
    Z.push_back(stod(vec_str.at(5)));
    getline(LR_ptr->fid_, line);
  }
  LR_ptr->xyz.push_back(X);
  LR_ptr->xyz.push_back(Y);
  LR_ptr->xyz.push_back(Z);

  LOG("Success reading Coordinates from .log file", 2);
}
