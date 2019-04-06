#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unordered_map>

#include "constants.hpp"
#include "io/io.hpp"
#include "log.hpp"
#include "matrix.hpp"
#include "qc_functions.hpp"

using namespace std;

namespace catnip {

unordered_map<int, pair<double, string>> findRank(Matrix &Orb_E_Alpha,
                                                  Matrix &Orb_E_Beta) {

  vector<pair<double, string>> all;

  auto m_a = Orb_E_Alpha.getCol(1);

  for (int ind = 1; ind <= m_a.get_rows(); ind++) {
    auto val = m_a.get_elem(ind);
    pair<double, string> pr(val, "Alpha");
    all.push_back(pr);
  }

  if (Orb_E_Beta.get_cols() > 0) {
    auto m_b = Orb_E_Beta.getCol(1);

    for (int ind = 1; ind <= m_b.get_rows(); ind++) {
      auto val = m_b.get_elem(ind);
      pair<double, string> pr(val, "Beta");
      all.push_back(pr);
    }
  }
  // Sort the vectors
  sort(all.begin(), all.end(),
       [](const pair<double, string> &P1, const pair<double, string> &P2)
           -> bool { return P1.first < P2.first; });
  // Now that they are sorted we are going to update the ranks
  int rank = 1;

  unordered_map<int, pair<double, string>> rank_map;

  for (auto pr : all) {
    rank_map[rank] = pr;
    rank++;
  }
  return rank_map;
}

// Essentially calculates the transfer integral
double calculate_transfer_integral(const Matrix & mat_1_Coef, const Matrix & mat_2_Coef,
                                   Matrix mat_P_Coef,const Matrix & mat_S,
                                   const Matrix &mat_P_OE, bool counterPoise_) {

  Matrix mat_1_Coefinv = mat_1_Coef.invert();
  Matrix mat_2_Coefinv = mat_2_Coef.invert();
  Matrix mat_P_Coefinv = mat_P_Coef.invert();

  Matrix zetaA;
  Matrix zetaB;
  if (counterPoise_) {
    LOG("Creating zeta matrices from coefficients assuming counterpoise", 2);
    zetaA = (mat_1_Coefinv);
    zetaB = (mat_2_Coefinv);
  } else {
    LOG("Creating zeta matrices from coefficients", 2);
    Matrix zerosA(mat_1_Coefinv.get_rows(), 1, mat_1_Coefinv.get_shel());
    Matrix zerosB(mat_2_Coefinv.get_rows(), 1, mat_2_Coefinv.get_shel());
    zetaA = Matrix_concatenate_rows(mat_1_Coefinv, zerosB);
    zetaB = Matrix_concatenate_rows(zerosA, mat_2_Coefinv);
  }
  Matrix zetaAinv = zetaA.invert();
  Matrix zetaBinv = zetaB.invert();

  LOG("Creating intermediate matrix", 3);
  Matrix Inter = mat_S * mat_P_Coefinv;

  LOG("Creating gamma and beta matrices", 2);
  Matrix gammaA = zetaAinv * Inter;
  Matrix gammaB = zetaBinv * Inter;

  Matrix gammaA_inv = gammaA.invert();
  Matrix gammaB_inv = gammaB.invert();

  LOG("Calculating S_AB", 2);
  Matrix S_AB = gammaB * gammaA_inv;

  Matrix Energy = Matrix_diag(mat_P_OE);
  Matrix J_AB = gammaB * (Energy * gammaA_inv);

  Matrix e_B = gammaB * (Energy * gammaB_inv);
  Matrix e_A = gammaA * (Energy * gammaA_inv);

  double J_ab = J_AB.get_elem(1, 1);
  double e_b = e_B.get_elem(1, 1);
  double e_a = e_A.get_elem(1, 1);
  double S_ab = S_AB.get_elem(1, 1);

  double J_eff = (J_ab - 1 / ((double)2) * (e_b + e_a) * S_ab);
  J_eff = J_eff / ((double)(1 - pow(S_ab, 2)));

  cout << "J_ab  " << J_ab * hartreeToeV << " eV\n";
  cout << "e_a   " << e_a * hartreeToeV << " eV\n";
  cout << "e_b   " << e_b * hartreeToeV << " eV\n";
  cout << "S_ab  " << S_ab << "\n";
  cout << "J_eff " << J_eff * hartreeToeV << " eV\n";

  return J_eff;
}

// Split a matrix up into a list of smaller matrices. The marix can be split
// in columns or in rows. The number of rows/cols in each smaller matrix is
// held in the vector<int> subMatrixDimension
//
// E.g. Given the matrix
//
// 1 2 3
// 3 4 5
//
// subMatrixDimension = { 2 1 }
//
// And I split by cols
//
// list<Matrix *> contain
//
// matrix 1   matrix 2
//   1 2         3
//   3 4         5
//
list<Matrix *> splitMatrixIntoList(const vector<int> &subMatrixDimension,
                                   const Matrix *const mat,
                                   const string &ColRowSplit) {

  list<Matrix *> list_matrix;
  int num_sub_matrices = subMatrixDimension.size();
  if (ColRowSplit.compare("Columns") == 0) {
    int col = 0;
    for (auto i = 1; i <= num_sub_matrices; ++i) {
      Matrix *mat_new =
          new Matrix(mat->get_rows(), subMatrixDimension.at(i - 1));
      for (auto k = 1; k <= mat->get_rows(); ++k) {
        for (auto j = 1; j <= subMatrixDimension.at(i - 1); ++j) {
          mat_new->set_elem(mat->get_elem(k, col + j), k, j);
        }
      }
      col += subMatrixDimension.at(i - 1);
      list_matrix.push_back(mat_new);
    }
  } else if (ColRowSplit.compare("Rows") == 0) {
    int row = 0;
    for (auto i = 1; i <= num_sub_matrices; ++i) {
      Matrix *mat_new =
          new Matrix(subMatrixDimension.at(i - 1), mat->get_cols());
      for (auto k = 1; k <= mat->get_cols(); ++k) {
        for (auto j = 1; j <= subMatrixDimension.at(i - 1); ++j) {
          mat_new->set_elem(mat->get_elem(row + j, k), j, k);
        }
      }
      row += subMatrixDimension.at(i - 1);
      list_matrix.push_back(mat_new);
    }
  } else {
    throw invalid_argument("Unrecognized string tag provided");
  }
  return list_matrix;
}

list<Matrix *> splitCoefsUpByAtoms(const vector<int> & basisFuncP, Matrix *Coefs,
                                   const string & ColRow) {
  return splitMatrixIntoList(basisFuncP, Coefs, ColRow);
}

// The purpose of this function is to ensure that swapping occurs correctly
// The following example explains in more detail what this is meant to do
// Now if we have a matrix in a specific state and want to swap the rows
// we can not simply list all the rows that need to be changed and then make
// the changes.
//
// We have to figure out how to swap rows sequentially without ending up with
// the wrong result.
//
// For instance say we have a matrix
// 1 row1
// 2 row2
// 3 row3
// 4 row4
//
// where:
// Row 1 needs to be at row 3
// Row 4 needs to be at row 1
// Row 2 needs to be at row 2
// Row 3 needs to be at row 4
//
// If we simply looked at our first instance of the matrix shown above
// and came up with the following swaps
// Swap row 1 with row 3
// Swap row 4 with row 1
// Swap row 2 with row 2
// Swap row 3 with row 4
//
// First swap
// 1 row3
// 2 row2
// 3 row1
// 4 row4
//
// Second swap
// 1 row4
// 2 row2
// 3 row1
// 4 row3
//
// Third swap does nothing
// Fourth swap
// 1 row4
// 2 row2
// 3 row3
// 4 row1
//
// When what we really wanted was
// 1 row4
// 2 row2
// 3 row1
// 4 row3
//
// Notice the row3 and row1 are in the wrong place
// The instructions should be rewritten
// Swap row 1 with row 3
// Swap row 4 with row 3
// Swap row 2 with row 2 - no swap
// Swap row 3 with row 3 - no swap
void refreshSwapOrder(vector<pair<int, int>> &monBmatch,
                      vector<pair<int, int>> &monAmatch) {

  // Because matrix B appears at the top of our full matrix e.g.
  //
  // [ B ]
  // [ A ]
  //
  // When sorting out the swap order we need to cycle through elements in B and
  // see where they appear in both the B and A matrix
  //
  // B -> [ B ]
  // B -> [ A ]
  //
  for (auto pr_ptr = monBmatch.begin(); pr_ptr != monBmatch.end(); ++pr_ptr) {
    auto pr = *pr_ptr;

    int row1was = pr.first;
    int row1is = pr.second;
    auto pr_ptr_temp = pr_ptr;
    pr_ptr_temp++;
    while (pr_ptr_temp != monBmatch.end()) {
      pr_ptr_temp = find_if(
          pr_ptr_temp, monBmatch.end(),
          [row1was](const pair<int, int> &p) { return row1was == p.second; });
      if (pr_ptr_temp != monBmatch.end()) {
        pr_ptr_temp->second = row1is;
      }
    }
    pr_ptr_temp = monAmatch.begin();
    while (pr_ptr_temp != monAmatch.end()) {
      pr_ptr_temp = find_if(
          pr_ptr_temp, monAmatch.end(),
          [row1was](const pair<int, int> &p) { return row1was == p.second; });
      if (pr_ptr_temp != monAmatch.end()) {
        pr_ptr_temp->second = row1is;
      }
    }
  }
  // Here we have already looked at the rows in B and they are up to date but
  // we still need to make sure the rows in A are swapped in the appropriate
  // order
  //
  //      [ B ]
  // A -> [ A ]
  //
  for (auto pr_ptr = monAmatch.begin(); pr_ptr != monAmatch.end(); ++pr_ptr) {
    auto pr = *pr_ptr;
    int row1was = pr.first;
    int row1is = pr.second;
    if (row1was != row1is) {
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while (pr_ptr_temp != monAmatch.end()) {
        pr_ptr_temp = find_if(
            pr_ptr_temp, monAmatch.end(),
            [row1was](const pair<int, int> &p) { return row1was == p.second; });
        if (pr_ptr_temp != monAmatch.end()) {
          pr_ptr_temp->second = row1is;
        }
      }
    }
  }
  return;
}

void refreshSwapOrder(vector<pair<int, int>> &monAmatch) {

  for (auto pr_ptr = monAmatch.begin(); pr_ptr != monAmatch.end(); ++pr_ptr) {
    auto pr = *pr_ptr;

    int row1was = pr.first;
    int row1is = pr.second;
    auto pr_ptr_temp = pr_ptr;
    pr_ptr_temp++;
    while (pr_ptr_temp != monAmatch.end()) {
      pr_ptr_temp = find_if(
          pr_ptr_temp, monAmatch.end(),
          [row1was](const pair<int, int> &p) { return row1was == p.second; });
      if (pr_ptr_temp != monAmatch.end()) {
        pr_ptr_temp->second = row1is;
      }
    }
  }
  return;
}

// The above function determines the appropriate sequence of swaps this function
// then actually implements the swaps by exchanging the matrices in the list.
void updateSwapLists(vector<pair<int, int>> &monBmatch,
                     vector<pair<int, int>> &monAmatch,
                     list<Matrix *> &p_atom_mat_coef) {

  for (auto p : monBmatch) {
    if (p.first != p.second) {
      auto it = p_atom_mat_coef.begin();
      Matrix *temp = *(next(it, p.first - 1));
      *(next(it, p.first - 1)) = *(next(it, p.second - 1));
      *(next(it, p.second - 1)) = temp;
    }
  }
  for (auto p : monAmatch) {
    if (p.first != p.second) {
      auto it = p_atom_mat_coef.begin();
      Matrix *temp = *(next(it, p.first - 1));
      *(next(it, p.first - 1)) = *(next(it, p.second - 1));
      *(next(it, p.second - 1)) = temp;
    }
  }
  return;
}
void updateSwapLists(vector<pair<int, int>> &monAmatch,
                     list<Matrix *> &atom_mat_coef) {

  for (auto p : monAmatch) {
    if (p.first != p.second) {
      auto it = atom_mat_coef.begin();
      Matrix *temp = *(next(it, p.first - 1));
      *(next(it, p.first - 1)) = *(next(it, p.second - 1));
      *(next(it, p.second - 1)) = temp;
    }
  }
  return;
}

Matrix *mergeListOfMatrices(list<Matrix *> &matrix_list, const int rows,
                            const int cols, const string &ColRowMerge) {

  Matrix *full_matrix = new Matrix(rows, cols);
  if (ColRowMerge.compare("Columns") == 0) {
    int col = 0;
    for (auto it = matrix_list.begin(); it != matrix_list.end(); ++it) {
      Matrix *mat_ptr = *it;
      int row = 1;
      if (col > cols)
        throw runtime_error("Your new matrix is not large enough");
      for (auto i = 1; i <= mat_ptr->get_rows(); ++i) {
        if (row > rows)
          throw runtime_error("Your new matrix is not large enough");
        for (auto j = 1; j <= mat_ptr->get_cols(); ++j) {
          full_matrix->set_elem(mat_ptr->get_elem(i, j), row, j + col);
        }
        ++row;
      }
      col += mat_ptr->get_cols();
    }
  } else if (ColRowMerge.compare("Rows") == 0) {
    int row = 0;
    for (auto it = matrix_list.begin(); it != matrix_list.end(); ++it) {
      Matrix *mat_ptr = *it;
      int col = 1;
      if (row > rows)
        throw runtime_error("Your new matrix is not large enough");
      for (auto j = 1; j <= mat_ptr->get_cols(); ++j) {
        if (col > cols)
          throw runtime_error("Your new matrix is not large enough");
        for (auto i = 1; i <= mat_ptr->get_rows(); ++i) {
          full_matrix->set_elem(mat_ptr->get_elem(i, j), row + i, col);
        }
        ++col;
      }
      row += mat_ptr->get_rows();
    }
  } else {
    throw invalid_argument("Unrecognized merge type for list of matrices");
  }
  return full_matrix;
}

// This function
Matrix *createNewMatrix(list<Matrix *> &p_atom_mat_coef, int rows, int cols,
                        const string &ColRow) {
  return mergeListOfMatrices(p_atom_mat_coef, rows, cols, ColRow);
}

// unscramble the coefficients
Matrix *unscramble_Coef(const std::vector<int> &matchDimerA,
                        const std::vector<int> &matchDimerB,
                        const std::vector<int> &basisFuncP, Matrix *dimerCoef) {

  // Let's reduce the complexity of the problem by instead of working
  // with the basis functions lets just work with the atoms. We can do
  // this by treating all the basis functions associated with a single
  // atom as a block.

  list<Matrix *> p_atom_mat_coef =
      splitCoefsUpByAtoms(basisFuncP, dimerCoef, "Columns");

  // Place all of monomer A atom basis functions on the left side of the
  // matrix and all of B monomer atom basis functions on the right side
  // of the dimer matrix
  // First int is the col in the dimer the atom should be at
  // Second int is the col in the dimer the atom is presently at

  vector<pair<int, int>> monAmatch;
  for (unsigned i = 1; i <= matchDimerA.size(); ++i) {
    pair<int, int> pr(i, matchDimerA.at(i - 1));
    monAmatch.push_back(pr);
  }
  vector<pair<int, int>> monBmatch;
  for (unsigned i = 1; i <= matchDimerB.size(); ++i) {
    pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i - 1));
    monBmatch.push_back(pr);
  }

  refreshSwapOrder(monBmatch, monAmatch);
  updateSwapLists(monBmatch, monAmatch, p_atom_mat_coef);
  Matrix *dimerCoef_new = createNewMatrix(
      p_atom_mat_coef, dimerCoef->get_rows(), dimerCoef->get_cols(), "Columns");
  return dimerCoef_new;
}

// unscramble the coefficients
Matrix *unscramble_Coef(const std::vector<int> &matchDimerB,
                        const std::vector<int> &basisFuncB, Matrix *Coef) {

  // Let's reduce the complexity of the problem by instead of working
  // with the basis functions lets just work with the atoms. We can do
  // this by treating all the basis functions associated with a single
  // atom as a block.

  list<Matrix *> atom_mat_coef =
      splitCoefsUpByAtoms(basisFuncB, Coef, "Columns");

  // Place all of monomer A atom basis functions on the left side of the
  // matrix and all of B monomer atom basis functions on the right side
  // of the  matrix
  // First int is the col in the dimer the atom should be at
  // Second int is the col in the dimer the atom is presently at

  vector<pair<int, int>> monBmatch;
  for (unsigned i = 1; i <= matchDimerB.size(); ++i) {
    pair<int, int> pr(i, matchDimerB.at(i - 1));
    monBmatch.push_back(pr);
  }

  refreshSwapOrder(monBmatch);
  updateSwapLists(monBmatch, atom_mat_coef);
  Matrix *Coef_new = createNewMatrix(atom_mat_coef, Coef->get_rows(),
                                     Coef->get_cols(), "Columns");
  return Coef_new;
}

// Similar to the above function but we will be moving both the rows
// and columns
Matrix *unscramble_S(const std::vector<int> &matchDimerA,
                     const std::vector<int> &matchDimerB,
                     const std::vector<int> &basisFuncP, Matrix *S) {

  Matrix *S_new;
  {
    list<Matrix *> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Columns");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 1; i <= matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i - 1));
      monAmatch.push_back(pr);
    }
    vector<pair<int, int>> monBmatch;
    for (unsigned i = 1; i <= matchDimerB.size(); ++i) {
      pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i - 1));
      monBmatch.push_back(pr);
    }

    refreshSwapOrder(monBmatch, monAmatch);
    updateSwapLists(monBmatch, monAmatch, p_atom_mat_S);
    S_new =
        createNewMatrix(p_atom_mat_S, S->get_rows(), S->get_cols(), "Columns");
  }

  S = S_new;

  {
    list<Matrix *> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Rows");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 1; i <= matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i - 1));
      monAmatch.push_back(pr);
    }
    vector<pair<int, int>> monBmatch;
    for (unsigned i = 1; i <= matchDimerB.size(); ++i) {
      pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i - 1));
      monBmatch.push_back(pr);
    }

    refreshSwapOrder(monBmatch, monAmatch);
    updateSwapLists(monBmatch, monAmatch, p_atom_mat_S);
    S_new = createNewMatrix(p_atom_mat_S, S->get_rows(), S->get_cols(), "Rows");
  }

  // Return the correctly swapped dimerCoef
  return S_new;
}

// Same as the above function but here we are assuming counterpoise correction
// is being used and thus we do not need to match with both monomer A and
// monomer B but only need to match with A.
Matrix *unscramble_S(const std::vector<int> &matchDimerA,
                     const std::vector<int> &basisFuncP, Matrix *S) {

  Matrix *S_new;
  {
    list<Matrix *> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Columns");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 1; i <= matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i - 1));
      monAmatch.push_back(pr);
    }
    refreshSwapOrder(monAmatch);
    updateSwapLists(monAmatch, p_atom_mat_S);
    S_new =
        createNewMatrix(p_atom_mat_S, S->get_rows(), S->get_cols(), "Columns");
  }

  S = S_new;

  {
    list<Matrix *> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Rows");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 1; i <= matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i - 1));
      monAmatch.push_back(pr);
    }

    refreshSwapOrder(monAmatch);
    updateSwapLists(monAmatch, p_atom_mat_S);
    S_new = createNewMatrix(p_atom_mat_S, S->get_rows(), S->get_cols(), "Rows");
  }

  // Return the correctly swapped dimerCoef
  return S_new;
}

}  // namespace catnip

using namespace catnip;

TransferComplex::TransferComplex(Matrix *mat1Coef, Matrix *mat2Coef,
                                 Matrix *matPCoef, std::pair<int, int> MOs1,
                                 std::pair<int, int> MOs2, Matrix *matS,
                                 Matrix *matPOE, bool cp) {

  unscrambled = false;
  counterPoise_ = cp;
  // Consistency check
  if (matS->get_cols() != matPCoef->get_cols()) {
    throw invalid_argument(
        "The overlap matrix must have the same number "
        "of basis functions as the dimer");
  }
  if (cp) {
    if (mat1Coef->get_cols() != matPCoef->get_cols()) {
      throw invalid_argument(
          "Counter poise correction requires that the"
          " monomers have the same number of coefficients as the dimer. "
          "Your monomer 1 does not");
    }
    if (mat2Coef->get_cols() != matPCoef->get_cols()) {
      throw invalid_argument(
          "Counter poise correction requires that the"
          " monomers have the same number of coefficients as the dimer. "
          "Your monomer 2 does not");
    }
  } else {
    int total_cols = mat1Coef->get_cols() + mat2Coef->get_cols();

    if (total_cols > matPCoef->get_cols()) {
      throw invalid_argument(
          "Counter poise has not been specified and your "
          "monomers have more basis function cols than your dimer");
    }
  }

  mat_1_Coef = mat1Coef;
  mat_2_Coef = mat2Coef;
  mat_P_Coef = matPCoef;
  Orbs1 = MOs1;
  Orbs2 = MOs2;
  mat_S = matS;
  mat_P_OE = matPOE;
}

void TransferComplex::unscramble(const Matrix &coord_1_mat,
                                 const Matrix &coord_2_mat,
                                 const Matrix &coord_P_mat,
                                 const std::vector<int> &basisP,
                                 const std::vector<int> &basis2) {

  unscrambled = true;

  const int sig_fig = 4;

  // If dealing with counter poise correction may also need to unscramble
  // the basis functions of the monomers
  if (counterPoise_) {
    vector<int> match_1_2 = coord_1_mat.matchCol(coord_2_mat, sig_fig);

    LOG("Counter Poise unscrambling matrix 2 with respect to matrix 1", 2);
    auto unscrambled_2_Coef = unscramble_Coef(match_1_2, basis2, mat_2_Coef);

    this->mat_2_Coef = unscrambled_2_Coef;

    vector<int> match_1_P = coord_1_mat.matchCol(coord_P_mat, sig_fig);

    auto unscrambled_P_Coef = unscramble_Coef(match_1_P, basisP, mat_P_Coef);

    this->mat_P_Coef = unscrambled_P_Coef;

    auto unscrambled_S = unscramble_S(match_1_P, basisP, mat_S);

    this->mat_S = unscrambled_S;

  } else {

    // Stores the rows in P that match 1
    vector<int> match_1_P = coord_1_mat.matchCol(coord_P_mat, sig_fig);

    // Stores the rows in P that match 2
    vector<int> match_2_P = coord_2_mat.matchCol(coord_P_mat, sig_fig);

    LOG("Unscrambling dimer matrix with respect to matrix 1 and 2", 2);
    auto unscrambled_P_Coef =
        unscramble_Coef(match_1_P, match_2_P, basisP, mat_P_Coef);

    this->mat_P_Coef = unscrambled_P_Coef;

    auto unscrambled_S = unscramble_S(match_1_P, match_2_P, basisP, mat_S);

    this->mat_S = unscrambled_S;
  }
}

double TransferComplex::calcJ(const map<string, string> &orbitaltype,
                              const map<string, int> &orbnum) {

  if (unscrambled == false) {
    cerr << "WARNING unable to automatically line up basis functions of"
            " monomers with dimers, you better make sure they correctly"
            " line up or run the calculations again with the correct "
            "flag pop=full"
         << endl;
  }

  Matrix mat1coef;
  Matrix mat2coef;

  string HOMO_OR_LUMO = orbitaltype.at("mon1");
  int MO = orbnum.at("mon1");
  if (HOMO_OR_LUMO.compare("HOMO") == 0) {
    if (MO > 0) {
      throw invalid_argument(
          "Having specified HOMO the MO"
          " value is in reference to the HOMO and must be a negative number");
    }
    // Number of orbitals that are choices
    if (MO <= (-1 * Orbs1.second)) {
      string err = "You are trying to access HOMO" + to_string(MO) +
                   " but there "
                   "are only " +
                   to_string(Orbs1.second) + " HOMO orbitals";
      throw invalid_argument(err);
    }
    mat1coef = mat_1_Coef->getRow(Orbs1.second + MO);
  } else if (HOMO_OR_LUMO.compare("LUMO") == 0) {
    if (MO < 0) {
      throw invalid_argument(
          "Having specified LUMO the MO"
          " value is in reference to the LUMO and must be a positive number");
    }
    int allowed_LUMO = Orbs1.first - Orbs1.second;
    if (MO >= allowed_LUMO) {
      string err = "You are trying to access LUMO+" + to_string(MO) +
                   " but there "
                   "are only " +
                   to_string(allowed_LUMO) + " LUMO orbitals";
      throw invalid_argument(err);
    }
    mat1coef = mat_1_Coef->getRow(Orbs1.second + MO + 1);
  } else {
    throw invalid_argument("orbitals must be referred to as HOMO or LUMO");
  }

  HOMO_OR_LUMO = orbitaltype.at("mon2");
  MO = orbnum.at("mon2");
  if (HOMO_OR_LUMO.compare("HOMO") == 0) {
    if (MO > 0) {
      throw invalid_argument(
          "Having specified HOMO the MO"
          " value is in reference to the HOMO and must be a negative number");
    }
    if (MO <= (-1 * Orbs2.second)) {
      string err = "You are trying to access HOMO" + to_string(MO) +
                   " but there "
                   "are only " +
                   to_string(Orbs2.second) + " HOMO orbitals";
      throw invalid_argument(err);
    }
    mat2coef = mat_2_Coef->getRow(Orbs2.second + MO);
  } else if (HOMO_OR_LUMO.compare("LUMO") == 0) {
    if (MO < 0) {
      throw invalid_argument(
          "Having specified LUMO the MO"
          " value is in reference to the LUMO and must be a positive number");
    }
    int allowed_LUMO = Orbs2.first - Orbs2.second;
    if (MO >= allowed_LUMO) {
      string err = "You are trying to access LUMO+" + to_string(MO) +
                   " but there "
                   "are only " +
                   to_string(allowed_LUMO) + " LUMO orbitals";
      throw invalid_argument(err);
    }
    mat2coef = mat_2_Coef->getRow(Orbs2.second + MO + 1);
  } else {
    throw invalid_argument("orbitals must be referred to as HOMO or LUMO");
  }

  return calculate_transfer_integral(mat1coef, mat2coef, *mat_P_Coef, *mat_S,
                                     *mat_P_OE, counterPoise_);
}
