#ifndef _CATNIP_QC_FUNCTIONS_HPP
#define _CATNIP_QC_FUNCTIONS_HPP

#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "matrix.hpp"

namespace catnip {

class TransferComplex {
 private:
  Matrix* mat_1_Coef;
  Matrix* mat_2_Coef;
  Matrix* mat_P_Coef;
  // Stores the number of Molecular orbitals
  // and the HOMO for both monomer 1 and 2
  std::pair<int, int> Orbs1;
  std::pair<int, int> Orbs2;
  Matrix* mat_S;
  Matrix* mat_P_OE;
  // If unscrambaling is required
  bool unscrambled;
  bool counterPoise_;

 public:
  TransferComplex(Matrix* mat1Coef, Matrix* mat2Coef, Matrix* matPCoef,
                  std::pair<int, int> MOs1, std::pair<int, int> MOs2,
                  Matrix* matS, Matrix* matPOE, bool cp);

  void unscramble(const Matrix& coord_1_mat, const Matrix& coord_2_mat,
                  const Matrix& coord_P_mat, const std::vector<int>& basisP,
                  const std::vector<int>& basis2);

  // Orbital type and a map of the corresponding number
  // E.g.
  // orbital type    orbital number
  // "mon1" "LUMO"    "mon1" -3
  // "mon2" "HOMO"    "mon2"  0
  //
  //    monomer1 LUMO-3
  //    monomer2 HOMO
  double calcJ(const std::map<std::string, std::string>& orbitaltype,
               const std::map<std::string, int>& orbitalnum);
};

std::unordered_map<int, std::pair<double, std::string>> findRank(
    Matrix& Orb_E_Alpha, Matrix& Orb_E_Beta);

double calculate_transfer_integral(const Matrix & mat_1_Coef, const Matrix & mat_2_Coef,
                                   Matrix mat_P_Coef,const Matrix & mat_S,
                                   const Matrix& mat_P_OE, bool counterPoise_);

// Reorganize the dimer coefficients to match up with the monomers
Matrix organize_P_Coef(std::vector<int> matchDimerA,
                       std::vector<int> matchDimerB,
                       std::vector<int> basisFuncA, std::vector<int> basisFuncB,
                       std::vector<int> basisFuncDimer, Matrix dimerCoef);

// Unscramble the coefficients of the dimer matrix
// Assumes that the match vectors describe swaps looking at a single
// instance of the dimerCoef matrix
Matrix* unscramble_Coef(const std::vector<int>& matchDimerA,
                        const std::vector<int>& matchDimerB,
                        const std::vector<int>& basisFuncDimer,
                        Matrix* dimerCoef);

Matrix* unscramble_Coef(const std::vector<int>& matchDimerA,
                        const std::vector<int>& basisFuncDimer, Matrix* dimerCoef);

// Reorganize the dimer overlap matrix to line up with the monomer
// coefficients.
Matrix* unscramble_S(const std::vector<int>& matchDimerA,
                     const std::vector<int>& matchDimerB,
                     const std::vector<int>& basisFuncDimer, Matrix* S);

Matrix* unscramble_S(const std::vector<int>& matchDimerA,
                     const std::vector<int>& basisFuncDimer, Matrix* S);

Matrix* unscramble_OE(std::vector<int> matchDimerA,
                      std::vector<int> matchDimerB,
                      std::vector<int> basisFuncDimer, Matrix* OE);

}  // namespace catnip
#endif  // _CATNIP_QC_FUNCTIONS_HPP
