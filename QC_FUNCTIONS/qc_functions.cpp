#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <list>
#include <unordered_map>
#include "../IO/io.hpp"
#include "../MATRIX/matrix.hpp"
#include "../CONSTANTS/constants.hpp"

using namespace std;

unordered_map<int,pair<double,string>> findRank(Matrix & Orb_E_Alpha, Matrix & Orb_E_Beta){

  vector<pair<double,string>> all;
  
  auto m_a = Orb_E_Alpha.getCol(1);

  for(int ind = 1;ind<=m_a.get_rows();ind++){
    auto val = m_a.get_elem(ind);
    pair<double,string> pr(val, "Alpha");
    all.push_back(pr);
  }

  if(Orb_E_Beta.get_cols()>0){
    auto m_b = Orb_E_Beta.getCol(1);

    for(int ind = 1;ind<=m_b.get_rows();ind++){
      auto val = m_b.get_elem(ind);
      pair<double,string> pr(val,"Beta");
      all.push_back(pr);
    }
  }
  // Sort the vectors
  cerr << "Sorting pairs " << endl;
  sort(all.begin(),all.end(),[](const pair<double,string>& P1, const pair<double,string>& P2) -> bool 
  {
    return P1.first < P2.first;
  });
//  cerr << "Finished Sorting " << endl;
  // Now that they are sorted we are going to update the ranks
  int rank = 1;

  unordered_map<int,pair<double,string>> rank_map;

  for( auto pr : all ){
//    cerr << "Rank " << rank << " Energy " << pr.first << " Spin " << pr.second << endl;
    rank_map[rank] = pr;
    rank++;
  }
  return rank_map;
}


// Essentially calculates the transfer integral
double calculate_transfer_integral(
  Matrix mat_1_Coef,
  Matrix mat_2_Coef,
  Matrix mat_P_Coef,
  int MO1,
  int MO2,
  Matrix mat_S,
  Matrix mat_P_OE){

  Matrix mat_1_Coefinv = mat_1_Coef.invert();
  Matrix mat_2_Coefinv = mat_2_Coef.invert();
  Matrix mat_P_Coefinv = mat_P_Coef.invert();

  Matrix zerosA(MO2,mat_1_Coefinv.get_cols(),mat_1_Coefinv.get_shel());
  Matrix zerosB(MO1,mat_2_Coefinv.get_cols(),mat_2_Coefinv.get_shel());

  Matrix zetaA = Matrix_concatenate_rows( mat_1_Coefinv, zerosA );
  Matrix zetaB = Matrix_concatenate_rows( zerosB, mat_2_Coefinv );

  Matrix zetaAinv = zetaA.invert();
  Matrix zetaBinv = zetaB.invert();

  Matrix Inter = mat_S * mat_P_Coefinv;

  Matrix gammaA = zetaAinv * Inter ;
  Matrix gammaB = zetaBinv * Inter ;

  Matrix gammaA_inv = gammaA.invert();
  Matrix gammaB_inv = gammaB.invert();

  Matrix S_AB = gammaB * gammaA_inv;

  Matrix Energy = Matrix_diag( mat_P_OE );
  Matrix J_AB = gammaB * (Energy * gammaA_inv);

  Matrix e_B = gammaB * (Energy * gammaB_inv );
  Matrix e_A = gammaA * (Energy * gammaA_inv );

  double J_ab = J_AB.get_elem(1,1);
  double e_b = e_B.get_elem(1,1);
  double e_a = e_A.get_elem(1,1);
  double S_ab = S_AB.get_elem(1,1);

  double J_eff = (J_ab-1/((double)2)*(e_b+e_a)*S_ab);
  J_eff = J_eff/((double)(1-pow(S_ab,2)));


  cout << "J_ab " << J_ab*hartreeToeV << " eV\n";
  cout << "e_a " << e_b*hartreeToeV << " eV\n";
  cout << "e_b " << e_a*hartreeToeV << " eV\n";
  cout << "S_ab " << S_ab << "\n";
  cout << "J_eff " << J_eff*hartreeToeV << " eV\n";

  return J_eff;
}

// unscramble the coefficients
Matrix * unscramble_P_Coef(std::vector<int> matchDimerA, std::vector<int> matchDimerB,std::vector<int> basisFuncP,Matrix * dimerCoef){

  // Let's reduce the complexity of the problem by instead of working
  // with the basis functions lets just work with the atoms. We can do
  // this by treating all the basis functions associated with a single
  // atom as a block. 

  
  list<Matrix *> p_atom_mat_coef;
  int num_atoms = basisFuncP.size();
  int coef_col = 0;
  cerr << "Number of atoms " << num_atoms << endl;
  for(auto i=1; i<=num_atoms;++i){
    cerr << "Creating matrix r " << dimerCoef->get_rows() << " c " << basisFuncP.at(i-1) << endl;
    Matrix * mat = new Matrix(dimerCoef->get_rows(),basisFuncP.at(i-1));
    cerr << "New Matrix " << endl;
    for( auto k=1; k<=dimerCoef->get_rows();++k){
      for(auto j=1; j<=basisFuncP.at(i-1) ;++j){
        mat->set_elem(dimerCoef->get_elem(k,coef_col+j),k,j);
        cerr << dimerCoef->get_elem(k,coef_col+j) << " " ;
      }
      cerr << endl;
    }
    coef_col+=basisFuncP.at(i-1);
    p_atom_mat_coef.push_back(mat);
  }

  // Place all of monomer A atom basis functions on the left side of the
  // matrix and all of B monomer atom basis functions on the right side 
  // of the dimer matrix
  // First int is the col in the dimer the atom should be at
  // Second int is the col in the dimer the atom is presently at
  cerr << "Matching monomerA" << endl;
  vector<pair<int,int>> monAmatch;
  for(unsigned i=1;i<=matchDimerA.size();++i){
    pair<int,int> pr(i,matchDimerA.at(i-1));
    monAmatch.push_back(pr);
  }
  vector<pair<int,int>> monBmatch;
  for(unsigned i=1;i<=matchDimerB.size();++i){
    pair<int,int> pr(i+monAmatch.size(),matchDimerB.at(i-1));
    monBmatch.push_back(pr);
  }

  // Now if we use the match vectors we need to organize them so that if
  // we execute matching of the columns sequentially that they do not refer
  // to a column that has alread been moved 
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
  cerr << "monAmatch" << endl;
  for(auto it : monAmatch ) cerr << it.first << " " << it.second << endl;
 
  cerr << "Determining swap order sorting A " << endl; 
  for( auto pr_ptr=monAmatch.begin(); pr_ptr!=monAmatch.end(); ++pr_ptr ){
    auto pr = *pr_ptr;
    int row1was = pr.first;
    int row1is  = pr.second;
    // Find if "row1was" is moved later if it is the reference needs to be
    // changed to "row1is"
    cerr << "first " << row1was << " second " << row1is << endl;

    // Fixing the values in MonomerA
    auto pr_ptr_temp = pr_ptr;
    pr_ptr_temp++;
    while(pr_ptr_temp!=monAmatch.end()){
      pr_ptr_temp = find_if(pr_ptr_temp,monAmatch.end(),[row1was](const pair<int,int>& p){
        return row1was==p.second;});
      if(pr_ptr_temp!=monAmatch.end()){
        pr_ptr_temp->second = row1is;
      }
    } 

    // Fixing the values in MonomerB
    pr_ptr_temp = monBmatch.begin();
    while(pr_ptr_temp!=monBmatch.end()){
      pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[row1was](const pair<int,int>& p){
        return row1was==p.second;});
      if(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp->second = row1is;
      }
    }
  } 

  cerr << "Sorting through monomer B " << endl;

  for( auto pr_ptr=monBmatch.begin(); pr_ptr!=monBmatch.end(); ++pr_ptr ){
    auto pr = *pr_ptr;
    int row1was = pr.first;
    int row1is  = pr.second;
    // Find if "row1was" is moved later if it is the reference needs to be
    // changed to "row1is"
    cerr << "first " << row1was << " second " << row1is << endl;

    // Fixing the values in MonomerB
    auto pr_ptr_temp = pr_ptr;
    pr_ptr_temp++;
    while(pr_ptr_temp!=monBmatch.end()){
      pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[row1was](const pair<int,int>& p){
        return row1was==p.second;});
      if(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp->second = row1is;
      }
    }
  } 


  // Now we know that if we swap sequentially we will not be moving the wrong
  // columns
  for( auto p : monAmatch ){
    if(p.first!=p.second){
      cerr << "Swapping " << p.first << " with " << p.second << endl;
      auto it = p_atom_mat_coef.begin();  
      cerr << "Elements in matrix " << p.first << " Elements in matrix " << p.second << endl;
      Matrix * temp = *(next(it, p.first-1));
      Matrix * temp2 = *(next(it,p.second-1));
      for(int i=1;i<=temp->get_rows();++i){
        for(int j=1;j<=temp->get_cols();++j){
          cerr << temp->get_elem(i,j) << " ";
        }
        cerr << "     ";
        for(int j=1;j<=temp2->get_cols();++j){
          cerr << temp2->get_elem(i,j) << " ";
        }
        cerr << endl;
      }
      *(next(it,p.first-1)) = *(next(it,p.second-1));
      *(next(it,p.second-1)) = temp;
    } 
  }
  cerr << "B" << endl;
  for( auto p : monBmatch ){
    if(p.first!=p.second){
      cerr << "Swapping " << p.first << " with " << p.second << endl;
      auto it = p_atom_mat_coef.begin();  
      Matrix * temp = *(next(it, p.first-1));
      *(next(it,p.first-1)) = *(next(it,p.second-1));
      *(next(it,p.second-1)) = temp;
    } 
  }

  cerr << "New matrix r " << dimerCoef->get_rows() << " c " << dimerCoef->get_cols() << endl;
  
  Matrix * dimerCoef_new = new Matrix(dimerCoef->get_rows(),dimerCoef->get_cols());
  // Now we want to load the elements in the correct order into the matrix
  int col = 0;
  for( auto it=p_atom_mat_coef.begin();it!=p_atom_mat_coef.end();++it){
    Matrix * mat_ptr = *it;
    int row = 1;
    for( auto i=1;i<=mat_ptr->get_rows();++i){
      for( auto j=1;j<=mat_ptr->get_cols();++j){
        //cerr << i << " " << j << " " << row << " " << j+col << " ";
        cerr << mat_ptr->get_elem(i,j) << " " ;
        dimerCoef_new->set_elem(mat_ptr->get_elem(i,j),row,j+col);
      }
      cerr << endl;
      ++row;
    }
    col+=mat_ptr->get_cols();
    cerr << "next set of coefficients" << endl;
  } 
 
  // Return the correctly swapped dimerCoef
  return dimerCoef_new;
} 


// Similar to the above function but we will be moving both the rows
// and columns
Matrix * unscramble_S(std::vector<int> matchDimerA,
                  std::vector<int> matchDimerB,
                  std::vector<int> basisFuncP,
                  Matrix * S){

  Matrix * S_new = new Matrix(S->get_rows(),S->get_cols());
  {  
    list<Matrix *> p_atom_mat_S;
    int num_atoms = basisFuncP.size();
    int coef_col = 0;
    cerr << "Number of atoms " << num_atoms << endl;
    for(auto i=1; i<=num_atoms;++i){
      cerr << "Creating matrix r " << S->get_rows() << " c " << basisFuncP.at(i-1) << endl;
      Matrix * mat = new Matrix(S->get_rows(),basisFuncP.at(i-1));
      cerr << "New Matrix " << endl;
      for( auto k=1; k<=S->get_rows();++k){
        for(auto j=1; j<=basisFuncP.at(i-1) ;++j){
          mat->set_elem(S->get_elem(k,coef_col+j),k,j);
          cerr << S->get_elem(k,coef_col+j) << " " ;
        }
        cerr << endl;
      }
      coef_col+=basisFuncP.at(i-1);
      p_atom_mat_S.push_back(mat);
    }

    // Place all of monomer A atom basis functions on the left side of the
    // matrix and all of B monomer atom basis functions on the right side 
    // of the dimer matrix
    // First int is the col in the dimer the atom should be at
    // Second int is the col in the dimer the atom is presently at
    cerr << "Matching monomerA" << endl;
    vector<pair<int,int>> monAmatch;
    for(unsigned i=1;i<=matchDimerA.size();++i){
      pair<int,int> pr(i,matchDimerA.at(i-1));
      monAmatch.push_back(pr);
    }
    vector<pair<int,int>> monBmatch;
    for(unsigned i=1;i<=matchDimerB.size();++i){
      pair<int,int> pr(i+monAmatch.size(),matchDimerB.at(i-1));
      monBmatch.push_back(pr);
    }

    cerr << "monAmatch" << endl;
    for(auto it : monAmatch ) cerr << it.first << " " << it.second << endl;

    cerr << "Determining swap order sorting A " << endl; 
    for( auto pr_ptr=monAmatch.begin(); pr_ptr!=monAmatch.end(); ++pr_ptr ){
      auto pr = *pr_ptr;
      int row1was = pr.first;
      int row1is  = pr.second;
      // Find if "row1was" is moved later if it is the reference needs to be
      // changed to "row1is"
      cerr << "first " << row1was << " second " << row1is << endl;

      // Fixing the values in MonomerA
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while(pr_ptr_temp!=monAmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monAmatch.end(),[row1was](const pair<int,int>& p){
            return row1was==p.second;});
        if(pr_ptr_temp!=monAmatch.end()){
          pr_ptr_temp->second = row1is;
        }
      } 
      // Fixing the values in MonomerB
      pr_ptr_temp = monBmatch.begin();
      while(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[row1was](const pair<int,int>& p){
            return row1was==p.second;});
        if(pr_ptr_temp!=monBmatch.end()){
          pr_ptr_temp->second = row1is;
        }
      }

    } 

    cerr << "Sorting through monomer B " << endl;

    for( auto pr_ptr=monBmatch.begin(); pr_ptr!=monBmatch.end(); ++pr_ptr ){
      auto pr = *pr_ptr;
      int row1was = pr.first;
      int row1is  = pr.second;
      // Find if "row1was" is moved later if it is the reference needs to be
      // changed to "row1is"
      cerr << "first " << row1was << " second " << row1is << endl;

      // Fixing the values in MonomerB
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[row1was](const pair<int,int>& p){
            return row1was==p.second;});
        if(pr_ptr_temp!=monBmatch.end()){
          pr_ptr_temp->second = row1is;
        }
      }
    } 

    // Now we know that if we swap sequentially we will not be moving the wrong
    // columns
    for( auto p : monAmatch ){
      if(p.first!=p.second){
        cerr << "Swapping " << p.first << " with " << p.second << endl;
        auto it = p_atom_mat_S.begin();  
        cerr << "Elements in matrix " << p.first << " Elements in matrix " << p.second << endl;
        Matrix * temp = *(next(it, p.first-1));
        Matrix * temp2 = *(next(it,p.second-1));
        for(int i=1;i<=temp->get_rows();++i){
          for(int j=1;j<=temp->get_cols();++j){
            cerr << temp->get_elem(i,j) << " ";
          }
          cerr << "     ";
          for(int j=1;j<=temp2->get_cols();++j){
            cerr << temp2->get_elem(i,j) << " ";
          }
          cerr << endl;
        }
        *(next(it,p.first-1)) = *(next(it,p.second-1));
        *(next(it,p.second-1)) = temp;
      } 
    }
    cerr << "B" << endl;
    for( auto p : monBmatch ){
      if(p.first!=p.second){
        cerr << "Swapping " << p.first << " with " << p.second << endl;
        auto it = p_atom_mat_S.begin();  
        Matrix * temp = *(next(it, p.first-1));
        *(next(it,p.first-1)) = *(next(it,p.second-1));
        *(next(it,p.second-1)) = temp;
      } 
    }

    cerr << "New matrix r " << S->get_rows() << " c " << S->get_cols() << endl;

    // Now we want to load the elements in the correct order into the matrix
    int col = 0;
    for( auto it=p_atom_mat_S.begin();it!=p_atom_mat_S.end();++it){
      Matrix * mat_ptr = *it;
      int row = 1;
      for( auto i=1;i<=mat_ptr->get_rows();++i){
        for( auto j=1;j<=mat_ptr->get_cols();++j){
          //cerr << i << " " << j << " " << row << " " << j+col << " ";
          cerr << mat_ptr->get_elem(i,j) << " " ;
          S_new->set_elem(mat_ptr->get_elem(i,j),row,j+col);
        }
        cerr << endl;
        ++row;
      }
      col+=mat_ptr->get_cols();
      cerr << "next set of coefficients" << endl;
    } 
  }

  cerr << "Matrix after swapping columns" << endl;
  cerr << *S_new << endl;
  S = S_new;

  {  
    list<Matrix *> p_atom_mat_S;
    int num_atoms = basisFuncP.size();
    int coef_row = 0;
    cerr << "Number of atoms " << num_atoms << endl;
    for(auto i=1; i<=num_atoms;++i){
      cerr << "Creating matrix r " << basisFuncP.at(i-1) << " c " << S->get_cols() << endl;
      Matrix * mat = new Matrix(basisFuncP.at(i-1),S->get_cols());
      cerr << "New Matrix " << endl;
      for(auto k=1; k<=basisFuncP.at(i-1);++k){
        for(auto j=1; j<=S->get_cols();++j){
          cerr << "j " << j << " k " << k << endl;
          mat->set_elem(S->get_elem(k+coef_row,j),k,j);
        }
      }

      coef_row+=basisFuncP.at(i-1);
      p_atom_mat_S.push_back(mat);
    }

    cerr << "Matching monomerA" << endl;
    vector<pair<int,int>> monAmatch;
    for(unsigned i=1;i<=matchDimerA.size();++i){
      pair<int,int> pr(i,matchDimerA.at(i-1));
      monAmatch.push_back(pr);
    }
    vector<pair<int,int>> monBmatch;
    for(unsigned i=1;i<=matchDimerB.size();++i){
      pair<int,int> pr(i+monAmatch.size(),matchDimerB.at(i-1));
      monBmatch.push_back(pr);
    }

    cerr << "monAmatch" << endl;
    for(auto it : monAmatch ) cerr << it.first << " " << it.second << endl;

    cerr << "Determining swap order sorting A " << endl; 
    for( auto pr_ptr=monAmatch.begin(); pr_ptr!=monAmatch.end(); ++pr_ptr ){
      auto pr = *pr_ptr;
      int col1was = pr.first;
      int col1is  = pr.second;
      // Find if "row1was" is moved later if it is the reference needs to be
      // changed to "row1is"
      cerr << "first " << col1was << " second " << col1is << endl;

      // Fixing the values in MonomerA
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while(pr_ptr_temp!=monAmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monAmatch.end(),[col1was](const pair<int,int>& p){
            return col1was==p.second;});
        if(pr_ptr_temp!=monAmatch.end()){
          pr_ptr_temp->second = col1is;
        }
      } 
      // Fixing the values in MonomerB
      pr_ptr_temp = monBmatch.begin();
      while(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[col1was](const pair<int,int>& p){
            return col1was==p.second;});
        if(pr_ptr_temp!=monBmatch.end()){
          pr_ptr_temp->second = col1is;
        }
      }

    } 

    cerr << "Sorting through monomer B " << endl;

    for( auto pr_ptr=monBmatch.begin(); pr_ptr!=monBmatch.end(); ++pr_ptr ){
      auto pr = *pr_ptr;
      int col1was = pr.first;
      int col1is  = pr.second;
      cerr << "first " << col1was << " second " << col1is << endl;

      // Fixing the values in MonomerB
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[col1was](const pair<int,int>& p){
            return col1was==p.second;});
        if(pr_ptr_temp!=monBmatch.end()){
          pr_ptr_temp->second = col1is;
        }
      }
    } 

    // Now we know that if we swap sequentially we will not be moving the wrong
    // columns
    for( auto p : monAmatch ){
      if(p.first!=p.second){
        cerr << "Swapping " << p.first << " with " << p.second << endl;
        auto it = p_atom_mat_S.begin();  
        cerr << "Elements in matrix " << p.first << " Elements in matrix " << p.second << endl;
        Matrix * temp = *(next(it, p.first-1));
        Matrix * temp2 = *(next(it,p.second-1));
        for(int j=1;j<=temp->get_cols();++j){
          for(int i=1;i<=temp->get_rows();++i){
            cerr << temp->get_elem(i,j) << " ";
          }
          cerr << "     ";
          for(int i=1;i<=temp2->get_rows();++i){
            cerr << temp2->get_elem(i,j) << " ";
          }
          cerr << endl;
        }
        *(next(it,p.first-1)) = *(next(it,p.second-1));
        *(next(it,p.second-1)) = temp;
      } 
    }
    cerr << "B" << endl;
    for( auto p : monBmatch ){
      if(p.first!=p.second){
        cerr << "Swapping " << p.first << " with " << p.second << endl;
        auto it = p_atom_mat_S.begin();  
        Matrix * temp = *(next(it, p.first-1));
        *(next(it,p.first-1)) = *(next(it,p.second-1));
        *(next(it,p.second-1)) = temp;
      } 
    }

    cerr << "New matrix r " << S->get_rows() << " c " << S->get_cols() << endl;

    // Now we want to load the elements in the correct order into the matrix
    int row = 0;
    for( auto it=p_atom_mat_S.begin();it!=p_atom_mat_S.end();++it){
      Matrix * mat_ptr = *it;
      int col = 1;
      for( auto j=1;j<=mat_ptr->get_cols();++j){
        for( auto i=1;i<=mat_ptr->get_rows();++i){
          //cerr << i << " " << j << " " << row << " " << j+col << " ";
          cerr << mat_ptr->get_elem(i,j) << " " ;
          S_new->set_elem(mat_ptr->get_elem(i,j),i+row,col);
        }
        cerr << endl;
        ++col;
      }
      row+=mat_ptr->get_rows();
      cerr << "next set of coefficients" << endl;
    } 
  }

  // Return the correctly swapped dimerCoef
  return S_new;
}

Matrix * unscramble_OE(std::vector<int> matchDimerA,
    std::vector<int> matchDimerB,
    std::vector<int> basisFuncP,
    Matrix * OE){

  Matrix * OE_new = new Matrix(OE->get_rows(),1);

    list<Matrix *> p_atom_mat_OE;
    int num_atoms = basisFuncP.size();
    int coef_row = 0;
    cerr << "Number of atoms " << num_atoms << endl;
    for(auto i=1; i<=num_atoms;++i){
      cerr << "Creating matrix r " << basisFuncP.at(i-1) << " c " << OE->get_cols() << endl;
      Matrix * mat = new Matrix(basisFuncP.at(i-1),OE->get_cols());
      cerr << "New Matrix " << endl;
      for(auto k=1; k<=basisFuncP.at(i-1);++k){
        for(auto j=1; j<=OE->get_cols();++j){
          cerr << "j " << j << " k " << k << endl;
          mat->set_elem(OE->get_elem(k+coef_row,j),k,j);
        }
      }

      coef_row+=basisFuncP.at(i-1);
      p_atom_mat_OE.push_back(mat);
    }

    cerr << "Matching monomerA" << endl;
    vector<pair<int,int>> monAmatch;
    for(unsigned i=1;i<=matchDimerA.size();++i){
      pair<int,int> pr(i,matchDimerA.at(i-1));
      monAmatch.push_back(pr);
    }
    vector<pair<int,int>> monBmatch;
    for(unsigned i=1;i<=matchDimerB.size();++i){
      pair<int,int> pr(i+monAmatch.size(),matchDimerB.at(i-1));
      monBmatch.push_back(pr);
    }

    cerr << "monAmatch" << endl;
    for(auto it : monAmatch ) cerr << it.first << " " << it.second << endl;

    cerr << "Determining swap order sorting A " << endl; 
    for( auto pr_ptr=monAmatch.begin(); pr_ptr!=monAmatch.end(); ++pr_ptr ){
      auto pr = *pr_ptr;
      int col1was = pr.first;
      int col1is  = pr.second;
      // Find if "row1was" is moved later if it is the reference needs to be
      // changed to "row1is"
      cerr << "first " << col1was << " second " << col1is << endl;

      // Fixing the values in MonomerA
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while(pr_ptr_temp!=monAmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monAmatch.end(),[col1was](const pair<int,int>& p){
            return col1was==p.second;});
        if(pr_ptr_temp!=monAmatch.end()){
          pr_ptr_temp->second = col1is;
        }
      } 
      // Fixing the values in MonomerB
      pr_ptr_temp = monBmatch.begin();
      while(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[col1was](const pair<int,int>& p){
            return col1was==p.second;});
        if(pr_ptr_temp!=monBmatch.end()){
          pr_ptr_temp->second = col1is;
        }
      }

    } 

    cerr << "Sorting through monomer B " << endl;

    for( auto pr_ptr=monBmatch.begin(); pr_ptr!=monBmatch.end(); ++pr_ptr ){
      auto pr = *pr_ptr;
      int col1was = pr.first;
      int col1is  = pr.second;
      cerr << "first " << col1was << " second " << col1is << endl;

      // Fixing the values in MonomerB
      auto pr_ptr_temp = pr_ptr;
      pr_ptr_temp++;
      while(pr_ptr_temp!=monBmatch.end()){
        pr_ptr_temp = find_if(pr_ptr_temp,monBmatch.end(),[col1was](const pair<int,int>& p){
            return col1was==p.second;});
        if(pr_ptr_temp!=monBmatch.end()){
          pr_ptr_temp->second = col1is;
        }
      }
    } 

    // Now we know that if we swap sequentially we will not be moving the wrong
    // columns
    for( auto p : monAmatch ){
      if(p.first!=p.second){
        cerr << "Swapping " << p.first << " with " << p.second << endl;
        auto it = p_atom_mat_OE.begin();  
        cerr << "Elements in matrix " << p.first << " Elements in matrix " << p.second << endl;
        Matrix * temp = *(next(it, p.first-1));
        Matrix * temp2 = *(next(it,p.second-1));
        for(int j=1;j<=temp->get_cols();++j){
          for(int i=1;i<=temp->get_rows();++i){
            cerr << temp->get_elem(i,j) << " ";
          }
          cerr << "     ";
          for(int i=1;i<=temp2->get_rows();++i){
            cerr << temp2->get_elem(i,j) << " ";
          }
          cerr << endl;
        }
        *(next(it,p.first-1)) = *(next(it,p.second-1));
        *(next(it,p.second-1)) = temp;
      } 
    }
    cerr << "B" << endl;
    for( auto p : monBmatch ){
      if(p.first!=p.second){
        cerr << "Swapping " << p.first << " with " << p.second << endl;
        auto it = p_atom_mat_OE.begin();  
        Matrix * temp = *(next(it, p.first-1));
        *(next(it,p.first-1)) = *(next(it,p.second-1));
        *(next(it,p.second-1)) = temp;
      } 
    }

    cerr << "New matrix r " << OE->get_rows() << " c " << OE->get_cols() << endl;

    // Now we want to load the elements in the correct order into the matrix
    int row = 0;
    for( auto it=p_atom_mat_OE.begin();it!=p_atom_mat_OE.end();++it){
      Matrix * mat_ptr = *it;
      int col = 1;
      for( auto j=1;j<=mat_ptr->get_cols();++j){
        for( auto i=1;i<=mat_ptr->get_rows();++i){
          //cerr << i << " " << j << " " << row << " " << j+col << " ";
          cerr << mat_ptr->get_elem(i,j) << " " ;
          OE_new->set_elem(mat_ptr->get_elem(i,j),i+row,col);
        }
        cerr << endl;
        ++col;
      }
      row+=mat_ptr->get_rows();
      cerr << "next set of coefficients" << endl;
    } 

  return OE_new;
}
/*
Matrix calculate_zeta1( Matrix Mon1, int MO ){

	if(Mon1.get_rows()>MO || Mon1.get_cols()>MO){
		std:err << "ERROR calculate_zeta1 function fails because Mon1\n";
		std:err << "matrix is larger than that of the dimer system\n";
		exit(1);
	}

	//For all MO levels not just a sigle one
	if(Mon1.col>1){
		Matrix zeta1(MO,MO);

		int i;
		int j;

		for(i=1;i<=MO;i++){
			for(j=1;j<=MO;j++){
				if(i<=Mon1.get_rows() && j<=Mon1.get_cols()){
					zeta1.set_elem(Mon1.get_elem(i,j),i,j);
				}
			}
		}
		return zeta1;
	//For a single MO e.g. the HOMO or LUMO
	}else{
		Matrix zeta1(MO,1);
		
		int i;
		for(i=1;i<=MO;i++){
			if(i<=Mon1.get_rows()){
				zeta1.set_elem(Mon1.get_elem(i),i);
			}
		}
		return zeta1;
	}
}

Matrix calculate_zeta2( Matrix Mon2, int MO ){

	if(Mon2.get_rows()>MO || Mon2.get_cols()>MO){
		std:err << "ERROR calculate_zeta2 function fails because Mon1\n";
		std:err << "matrix is larger than that of the dimer system\n";
		exit(1);
	}

	//For all MO levels not just a sigle one
	if(Mon2.get_cols()>1){
		Matrix zeta2(MO,MO);

		int i;
		int j;
		int dffr;
		int diffc;

		diffr = MO-Mon2.get_rows();
		diffc = MO-Mon2.get_cols();

		for(i=1;i<=MO;i++){
			for(j=1;j<=MO;j++){
				if(i>diffr && j>diffc){
					zeta2.set_elem(Mon2.get_elem(i,j),i,j);
				}
			}
		}
		return zeta1;
	//For a single MO e.g. the HOMO or LUMO
	}else{
		Matrix zeta2(MO,1);
		
		int i;
		int dffr;

		diffr = MO-Mon2.get_rows();

		for(i=1;i<=MO;i++){
			if(i>diffr){
				zeta2.set_elem(Mon2.get_elem(i),i);
			}
		}
		return zeta1;
	}
}*/
