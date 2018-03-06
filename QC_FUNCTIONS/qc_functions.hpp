#ifndef _QC_FUNCTIONS_HPP
#define _QC_FUNCTIONS_HPP
#include <sstream>
#include <string>
#include <unordered_map>

#include "../MATRIX/matrix.hpp"

std::unordered_map<int,std::pair<double,std::string>> findRank(Matrix & Orb_E_Alpha, Matrix & Orb_E_Beta);

double calculate_transfer_integral(
    Matrix mat_1_Coef,
    Matrix mat_2_Coef,
    Matrix mat_P_Coef,
    int MO1,
    int MO2,
    Matrix mat_S,
    Matrix mat_P_OE);

// Reorganize the dimer coefficients to match up with the monomers
Matrix organize_P_Coef(std::vector<int> matchDimerA,
    std::vector<int> matchDimerB,
    std::vector<int> basisFuncA,
    std::vector<int> basisFuncB,
    std::vector<int> basisFuncDimer,
    Matrix dimerCoef);

// Reorganize the dimer overlap matrix to line up with the monomer
// coefficients. 
/*Matrix organize_S(std::vector<int> matchDimerA,
    std::vector<int> matchDimerB,
    std::vector<int> basisFuncA,
    std::vector<int> basisFuncB,
    std::vector<int> basisFuncDimer,
    Matrix S);
*/
#endif
