
// Local private includes
#include "transfer_complex.hpp"

#include "atom.hpp"
#include "atom_group.hpp"
#include "basis_map.hpp"
#include "swap_engine.hpp"

namespace catnip {

  void TransferComplex::unscramble(AtomSystem atom_sys) {
    // We should figure out how all the rows and columns should be
    // rearranged before moving any of the coefficients in the matrix    
    BasisMap basis_map = BasisMap(std::move(atom_sys));
  
    size_t total_basis = atom_sys.getTotalBasisFunctions(GroupType::Component); 
    size_t total_basis_comp = atom_sys.getTotalBasisFunctions(GroupType::Complex); 

    // If the total number of basis functions in the components is greater than
    // the total number of basis functions in the complex, throw an error
    if(total_basis != total_basis_comp) {
      throw std::runtime_error("Sum of basis functions in components does not equal the number of basis functions in the complex");
    }

    auto swap_eng = SwapEngine(basis_map);

    //swap_eng.arrange(*(params_->complex_coefs));
    //swap_eng.arrange(*(params_->S_AO));

//    auto dimension = mat_1_Coef.cols()+mat_2_Coef.cols();
//    Eigen::MatrixXd zetaA(mat_1_Coef.rows(),dimension);
//    Eigen::MatrixXd zetaB(mat_2_Coef.rows(),dimension);
//    if (params_->counter_poise) {
//
//    } else {
//
//    }
  }

  void TransferComplex::calcJ() {};
//  using namespace std;
/*
// Essentially calculates the transfer integral
void TransferComplex::calculate_transfer_integral_() {

  auto dimension = mat_1_Coef.cols()+mat_2_Coef.cols();
  Eigen::MatrixXd zetaA(mat_1_Coef.rows(),dimension);
  Eigen::MatrixXd zetaB(mat_2_Coef.rows(),dimension);
  if (counterPoise_) {

    LOG("Creating zeta matrices from coefficients assuming counterpoise", 2);
    zetaA = mat_1_Coef;
    zetaB = mat_2_Coef;
  } else {
    LOG("Creating zeta matrices from coefficients", 2);
    zetaA << mat_1_Coef, Eigen::MatrixXd::Zero(mat_1_Coef.rows(),mat_2_Coef.cols()); 
    zetaB << Eigen::MatrixXd::Zero(mat_2_Coef.rows(),mat_1_Coef.cols()), mat_2_Coef; 
  }

  LOG("Creating gamma and beta matrices", 2);
  Eigen::MatrixXd gammaA = zetaA * mat_S * mat_P_Coef.transpose(); 
  Eigen::MatrixXd gammaB = zetaB * mat_S * mat_P_Coef.transpose(); 

  assert(gammaA.cols() == gammaB.cols() && "Column count between gamma A and B must be consistent");
  Eigen::MatrixXd gamma(gammaA.rows()+gammaB.rows(),gammaA.cols());
  gamma << gammaA, gammaB;

  LOG("Calculating S_MO", 2);
  S_MO.resize(dimension,dimension);
  S_MO = gamma * gamma.transpose();
  
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigen_solver(S_MO);
  Eigen::MatrixXd S_MO_inv_sqrt = eigen_solver.operatorInverseSqrt();

  Hamiltonian.resize(mat_S.rows(),mat_S.cols());
  Hamiltonian = gamma * vec_P_OE.asDiagonal() * gamma.transpose();

  Hamiltonian_eff = S_MO_inv_sqrt * Hamiltonian * S_MO_inv_sqrt;  
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
    if (MO_A <= (-1 * HOMO_A_)) {
      string err = "You are trying to access HOMO" + to_string(MO_A) +
                   " but there "
                   "are only " +
                   to_string(HOMO_A_) + " HOMO orbitals";
      throw invalid_argument(err);
    }
  } else if (HOMO_OR_LUMO_A.compare("LUMO") == 0) {
    if (MO_A < 0) {
      throw invalid_argument(
          "Having specified LUMO the MO_A"
          " value is in reference to the LUMO and must be a positive number");
    }
    int allowed_LUMO = mat_1_Coef.rows() - HOMO_A_;
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
    if (MO_B <= (-1 * HOMO_B_)) {
      string err = "You are trying to access HOMO" + to_string(MO_B) +
                   " but there "
                   "are only " +
                   to_string(HOMO_B_) + " HOMO orbitals";
      throw invalid_argument(err);
    }
  } else if (HOMO_OR_LUMO_B.compare("LUMO") == 0) {
    if (MO_B < 0) {
      throw invalid_argument(
          "Having specified LUMO the MO_B"
          " value is in reference to the LUMO and must be a positive number");
    }
    int allowed_LUMO = mat_2_Coef.rows() - HOMO_B_;
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

  int offset = mat_1_Coef.rows();
  int orbital1_num = 0;
  int orbital2_num = 0;
  if(orbital1.first.compare("HOMO")==0){
    assert(orbitalValid_(orbital1)==true); 
    orbital1_num = orbital1.second + HOMO_A_-1;
  }else if(orbital1.first.compare("LUMO")==0){
    assert(orbitalValid_(orbital1)==true); 
    orbital1_num = orbital1.second + HOMO_A_;
  }
  if(orbital2.first.compare("HOMO")==0){
    assert(orbitalValid_(orbital2)==true); 
    orbital2_num = offset+orbital2.second + HOMO_B_-1;
  }else if(orbital2.first.compare("LUMO")==0){
    assert(orbitalValid_(orbital2)==true); 
    orbital2_num = offset+orbital2.second + HOMO_B_;
  }
  double J_ab = Hamiltonian(orbital1_num,orbital2_num);
  double e_a = Hamiltonian(orbital1_num,orbital1_num);
  double e_b = Hamiltonian(orbital2_num,orbital2_num);
  double S_ab = S_MO(orbital1_num,orbital2_num); 
 
  cout << "\nPre-Orthonormalization" << endl;
  cout << "J_ab             " << J_ab * hartreeToeV << " eV\n";
  cout << "e_a              " << e_a * hartreeToeV << " eV\n";
  cout << "e_b              " << e_b * hartreeToeV << " eV\n";
  cout << "S_ab              " << S_ab << "\n" << endl;

  cout << "Single Orbital DIPRO" << endl;
  double J_ab_single = (J_ab - 0.5*(e_a+e_b)*S_ab)/(1-pow(S_ab,2.0));
  double e_a_single = 0.5*((e_a+e_b)-2*J_ab*S_ab+(e_a-e_b)*pow(1-pow(S_ab,2.0),0.5))/(1-pow(S_ab,2.0));
  double e_b_single = 0.5*((e_a+e_b)-2*J_ab*S_ab-(e_a-e_b)*pow(1-pow(S_ab,2.0),0.5))/(1-pow(S_ab,2.0));
  cout << "J_ab_eff_single  " << J_ab_single * hartreeToeV << " eV\n";
  cout << "e_a_eff_single   " << e_a_single * hartreeToeV << " eV\n";
  cout << "e_b_eff_single   " << e_b_single * hartreeToeV << " eV\n" << endl;
  
  cout << "All Orbital DIPRO" << endl;
  double J_eff = Hamiltonian_eff(orbital1_num,orbital2_num);
  double e_a_eff = Hamiltonian_eff(orbital1_num,orbital1_num);
  double e_b_eff = Hamiltonian_eff(orbital2_num,orbital2_num);
  cout << "J_ab_eff_all    " << J_eff * hartreeToeV << " eV\n";
  cout << "e_a_eff_all     " << e_a_eff * hartreeToeV << " eV\n";
  cout << "e_b_eff_all     " << e_b_eff * hartreeToeV << " eV\n" << endl;

}

void TransferComplex::printAll() const {

  int column_width = 14;
  cout << "Effective Hamiltonian" << endl;

  int col_offset = mat_1_Coef.cols();
  int start_col = 0;
  int end_col = 5;
  while(start_col<Hamiltonian.cols()){
  
    cout << setw(column_width-2) << left << ""; 
    if(end_col>Hamiltonian.cols()){
      end_col = Hamiltonian.cols();
    }
    for(int orbital_num = start_col;orbital_num<end_col;++orbital_num){
      string column_label = "";
      if(orbital_num<HOMO_A_ || (orbital_num>=col_offset && orbital_num<(HOMO_B_+col_offset))){
        if(orbital_num<col_offset){
          column_label=string("A HOMO");
        }else{
          column_label=string("B HOMO");
        }
        if(orbital_num!=(HOMO_A_-1) && orbital_num!=(col_offset+HOMO_B_-1)){
          if(orbital_num<col_offset){
            column_label+=to_string(orbital_num+1-HOMO_A_);
          }else {
            column_label+=to_string(orbital_num+1-col_offset-HOMO_B_);
          }
        }
      }else {
        if((orbital_num>=HOMO_A_ && orbital_num<col_offset) || orbital_num>(col_offset+HOMO_B_)){
          if(orbital_num<col_offset){
            column_label=string("A LUMO");
          }else{
            column_label=string("B LUMO");
          }
          if(orbital_num!=(HOMO_A_) && orbital_num!=(col_offset+HOMO_B_)){
            if(orbital_num<col_offset){
              column_label+=string("+")+to_string(orbital_num-HOMO_A_);
            }else{
              column_label+=string("+")+to_string(orbital_num-col_offset-HOMO_B_);
            }
          }
        }
      } 
      cout << " | " << setw(column_width) << right << column_label;
    }
    cout << " |" << endl; 

    for(int orbital_num = 0;orbital_num<Hamiltonian.rows();++orbital_num){
      string column_label = "";
      if(orbital_num<HOMO_A_ || (orbital_num>=col_offset && orbital_num<(HOMO_B_+col_offset))){
        if(orbital_num<col_offset){
          column_label=string("A HOMO");
        }else{
          column_label=string("B HOMO");
        }
        if(orbital_num!=(HOMO_A_-1) && orbital_num!=(col_offset+HOMO_B_-1)){
          if(orbital_num<col_offset){
            column_label+=to_string(orbital_num+1-HOMO_A_);
          }else {
            column_label+=to_string(orbital_num+1-col_offset-HOMO_B_);
          }
        }
      }else {
        if(orbital_num < col_offset){
          column_label=string("A LUMO");
        }else{
          column_label=string("B LUMO");
        }
        if(orbital_num!=(HOMO_A_) && orbital_num!=(col_offset+HOMO_B_)){
          if(orbital_num<col_offset){
            column_label+=string("+")+to_string(orbital_num-HOMO_A_);
          }else{
            column_label+=string("+")+to_string(orbital_num-col_offset-HOMO_B_);
          }
        }
      } 
      cout << " " << setw(column_width-3) << left << column_label << " ";
      for(int orbital_num2 = start_col;orbital_num2<end_col;++orbital_num2){
        cout << "| "<< right  << scientific << std::setprecision(6)  << setw(column_width) << Hamiltonian_eff(orbital_num,orbital_num2) << " ";
      }
      cout << "|" << endl; 
    }

    start_col+=5;
    end_col+=5;
    cout << endl;
  } 
}

bool TransferComplex::orbitalValid_(const std::pair<std::string, int> & orbital) const{
  int HOMO_AB = HOMO_A_+HOMO_B_;
  int LUMO_AB = HOMO_AB+1;
  if(orbital.first.compare("HOMO")==0){
    if(orbital.second>0){
      cerr << "HOMO orbital number is not negative or 0" << endl;
      return false;  
    }
    if(orbital.second>HOMO_AB){
      cerr << "HOMO orbital does not exst " << orbital.second << " "<< HOMO_AB << endl;
      return false;
    }
  }else if(orbital.first.compare("LUMO")==0){
    if(orbital.second<0){
      cerr << "LUMO orbital number is not positive or 0" << endl; return false;  
    }
    if(orbital.second>(Hamiltonian_eff.rows()-LUMO_AB)){
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
// index 1 | row 1
// index 2 | row 2
// index 3 | row 3
// index 4 | row 4
//
// where:
// Row 1 needs to be at index 3
// Row 4 needs to be at index 1
// Row 2 needs to be at index 2
// Row 3 needs to be at index 4
//
// If we simply looked at our first instance of the matrix shown above
// and came up with the following swaps
// Swap row 1 with index 3
// Swap row 4 with index 1
// Swap row 2 with index 2
// Swap row 3 with index 4
//
// First swap
// index 1 row3
// index 2 row2
// index 3 row1
// index 4 row4
//
// Second swap
// index 1 row4
// index 2 row2
// index 3 row1
// index 4 row3
//
// Third swap does nothing
// Fourth swap
// index 1 row4
// index 2 row2
// index 3 row3
// index 4 row1
//
// When what we really wanted was
// index 1 row4
// index 2 row2
// index 3 row1
// index 4 row3
//
// Notice the row3 and row1 are in the wrong place
// The instructions should be rewritten
// Swap row 1 with index 3
// Swap row 4 with index 3
// Swap row 2 with index 2 - no swap
// Swap row 3 with index 3 - no swap
//
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

// To replace both refreshSwapOrder and updateSwapLists
// instead of figuring out a proper order for swapping we will simply start
// with a new link list of the same size and place the coefficents in that
// new list
Eigen::MatrixXd createSortedCoefMatrix(vector<pair<int,int>> & mon_match_ind, list<Eigen::MatrixXd> & atom_mat_coefs){

  // Actually lets use a vector of pointers to the matrices that are in the list
  vector<Eigen::MatrixXd *> mat_ptrs;
  // Error checking
  if(atom_mat_coefs.size()==0){
    throw std::runtime_error("ERROR cannot create sorted coef matrix, there are atomically partitioned matrices to build the full sorted coef matrix from.");
  } 

  int cols = atom_mat_coefs.front().cols();
  for ( list<Eigen::MatrixXd>::iterator coef_ptr = atom_mat_coefs.begin();
        coef_ptr != atom_mat_coefs.end();
        ++coef_ptr ){
      if(coef_ptr->cols()!=cols){
        throw std::runtime_error("ERROR cannot created sorted coef matrix from atomically partitioned matrices with varying number of columns.");
      }
  } 
  // End of error checking
 
  // Count the total number of rows in each atomically partitioned coef matrix 
  vector<int> rows_per_mat; 
  for ( list<Eigen::MatrixXd>::iterator coef_ptr = atom_mat_coefs.begin();
      coef_ptr != atom_mat_coefs.end();
      ++coef_ptr ){
    rows_per_mat.push_back(coef_ptr->rows());
  } 
 
  int initial_value = 0;
  int total_rows = std::accumulate(rows_per_mat.begin(),rows_per_mat.end(), initial_value);

  // Determine the offset of each row to each index
  std::map<int,int> row_to_index_offset; 
  int offset = 0;
  for ( std::pair<int,int> & row_ind : mon_match_ind ){
    row_to_index_offset.at(row_ind.first) = offset; 
    offset+= rows_per_mat.at(row_ind.second);
  }

  Eigen::MatrixXd sorted_atom_mat_coefs
   
   

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
  return full_matrix;
}

// This function
Eigen::MatrixXd createNewMatrix(list<Eigen::MatrixXd> &p_atom_mat_coef, int rows, int cols,
                        const string &ColRow) {
  return mergeListOfMatrices(p_atom_mat_coef, rows, cols, ColRow);
}

// unscramble the coefficients
//Eigen::MatrixXd unscramble_Coef(
//    const std::vector<int> &matchDimerA,
//    const std::vector<int> &matchDimerB,
//    const std::vector<int> &basisFuncP, 
//    const Eigen::MatrixXd & dimerCoef) {
//
//  // Let's reduce the complexity of the problem by instead of working
//  // with the basis functions lets just work with the atoms. We can do
//  // this by treating all the basis functions associated with a single
//  // atom as a block.
//
//  list<Eigen::MatrixXd> p_atom_mat_coef =
//      splitCoefsUpByAtoms(basisFuncP, dimerCoef, "Columns");
//
//  // Place all of monomer A atom basis functions on the left side of the
//  // matrix and all of B monomer atom basis functions on the right side
//  // of the dimer matrix
//  // First int is the col in the dimer the atom should be at
//  // Second int is the col in the dimer the atom is presently at
//
//  vector<pair<int, int>> monAmatch;
//  for (unsigned i = 0; i < matchDimerA.size(); ++i) {
//    pair<int, int> pr(i, matchDimerA.at(i));
//    monAmatch.push_back(pr);
//  }
//  vector<pair<int, int>> monBmatch;
//  for (unsigned i = 0; i < matchDimerB.size(); ++i) {
//    pair<int, int> pr(i + monAmatch.size(), matchDimerB.at(i));
//    monBmatch.push_back(pr);
//  }
//
//  refreshSwapOrder(monBmatch, monAmatch);
//  updateSwapLists(monBmatch, monAmatch, p_atom_mat_coef);
//
//  Eigen::MatrixXd dimerCoef_new = createNewMatrix(
//      p_atom_mat_coef, dimerCoef.rows(), dimerCoef.cols(), "Columns");
//
//  return dimerCoef_new;
//}
//
//// unscramble the coefficients
//Eigen::MatrixXd unscramble_Coef(
//    const std::vector<int> &matchDimerB,
//    const std::vector<int> &basisFuncB,
//    const Eigen::MatrixXd &Coef) {
//
//  // Let's reduce the complexity of the problem by instead of working
//  // with the basis functions lets just work with the atoms. We can do
//  // this by treating all the basis functions associated with a single
//  // atom as a block.
//
//  list<Eigen::MatrixXd> atom_mat_coef =
//      splitCoefsUpByAtoms(basisFuncB, Coef, "Columns");
//
//  // Place all of monomer A atom basis functions on the left side of the
//  // matrix and all of B monomer atom basis functions on the right side
//  // of the  matrix
//  // First int is the col in the dimer the atom should be at
//  // Second int is the col in the dimer the atom is presently at
//
//  vector<pair<int, int>> monBmatch;
//  for (unsigned i = 0; i < matchDimerB.size(); ++i) {
//    pair<int, int> pr(i, matchDimerB.at(i));
//    monBmatch.push_back(pr);
//  }
//
//  refreshSwapOrder(monBmatch);
//  updateSwapLists(monBmatch, atom_mat_coef);
//  Eigen::MatrixXd Coef_new = createNewMatrix(atom_mat_coef, Coef.rows(),
//                                     Coef.cols(), "Columns");
//  return Coef_new;
//}

Eigen::MatrixXd unscrambleCoef(
    const std::vector<std::vector<int>> &match_mon_complex,
    const std::vector<int> &basis_functions_complex, 
    const Eigen::MatrixXd & complex_coefs) {

  // Let's reduce the complexity of the problem by instead of working
  // with the basis functions lets just work with the atoms. We can do
  // this by treating all the basis functions associated with a single
  // atom as a block.

  list<Eigen::MatrixXd> complex_coef_block =
      splitCoefsUpByAtoms(basis_functions_complex, complex_coefs, "Columns");

  // Place all of monomer A atom basis functions on the left side of the
  // matrix and all of B monomer atom basis functions on the right side
  // of the dimer matrix
  // First int is the col in the dimer the atom should be at
  // Second int is the col in the dimer the atom is presently at

//  vector<pair<int, int>> monBmatch;
//  for (unsigned i = 0; i < matchDimerB.size(); ++i) {
//    pair<int, int> pr(i, matchDimerB.at(i));
//    monBmatch.push_back(pr);
//  }

  vector<vector<pair<int,int>>> mon_match_index;
  unsigned new_index = 0;
  for ( const vector<int> & monomer_rows : match_mon_complex){
    vector<pair<int,int>> mon_matching_index_row;
    for( const int & row_index : monomer_rows ){
      pair<int,int> pr(new_index,row_index);
      mon_matching_index_row.push_back(pr); 
    } 
    mon_match_index.push_back(mon_matching_index_row);
  }

  //refreshSwapOrder(monBmatch);
//  refreshSwapOrder(mon_match_index);
 // updateSwapLists(monBmatch, atom_mat_coef);
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


//TransferComplex::TransferComplex(
//    const Eigen::MatrixXd & mat1Coef, 
//    const Eigen::MatrixXd & mat2Coef,
//    const Eigen::MatrixXd & matPCoef, 
//    const int HOMO_A, 
//    const int HOMO_B, 
//    const Eigen::MatrixXd & matS,
//    const Eigen::VectorXd & vecPOE, bool cp) {
//
TransferComplex::TransferComplex(const Parameters params) : params_(params) { 

  unscrambled_ = false;
//  counterPoise_ = cp;
  // Consistency check
  if (params_.S_AO.cols() != params_.mat_complex.cols()) {
    throw invalid_argument(
        "The overlap matrix must have the same number "
        "of basis functions as the dimer");
  }
  //if (cp) {
  if (params_.counter_poise) {
    int monomer_number = 1;
    for ( const Eigen::MatrixXd & mat : params_.mat_monomers){
      if(params_.mat_complex.cols() != mat.cols()){
        throw invalid_argument(
            "Counter poise correction requires that the"
            " monomers have the same number of coefficients as the complex. "
            "Your monomer " + std::to_string(monomer_number) +" does not");
      }
      ++monomer_number;
    }
//    if (mat1Coef.cols() != matPCoef.cols()) {
//      throw invalid_argument(
//          "Counter poise correction requires that the"
//          " monomers have the same number of coefficients as the dimer. "
//          "Your monomer 1 does not");
//    }
//    if (mat2Coef.cols() != matPCoef.cols()) {
//      throw invalid_argument(
//          "Counter poise correction requires that the"
//          " monomers have the same number of coefficients as the dimer. "
//          "Your monomer 2 does not");
//    }
  } else {
    /// If not counter poise the sum of the cols should equal the same number as is in the complex
//    int total_cols = mat1Coef.cols() + mat2Coef.cols();
    int total_cols = 0; 
    for ( const Eigen::MatrixXd & mat : params_.mat_monomers){
      total_cols+=mat.cols();
    }
    
    //if (total_cols > matPCoef.cols()) {
    if (total_cols > params_.mat_complex.cols()) {
      throw invalid_argument(
          "Counter poise has not been specified and the total number of "
          "basis functions in your monomers is more than the number of basis "
          "functions in your complex.");
    }else if(total_cols < params_.complex_coefs.cols()){
      throw invalid_argument( 
          "Counter poise has not been specified and the total number of "
          "basis functions in your monomers is less than the number of basis "
          "functions in your complex.");
    }
  }
//
//  mat_1_Coef.resize(mat1Coef.rows(),mat1Coef.cols());
//  mat_1_Coef = mat1Coef;
//  mat_2_Coef.resize(mat2Coef.rows(),mat2Coef.cols());
//  mat_2_Coef = mat2Coef;
//  mat_P_Coef.resize(matPCoef.rows(),matPCoef.cols());
//  mat_P_Coef = matPCoef;
//  HOMO_A_ = HOMO_A;
//  HOMO_B_ = HOMO_B;
//  mat_S.resize(matS.rows(),matS.cols());
//  mat_S = matS;
//  vec_P_OE.resize(vecPOE.size());
//  vec_P_OE = vecPOE;
}

//void TransferComplex::unscramble(const Eigen::MatrixXd &coord_1_mat,
//                                 const Eigen::MatrixXd &coord_2_mat,
//                                 const Eigen::MatrixXd &coord_P_mat,
//                                 const std::vector<int> &basisP,
//                                 const std::vector<int> &basis2) {
//
void TransferComplex::unscramble(const vector<Eigen::MatrixXd> &coord_monomers,
                                 const Eigen::MatrixXd &coord_complex,
                                 const std::vector<int> &basis_complex,
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

    this->mat_P_Coef = unscramble_Coef(match_1_P, basisP, mat_P_Coef);

    this->mat_S = unscramble_S(match_1_P, basisP, mat_S);

  } else {

    vector<vector<int>> match_mon_complex;
    for ( const Eigen::MatrixXd & coord_mon : coord_monomers){
      // Returns each of the rows in complex that match each of the monomers
      match_mon_complex.push_back(matchCol(coord_mon,coord_complex, sig_fig));
    }
    // Stores the rows in P that match 2
    //vector<int> match_2_P = matchCol(coord_2_mat,coord_P_mat, sig_fig);

    LOG("Unscrambling dimer matrix with respect to matrix 1 and 2", 2);
    //this->mat_P_Coef =
    //    unscramble_Coef(match_1_P, match_2_P, basisP, mat_P_Coef);
    this->params_.complex_coefs = unscrambleCoef(match_mon_complex,basis_complex,complex_coefs);
//    this->mat_S = unscramble_S(match_1_P, match_2_P, basisP, mat_S);
    }
  }
}

void TransferComplex::calcJ() {

  if (unscrambled_ == false) {
    cerr << "\nWARNING unable to automatically line up basis functions of"
            " monomers with dimers, you better make sure they correctly"
            " line up or run the calculations again with the correct "
            "flag pop=full"
         << endl;
  }

  calculate_transfer_integral_();
}
*/
}  // namespace catnip
