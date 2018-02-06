#ifndef _QC_FUNCTIONS_HPP
#define _QC_FUNCTIONS_HPP
#include <sstream>
#include <string>
#include <unordered_map>

#include "../MATRIX/matrix.hpp"

std::unordered_map<int,std::pair<double,std::string>> findRank(Matrix & Orb_E_Alpha, Matrix & Orb_E_Beta);

#endif
