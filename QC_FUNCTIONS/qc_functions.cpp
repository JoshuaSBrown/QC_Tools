#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
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

/*
Matrix organize_P_Coef(std::vector<int> matchDimerA,
                       std::vector<int> matchDimerB,
                       std::vector<int> basisFuncA,
                       std::vector<int> basisFuncB,
                       std::vector<int> basisFuncDimer,
                       Matrix dimerCoef){


  Matrix dimerCoef_new(dimerCoef.get_rows(),dimerCoef.get_cols());
  
  // Determine the starting row/col of the basis function associated
  // with a particular atom
  vector<int> matchToBasisA;
  matchToBasisA.push_back(1);
  int row = 1;
  for( auto d : basisFuncA ){
    row+=d;
    matchToBasisA.push_back(row);
  }

  // For A we will move all the coefficents in the dimer associated 
  // with A monomer to the top of the matrix and match with the correct
  // atom
  int dimer_row = 1;
  for( auto r : matchDimerA ){
    // match dimer onto A but within the dimer matrix
    // Have to make sure we grab all the atomic orbitals assocaited with the
    // atom
    if(r!=-1){
      for( int ao=0;ao<basisFuncA.at(dimer_row-1);ao++ ){
        dimerCoef_new.set_row(matchToBasisA.at(r-1+ao),dimerCoef.get_row(dimer_row+ao));
      } 
    }
    dimer_row++;
  }

  // Determine the starting row/col of the basis function associated
  // with a particular atom
  vector<int> matchToBasisB;
  matchToBasisB.push_back(1);
  row = 1;
  for( auto d : basisFuncB ){
    row+=d;
    matchToBasisB.push_back(row);
  }

  // For B we will move all the coefficients in the dimer associated
  // with B monomer to the bottome of the matrix and match with the
  // correct atom
  dimer_row = 1;
  for( auto r : matchDimerB ){
    // match dimer onto A but within the dimer matrix
    if(r!=-1){
      for( int ao=0;ao<basisFuncB.at(dimer_row-1);ao++ ){
        dimerCoef_new.set_row(matchToBasisB.at(r-1+ao),dimerCoef.get_row(dimer_row+ao));
      }
    } 
    dimer_row++;
  }
  return dimerCoef_new;
}
*/

/*
Matrix organize_S(std::vector<int> matchDimerA,
                  std::vector<int> matchDimerB,
                  std::vector<int> basisFuncA,
                  std::vector<int> basisFuncB,
                  std::vector<int> basisFuncDimer,
                  Matrix S){

  // If A move S columns horizontally 

  // If B move S rows vertically
}*/

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
