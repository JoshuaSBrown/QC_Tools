#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
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

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Eigenvalues>
namespace catnip {

  using namespace std;

unordered_map<int, pair<double, string>> findRank(Eigen::VectorXd &Orb_E_Alpha,
                                                  Eigen::VectorXd &Orb_E_Beta) {

  vector<pair<double, string>> all;

  for( int i = 0; i<Orb_E_Alpha.size();++i){
    all.push_back(pair<double, string>(Orb_E_Alpha(i),"Alpha"));
  }

  for( int i = 0; i<Orb_E_Beta.size();++i){
    all.push_back(pair<double, string>(Orb_E_Beta(i), "Beta"));
  }
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
void TransferComplex::calculate_transfer_integral_() {

  auto dimension = mat_1_Coef.rows()+mat_2_Coef.rows();
  Eigen::MatrixXd zetaA(dimension,dimension);
  Eigen::MatrixXd zetaB(dimension,dimension);
  if (counterPoise_) {
    LOG("Creating zeta matrices from coefficients assuming counterpoise", 2);
    zetaA = mat_1_Coef;
    zetaB = mat_2_Coef;
  } else {
    LOG("Creating zeta matrices from coefficients", 2);
    zetaA << mat_1_Coef, Eigen::MatrixXd::Zero(mat_1_Coef.rows(),mat_2_Coef.cols()); 
    zetaA << Eigen::MatrixXd::Zero(mat_2_Coef.rows(),mat_1_Coef.cols()), mat_2_Coef; 
  }

  LOG("Creating gamma and beta matrices", 2);
  Eigen::MatrixXd gammaA = zetaA * mat_S * mat_P_Coef.transpose(); 
  Eigen::MatrixXd gammaB = zetaB * mat_S * mat_P_Coef.transpose(); 


  LOG("Calculating S_AB", 2);
  S_AB.resize(gammaB.rows(),gammaA.cols());
  S_AB = gammaB * gammaA.transpose();
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigen_solver(S_AB);
  Eigen::MatrixXd S_AB_inv_sqrt = eigen_solver.operatorInverseSqrt();

  Eigen::MatrixXd Energy = Eigen::MatrixXd::Identity(vec_P_OE.size(),vec_P_OE.size());

  Hamiltonian.resize(mat_S.rows(),mat_S.cols());
  Hamiltonian = mat_P_Coef * mat_S * mat_P_Coef.transpose();

  Hamiltonian_eff.resize(Hamiltonian.rows(),Hamiltonian.cols());
  Hamiltonian_eff = S_AB_inv_sqrt * Hamiltonian * S_AB_inv_sqrt.transpose();  
  
}

void TransferComplex::printTransferIntegral(
    const map<string, string> &orbitaltype,
    const map<string, int> &orbnum) const {

  string HOMO_OR_LUMO_A = orbitaltype.at("mon1");
  int MO_A = orbnum.at("mon1");
  if (HOMO_OR_LUMO_A.compare("HOMO") == 0) {
    if (MO_A > 0) {
      throw invalid_argument(
          "Having specified HOMO the MO_A"
          " value is in reference to the HOMO and must be a negative number");
    }
    // Number of orbitals that are choices
    if (MO_A <= (-1 * Orbs1.second)) {
      string err = "You are trying to access HOMO" + to_string(MO_A) +
                   " but there "
                   "are only " +
                   to_string(Orbs1.second) + " HOMO orbitals";
      throw invalid_argument(err);
    }
  } else if (HOMO_OR_LUMO_A.compare("LUMO") == 0) {
    if (MO_A < 0) {
      throw invalid_argument(
          "Having specified LUMO the MO_A"
          " value is in reference to the LUMO and must be a positive number");
    }
    int allowed_LUMO = Orbs1.first - Orbs1.second;
    if (MO_A >= allowed_LUMO) {
      string err = "You are trying to access LUMO+" + to_string(MO_A) +
                   " but there "
                   "are only " +
                   to_string(allowed_LUMO) + " LUMO orbitals";
      throw invalid_argument(err);
    }
  } else {
    throw invalid_argument("orbitals must be referred to as HOMO or LUMO");
  }

  string HOMO_OR_LUMO_B = orbitaltype.at("mon2");
  int MO_B = orbnum.at("mon2");
  if (HOMO_OR_LUMO_B.compare("HOMO") == 0) {
    if (MO_B > 0) {
      throw invalid_argument(
          "Having specified HOMO the MO_B"
          " value is in reference to the HOMO and must be a negative number");
    }
    if (MO_B <= (-1 * Orbs2.second)) {
      string err = "You are trying to access HOMO" + to_string(MO_B) +
                   " but there "
                   "are only " +
                   to_string(Orbs2.second) + " HOMO orbitals";
      throw invalid_argument(err);
    }
  } else if (HOMO_OR_LUMO_B.compare("LUMO") == 0) {
    if (MO_B < 0) {
      throw invalid_argument(
          "Having specified LUMO the MO_B"
          " value is in reference to the LUMO and must be a positive number");
    }
    int allowed_LUMO = Orbs2.first - Orbs2.second;
    if (MO_B >= allowed_LUMO) {
      string err = "You are trying to access LUMO+" + to_string(MO_B) +
                   " but there "
                   "are only " +
                   to_string(allowed_LUMO) + " LUMO orbitals";
      throw invalid_argument(err);
    }
  } else {
    throw invalid_argument("orbitals must be referred to as HOMO or LUMO");
  }

  printTransferIntegral_(
      pair<string,int>(HOMO_OR_LUMO_A,MO_A),
      pair<string,int>(HOMO_OR_LUMO_B,MO_B));
}
// Find the transfer integral between two orbitals. 
// pair<string,int> string - Either HOMO or LUMO
//                  int    - is the orbital number HOMO-3 LUMO+5
void TransferComplex::printTransferIntegral_(
    const pair<string,int>& orbital1,
    const pair<string,int>& orbital2) const {

  int orbital1_num = 0;
  int orbital2_num = 0;
  if(orbital1.first.compare("HOMO")==0){
    assert(orbitalValid_(orbital1)==true); 
    orbital1_num = orbital1.second;
  }else if(orbital2.first.compare("LUMO")==0){
    assert(orbitalValid_(orbital2)==true); 
    orbital2_num = orbital2.second;
  }
  double J_ab = Hamiltonian(orbital1_num,orbital2_num);
  double e_a = Hamiltonian(orbital1_num,orbital1_num);
  double e_b = Hamiltonian(orbital2_num,orbital2_num);
  double S_ab = S_AB(orbital1_num,orbital2_num); 
  double J_eff = Hamiltonian_eff(orbital1_num,orbital2_num);
  cout << "J_ab  " << J_ab * hartreeToeV << " eV\n";
  cout << "e_a   " << e_a * hartreeToeV << " eV\n";
  cout << "e_b   " << e_b * hartreeToeV << " eV\n";
  cout << "S_ab  " << S_ab << "\n";
  cout << "J_eff " << J_eff * hartreeToeV << " eV\n";
}

void TransferComplex::printAll() const {

  int column_width = 8;
  cout << "Effective Hamiltonian" << endl;
  cout << setw(column_width+2) << ""; 
  for(int orbital_num = 0;orbital_num<Hamiltonian.cols();++orbital_num){
    string column_label = "";
    if(orbital_num<HOMO_Orb_){
      column_label+=string("HOMO");
      if(orbital_num!=(HOMO_Orb_-1)){
        column_label+=string("-")+to_string(HOMO_Orb_-orbital_num-1);
      }
    }else {
      column_label+=string("LUMO");
      if(orbital_num!=LUMO_Orb_){
        column_label+=string("+")+to_string(orbital_num-LUMO_Orb_);
      }
    } 
    cout << "| " << setw(column_width) << column_label << " ";
  }
  cout << "|" << endl; 
 
  for(int orbital_num = 0;orbital_num<Hamiltonian.rows();++orbital_num){
    string column_label = "";
    if(orbital_num<HOMO_Orb_){
      column_label+=string("HOMO");
      if(orbital_num!=(HOMO_Orb_-1)){
        column_label+=string("-")+to_string(HOMO_Orb_-orbital_num-1);
      }
    }else {
      column_label+=string("LUMO");
      if(orbital_num!=LUMO_Orb_){
        column_label+=string("+")+to_string(orbital_num-LUMO_Orb_);
      }
    } 
    cout << setw(column_width+2) << column_label;
    for(int orbital_num2 = 0;orbital_num2<Hamiltonian.cols();++orbital_num2){
      cout << "| " << setw(column_width) << Hamiltonian_eff(orbital_num2,orbital_num2) << " ";
    }
    cout << "|" << endl; 
  }
 
}

bool TransferComplex::orbitalValid_(const std::pair<std::string, int> & orbital) const{
  if(orbital.first.compare("HOMO")==0){
    if(orbital.second>0){
      cerr << "HOMO orbital number is not negative or 0" << endl;
      return false;  
    }
    if(orbital.second<=(-1*HOMO_Orb_)){
      cerr << "HOMO orbital does not exst " << orbital.second << endl;
      return false;
    }
  }else if(orbital.first.compare("LUMO")==0){
    if(orbital.second<0){
      cerr << "LUMO orbital number is not positive or 0" << endl; return false;  
    }
    if(orbital.second>(Hamiltonian_eff.rows()-LUMO_Orb_)){
      cerr << "LUMO orbital does not exst " << orbital.second << endl;
      return false;
    }
  }
  return true;
} 
// Split a matrix up into a list of smaller matrices. The matrix can be split
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
list<Eigen::MatrixXd> splitMatrixIntoList(
    const vector<int> &subMatrixDimension,
    const Eigen::MatrixXd mat,
    const string &ColRowSplit) {

  list<Eigen::MatrixXd> list_matrix;
  int num_sub_matrices = subMatrixDimension.size();
  if (ColRowSplit.compare("Columns") == 0) {
    int col = 0;
    for (int i = 0; i < num_sub_matrices; ++i) {
      Eigen::MatrixXd mat_new(mat.rows(), subMatrixDimension.at(i));
      for (auto k = 0; k < mat.rows(); ++k) {
        for (auto j = 0; j < subMatrixDimension.at(i); ++j) {
          mat_new(k,j) = mat(k, col + j);
        }
      }
      col += subMatrixDimension.at(i);
      list_matrix.push_back(mat_new);
    }
  } else if (ColRowSplit.compare("Rows") == 0) {
    int row = 0;
    for (auto i = 0; i < num_sub_matrices; ++i) {
      Eigen::MatrixXd mat_new(subMatrixDimension.at(i), mat.cols());
      for (auto k = 0; k < mat.cols(); ++k) {
        for (auto j = 0; j < subMatrixDimension.at(i); ++j) {
          mat_new(j,k) = mat(row + j, k);
        }
      }
      row += subMatrixDimension.at(i);
      list_matrix.push_back(mat_new);
    }
  } else {
    throw invalid_argument("Unrecognized string tag provided");
  }
  return list_matrix;
}

list<Eigen::MatrixXd> splitCoefsUpByAtoms(const vector<int> & basisFuncP, Eigen::MatrixXd Coefs,
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
                     list<Eigen::MatrixXd> &p_atom_mat_coef) {

  for (auto p : monBmatch) {
    if (p.first != p.second) {
      auto it = p_atom_mat_coef.begin();
      Eigen::MatrixXd temp = *(next(it, p.first));
      *(next(it, p.first)) = *(next(it, p.second));
      *(next(it, p.second)) = temp;
    }
  }
  for (auto p : monAmatch) {
    if (p.first != p.second) {
      auto it = p_atom_mat_coef.begin();
      Eigen::MatrixXd temp = *(next(it, p.first));
      *(next(it, p.first)) = *(next(it, p.second));
      *(next(it, p.second)) = temp;
    }
  }
  return;
}
void updateSwapLists(vector<pair<int, int>> &monAmatch,
                     list<Eigen::MatrixXd> &atom_mat_coef) {

  for (auto p : monAmatch) {
    if (p.first != p.second) {
      auto it = atom_mat_coef.begin();
      Eigen::MatrixXd & temp = *(next(it, p.first));
      *(next(it, p.first)) = *(next(it, p.second));
      *(next(it, p.second)) = temp;
    }
  }
  return;
}

Eigen::MatrixXd mergeListOfMatrices(list<Eigen::MatrixXd> &matrix_list, const int rows,
                            const int cols, const string &ColRowMerge) {

  Eigen::MatrixXd full_matrix(rows, cols);
  if (ColRowMerge.compare("Columns") == 0) {
    int col = 0;
    for (auto it = matrix_list.begin(); it != matrix_list.end(); ++it) {
      Eigen::MatrixXd mat = *it;
      cout << "Cycling columns to merge" << endl;
      cout << mat << endl;
      int row = 0;
      if (col > cols)
        throw runtime_error("Your new matrix is not large enough");
      for (auto i = 0; i < mat.rows(); ++i) {
        if (row > rows)
          throw runtime_error("Your new matrix is not large enough");
        for (auto j = 0; j < mat.cols(); ++j) {
          full_matrix(row,j+col) = mat(i, j);
        }
        ++row;
      }
      col += mat.cols();
    }
  } else if (ColRowMerge.compare("Rows") == 0) {
    int row = 0;
    for (auto it = matrix_list.begin(); it != matrix_list.end(); ++it) {
      Eigen::MatrixXd mat = *it;
      cout << "Cycling rows " << endl;
      int col = 0;
      if (row > rows)
        throw runtime_error("Your new matrix is not large enough");
      for (auto j = 0; j < mat.cols(); ++j) {
        if (col > cols)
          throw runtime_error("Your new matrix is not large enough");
        for (auto i = 0; i < mat.rows(); ++i) {
          full_matrix(row + i, col) = mat(i, j);
        }
        ++col;
      }
      row += mat.rows();
    }
  } else {
    throw invalid_argument("Unrecognized merge type for list of matrices");
  }
  cout << full_matrix << endl;
  return full_matrix;
}

// This function
Eigen::MatrixXd createNewMatrix(list<Eigen::MatrixXd> &p_atom_mat_coef, int rows, int cols,
                        const string &ColRow) {
  return mergeListOfMatrices(p_atom_mat_coef, rows, cols, ColRow);
}

// unscramble the coefficients
Eigen::MatrixXd unscramble_Coef(
    const std::vector<int> &matchDimerA,
    const std::vector<int> &matchDimerB,
    const std::vector<int> &basisFuncP, 
    const Eigen::MatrixXd & dimerCoef) {

  // Let's reduce the complexity of the problem by instead of working
  // with the basis functions lets just work with the atoms. We can do
  // this by treating all the basis functions associated with a single
  // atom as a block.

  list<Eigen::MatrixXd> p_atom_mat_coef =
      splitCoefsUpByAtoms(basisFuncP, dimerCoef, "Columns");

  // Place all of monomer A atom basis functions on the left side of the
  // matrix and all of B monomer atom basis functions on the right side
  // of the dimer matrix
  // First int is the col in the dimer the atom should be at
  // Second int is the col in the dimer the atom is presently at

  vector<pair<int, int>> monAmatch;
  for (unsigned i = 0; i < matchDimerA.size(); ++i) {
    pair<int, int> pr(i, matchDimerA.at(i));
    monAmatch.push_back(pr);
  }
  vector<pair<int, int>> monBmatch;
  for (unsigned i = 0; i < matchDimerB.size(); ++i) {
    pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i));
    monBmatch.push_back(pr);
  }
  cout << "Contents of list before swap" << endl;
  for( Eigen::MatrixXd & mat : p_atom_mat_coef ){
    cout << mat << "\n" << endl;
  }

  refreshSwapOrder(monBmatch, monAmatch);
  cout << "Contents of list before update" << endl;
  for( Eigen::MatrixXd & mat : p_atom_mat_coef ){
    cout << mat << "\n" << endl;
  }

  updateSwapLists(monBmatch, monAmatch, p_atom_mat_coef);
  cout << "Contents of list" << endl;
  for( Eigen::MatrixXd & mat : p_atom_mat_coef ){
    cout << mat << "\n" << endl;
  }

  Eigen::MatrixXd dimerCoef_new = createNewMatrix(
      p_atom_mat_coef, dimerCoef.rows(), dimerCoef.cols(), "Columns");

  return dimerCoef_new;
}

// unscramble the coefficients
Eigen::MatrixXd unscramble_Coef(
    const std::vector<int> &matchDimerB,
    const std::vector<int> &basisFuncB,
    const Eigen::MatrixXd &Coef) {

  // Let's reduce the complexity of the problem by instead of working
  // with the basis functions lets just work with the atoms. We can do
  // this by treating all the basis functions associated with a single
  // atom as a block.

  list<Eigen::MatrixXd> atom_mat_coef =
      splitCoefsUpByAtoms(basisFuncB, Coef, "Columns");

  // Place all of monomer A atom basis functions on the left side of the
  // matrix and all of B monomer atom basis functions on the right side
  // of the  matrix
  // First int is the col in the dimer the atom should be at
  // Second int is the col in the dimer the atom is presently at

  vector<pair<int, int>> monBmatch;
  for (unsigned i = 0; i < matchDimerB.size(); ++i) {
    pair<int, int> pr(i, matchDimerB.at(i));
    monBmatch.push_back(pr);
  }

  refreshSwapOrder(monBmatch);
  updateSwapLists(monBmatch, atom_mat_coef);
  Eigen::MatrixXd Coef_new = createNewMatrix(atom_mat_coef, Coef.rows(),
                                     Coef.cols(), "Columns");
  return Coef_new;
}

// Similar to the above function but we will be moving both the rows
// and columns
Eigen::MatrixXd unscramble_S(
    const std::vector<int> &matchDimerA,
    const std::vector<int> &matchDimerB,
    const std::vector<int> &basisFuncP, 
    Eigen::MatrixXd S) {

  Eigen::MatrixXd S_new(S.rows(),S.cols());
  {
    list<Eigen::MatrixXd> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Columns");
    cout << "List of split matrices" << endl;
    for( Eigen::MatrixXd & mat : p_atom_mat_S ){
      cout << mat << "\n" << endl;
    }
    vector<pair<int, int>> monAmatch;
    for (unsigned i = 0; i < matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i));
      monAmatch.push_back(pr);
    }
    vector<pair<int, int>> monBmatch;
    for (unsigned i = 0; i < matchDimerB.size(); ++i) {
      pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i));
      monBmatch.push_back(pr);
    }

    refreshSwapOrder(monBmatch, monAmatch);
    updateSwapLists(monBmatch, monAmatch, p_atom_mat_S);
    S_new =
        createNewMatrix(p_atom_mat_S, S.rows(), S.cols(), "Columns");
  }

  S = S_new;

  {
    list<Eigen::MatrixXd> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Rows");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 0; i < matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i));
      monAmatch.push_back(pr);
    }
    vector<pair<int, int>> monBmatch;
    for (unsigned i = 0; i < matchDimerB.size(); ++i) {
      pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i));
      monBmatch.push_back(pr);
    }

    refreshSwapOrder(monBmatch, monAmatch);
    updateSwapLists(monBmatch, monAmatch, p_atom_mat_S);
    S_new = createNewMatrix(p_atom_mat_S, S.rows(), S.cols(), "Rows");
  }

  // Return the correctly swapped dimerCoef
  return S_new;
}

// Same as the above function but here we are assuming counterpoise correction
// is being used and thus we do not need to match with both monomer A and
// monomer B but only need to match with A.
Eigen::MatrixXd unscramble_S(
    const std::vector<int> &matchDimerA,
    const std::vector<int> &basisFuncP,
    Eigen::MatrixXd S) {

  Eigen::MatrixXd S_new(S.rows(),S.cols());
  {
    list<Eigen::MatrixXd> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Columns");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 0; i < matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i));
      monAmatch.push_back(pr);
    }
    refreshSwapOrder(monAmatch);
    updateSwapLists(monAmatch, p_atom_mat_S);
    S_new =
        createNewMatrix(p_atom_mat_S, S.rows(), S.cols(), "Columns");
  }

  S = S_new;

  {
    list<Eigen::MatrixXd> p_atom_mat_S = splitCoefsUpByAtoms(basisFuncP, S, "Rows");

    vector<pair<int, int>> monAmatch;
    for (unsigned i = 0; i < matchDimerA.size(); ++i) {
      pair<int, int> pr(i, matchDimerA.at(i));
      monAmatch.push_back(pr);
    }

    refreshSwapOrder(monAmatch);
    updateSwapLists(monAmatch, p_atom_mat_S);
    S_new = createNewMatrix(p_atom_mat_S, S.rows(), S.cols(), "Rows");
  }

  // Return the correctly swapped dimerCoef
  return S_new;
}

TransferComplex::TransferComplex(Eigen::MatrixXd mat1Coef, Eigen::MatrixXd mat2Coef,
                                 Eigen::MatrixXd matPCoef, std::pair<int, int> MOs1,
                                 std::pair<int, int> MOs2, Eigen::MatrixXd matS,
                                 Eigen::VectorXd vecPOE, bool cp) {

  unscrambled_ = false;
  counterPoise_ = cp;
  // Consistency check
  if (matS.cols() != matPCoef.cols()) {
    throw invalid_argument(
        "The overlap matrix must have the same number "
        "of basis functions as the dimer");
  }
  if (cp) {
    if (mat1Coef.cols() != matPCoef.cols()) {
      throw invalid_argument(
          "Counter poise correction requires that the"
          " monomers have the same number of coefficients as the dimer. "
          "Your monomer 1 does not");
    }
    if (mat2Coef.cols() != matPCoef.cols()) {
      throw invalid_argument(
          "Counter poise correction requires that the"
          " monomers have the same number of coefficients as the dimer. "
          "Your monomer 2 does not");
    }
  } else {
    int total_cols = mat1Coef.cols() + mat2Coef.cols();

    if (total_cols > matPCoef.cols()) {
      throw invalid_argument(
          "Counter poise has not been specified and your "
          "monomers have more basis function cols than your dimer");
    }
  }

  mat_1_Coef.resize(mat1Coef.rows(),mat1Coef.cols());
  mat_1_Coef = mat1Coef;
  mat_2_Coef.resize(mat2Coef.rows(),mat2Coef.cols());
  mat_2_Coef = mat2Coef;
  mat_P_Coef.resize(matPCoef.rows(),matPCoef.cols());
  mat_P_Coef = matPCoef;
  Orbs1 = MOs1;
  Orbs2 = MOs2;
  mat_S.resize(matS.rows(),matS.cols());
  mat_S = matS;
  vec_P_OE.resize(vecPOE.size());
  vec_P_OE = vecPOE;
}

void TransferComplex::unscramble(const Eigen::MatrixXd &coord_1_mat,
                                 const Eigen::MatrixXd &coord_2_mat,
                                 const Eigen::MatrixXd &coord_P_mat,
                                 const std::vector<int> &basisP,
                                 const std::vector<int> &basis2) {

  unscrambled_ = true;

  const int sig_fig = 4;

  // If dealing with counter poise correction may also need to unscramble
  // the basis functions of the monomers
  if (counterPoise_) {
    vector<int> match_1_2 = matchCol(coord_1_mat,coord_2_mat, sig_fig);

    LOG("Counter Poise unscrambling matrix 2 with respect to matrix 1", 2);
    Eigen::MatrixXd unscrambled_2_Coef = unscramble_Coef(match_1_2, basis2, mat_2_Coef);

    this->mat_2_Coef = unscrambled_2_Coef;

    vector<int> match_1_P = matchCol(coord_1_mat,coord_P_mat, sig_fig);

    Eigen::MatrixXd unscrambled_P_Coef = unscramble_Coef(match_1_P, basisP, mat_P_Coef);

    this->mat_P_Coef = unscrambled_P_Coef;

    auto unscrambled_S = unscramble_S(match_1_P, basisP, mat_S);

    this->mat_S = unscrambled_S;

  } else {

    // Stores the rows in P that match 1
    vector<int> match_1_P = matchCol(coord_1_mat,coord_P_mat, sig_fig);

    // Stores the rows in P that match 2
    vector<int> match_2_P = matchCol(coord_2_mat,coord_P_mat, sig_fig);

    LOG("Unscrambling dimer matrix with respect to matrix 1 and 2", 2);
    Eigen::MatrixXd unscrambled_P_Coef =
        unscramble_Coef(match_1_P, match_2_P, basisP, mat_P_Coef);

    this->mat_P_Coef = unscrambled_P_Coef;

    auto unscrambled_S = unscramble_S(match_1_P, match_2_P, basisP, mat_S);

    this->mat_S = unscrambled_S;
  }
}

void TransferComplex::calcJ() {

  if (unscrambled_ == false) {
    cerr << "WARNING unable to automatically line up basis functions of"
            " monomers with dimers, you better make sure they correctly"
            " line up or run the calculations again with the correct "
            "flag pop=full"
         << endl;
  }

  calculate_transfer_integral_();
}

}  // namespace catnip
