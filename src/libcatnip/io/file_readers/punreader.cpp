
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "../../log.hpp"
#include "../../string_support.hpp"
#include "punreader.hpp"

using namespace catnip;
using namespace std;

PunReader::PunReader(const string &fileName) : FileReader(fileName) {
  validFileName_();
  registerSections_();
}

void PunReader::registerSections_() {
  sectionHeaders_["CoefAlpha"] = set<string>{" 1 Alpha MO "};
  sectionHeaders_["CoefBeta"] = set<string>{" 1 Beta MO "};

  sectionReaders_["CoefAlpha"] = &PunReader::OrbitalCoefAlphaSectionReader;
  sectionReaders_["CoefBeta"] = &PunReader::OrbitalCoefBetaSectionReader;

  FileReader::registerSections_();
}

Matrix *PunReader::getCoefsMatrix(const string &orb_type) {
  if (coefs.count(orb_type) != 1) {
    throw invalid_argument("Coefficients for spin " + orb_type +
                           " were not found");
  }
  return coefs[orb_type];
}

void PunReader::validFileName_() {
  string ext = fileExt_();
  if (ext.compare(".pun") != 0) {
    throw invalid_argument("A pun file must have the .pun extension");
  }
}

vector<double> PunReader::readGausCoefLine(const string &line) {
  vector<double> values;

  int indent = 0;
  while ((line.length() - indent) >= 15) {
    string str = line.substr(0 + indent, 11);
    str = trimmed(str);
    double temp_d = stod(str);
    str = line.substr(12 + indent, 3);
    str = trimmed(str);
    double temp_d2 = stod(str);
    values.push_back(temp_d * pow(10, temp_d2));
    indent += 15;
  }
  return values;
}

void PunReader::ReadCoef(const string &orb_type) {

  LOG("Reading atomic orbital coefficients from .pun file.", 1);
  string pattern = " MO OE";

  vector<vector<double>> v_vec;
  size_t count = 0;
  bool first_row_set = false;

  bool allCoefsRead = false;
  while (!allCoefsRead) {
    string line;
    getline(fid_, line);
    vector<double> row;
    size_t current_count = 0;
    while (!foundSubStrInStr(line, pattern)) {

      if (first_row_set && current_count >= count) break;
      auto coef = readGausCoefLine(line);
      for (auto val : coef) {
        row.push_back(val);
        ++current_count;
      }
      getline(fid_, line);
    }
    first_row_set = true;
    count = row.size();
    v_vec.push_back(row);

    allCoefsRead = !(foundSubStrInStr(line, orb_type));
  }

  Matrix *Coefs = new Matrix(v_vec);
  coefs[orb_type] = Coefs;
  LOG("Success reading atomic orbital coefficients from .pun file.", 2);
}

void PunReader::OrbitalCoefAlphaSectionReader(void *ptr) {
  PunReader *PR_ptr = static_cast<PunReader *>(ptr);
  PR_ptr->ReadCoef("Alpha");
}

void PunReader::OrbitalCoefBetaSectionReader(void *ptr) {
  PunReader *PR_ptr = static_cast<PunReader *>(ptr);
  PR_ptr->ReadCoef("Beta");
}
