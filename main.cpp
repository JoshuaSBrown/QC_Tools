#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <locale>
#include <functional>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>
#include <math.h>

#include "MATRIX/matrix.hpp"
#include "IO/io.hpp"
#include "QC_FUNCTIONS/qc_functions.hpp"

using namespace std;

int main(int argc, char *argv[]){

	int rv;
	int MO1;
	int MO2;
	int MOP;
  int MOPAlpha;
  int MOPBeta;
	
	double hartreeToeV = 27.2114;

	string log;
	string pun1;
	string pun2;
	string punP;
	string line;
	string str;

	Matrix mat_S;

	Matrix mat_P_Coef_Alpha;
	Matrix mat_1_Coef;
	Matrix mat_2_Coef;
	Matrix mat_P_OE;
	Matrix mat_1_HOMO_Coef;
	Matrix mat_2_HOMO_Coef;
	Matrix mat_1_LUMO_Coef;
	Matrix mat_2_LUMO_Coef;

	Matrix mat_P_Coef;
	Matrix mat_P_OE_Alpha;

	Matrix mat_1_OE;
	Matrix mat_1_Coef_Alpha;
	Matrix mat_1_OE_Alpha;
	Matrix mat_1_HOMO_Coef_Alpha;

	Matrix mat_2_OE;
	Matrix mat_2_Coef_Alpha;
	Matrix mat_2_OE_Alpha;
	Matrix mat_2_HOMO_Coef_Alpha;

	Matrix mat_P_Coef_Beta;
	Matrix mat_P_OE_Beta;

	Matrix mat_1_Coef_Beta;
	Matrix mat_1_OE_Beta;
	Matrix mat_1_HOMO_Coef_Beta;

	Matrix mat_2_Coef_Beta;
	Matrix mat_2_OE_Beta;
	Matrix mat_2_HOMO_Coef_Beta;


	Matrix zetaAinv;
	Matrix zetaBinv;

	Matrix Inter;

	Matrix gammaA;
	Matrix gammaB;

	Matrix gammaA_inv;
	Matrix gammaB_inv;

	int HOMO1 = 0;
	int HOMO2 = 0;

  int HOMO1_Alpha = 0;
  int HOMO1_Beta  = 0;
  int HOMO2_Alpha = 0;
  int HOMO2_Beta  = 0;

	rv = check_arguments(argv, argc, &log, &pun1, &pun2, &punP, &HOMO1_Alpha,&HOMO1_Beta,&HOMO2_Alpha, &HOMO2_Beta);

	cout << "log file is: " << log << "\n";
	cout << "pun file for the first monomer is: "+pun2+"\n";
	cout << "pun file for the second monomer is: "+pun1+"\n";
	cout << "pun file for the dimer is: " << punP << "\n";
	
	if(rv==-1){
		exit(1);
	}

	//Open the .pun file find the total number of molecular orbitals

	MOPAlpha = log_countMOAlpha(&log);
	MOPBeta  = log_countMOBeta(&log);

  // If there are only alpha orbitals then we can assume that restricted HF was used
  if(MOPBeta==0){
    printf("Assuming Restricted HF\n");
	  printf("Number of Alpha MO %d\n",MOPAlpha);
  }else{
    printf("Assuming Unrestricted HF\n");
	  printf("Number of Alpha MO %d\n",MOPAlpha);
	  printf("Number of Beta  MO %d\n",MOPBeta);
  }

  MOP = MOPAlpha;
  if(MOPBeta>MOPAlpha){
    MOP = MOPBeta;
  } 
  // Determine which spin has more orbitals use this to count the orbitals
	
	log_getS(&log,&mat_S,MOP);

  cout << "Read in Alpha Overlap matrix\n";
  // Orbital Energies from the .pun file are questionable
	pun_getMOAlpha(&punP,&mat_P_Coef_Alpha,&mat_P_OE_Alpha);
  log_getMOEnergiesAlpha(&log);
	cout << "Read in Alpha Overlap Coefficients and Energies for dimer\n";
	pun_getMOAlpha(&pun1,&mat_1_Coef_Alpha,&mat_1_OE_Alpha);
	cout << "Read in Alpha Overlap Coefficients and Energies for monomer A\n";
	pun_getMOAlpha(&pun2,&mat_2_Coef_Alpha,&mat_2_OE_Alpha);
	cout << "Read in Alpha Overlap Coefficients and Energies for monomer B\n";

  cout << "Read in Beta Overlap matrix\n";
  // Orbital Energies from the .pun file are questionable
	pun_getMOBeta(&punP,&mat_P_Coef_Beta,&mat_P_OE_Beta);
	cout << "Read in Beta Overlap Coefficients and Energies for dimer\n";
	pun_getMOBeta(&pun1,&mat_1_Coef_Beta,&mat_1_OE_Beta);
	cout << "Read in Beta Overlap Coefficients and Energies for monomer A\n";
	pun_getMOBeta(&pun2,&mat_2_Coef_Beta,&mat_2_OE_Beta);
	cout << "Read in Beta Overlap Coefficients and Energies for monomer B\n";

  // Returns an unordered map with the rank as the key stores a pair where the first
  // value is the energy and the second is a string indicating if it is in the alpha
  // or beta orbitals
  auto rank_1 = findRank(mat_1_OE_Alpha, mat_1_OE_Beta);
  auto rank_2 = findRank(mat_2_OE_Alpha, mat_2_OE_Beta);
  auto rank_P = findRank(mat_P_OE_Alpha, mat_P_OE_Beta);

  // Choosing the state with the highest filled state looking at both
  // Beta and alpha orbitals
  // HOMO 
  auto pr = rank_P[MOPAlpha+MOPBeta];
  auto pr1 = rank_1[HOMO1_Alpha+HOMO1_Beta];
  auto pr2 = rank_2[HOMO2_Alpha+HOMO2_Beta];
  
  // LUMO
  auto LUMO_pr = rank_P[MOPAlpha+MOPBeta+1];
  auto LUMO_pr1 = rank_1[HOMO1_Alpha+HOMO1_Beta+1];
  auto LUMO_pr2 = rank_2[HOMO2_Alpha+HOMO2_Beta+1];

  if( pr.second.compare("Alpha")==0){
    // Use Alpha for dimer
    mat_P_Coef = mat_P_Coef_Alpha;
    mat_P_OE   = mat_P_OE_Alpha;
  }else{
    // Use Beta for dimer
    mat_P_Coef = mat_P_Coef_Beta;
    mat_P_OE   = mat_P_OE_Beta;
  }

  if(pr1.second.compare("Alpha")==0){
    mat_1_Coef = mat_1_Coef_Alpha;
    HOMO1 = HOMO1_Alpha;
    mat_1_OE = mat_1_OE_Alpha;
  }else{
    mat_1_Coef = mat_1_Coef_Beta;
    HOMO1 = HOMO1_Beta;
    mat_1_OE = mat_1_OE_Beta;
  }

  if(pr2.second.compare("Alpha")==0){
    mat_2_Coef = mat_2_Coef_Alpha;
    HOMO2 = HOMO2_Alpha;
    mat_2_OE = mat_2_OE_Alpha;
  }else{
    mat_2_Coef = mat_2_Coef_Beta;
    HOMO2 = HOMO2_Beta;
    mat_2_OE = mat_2_OE_Beta;

  }

	MO1 = mat_1_OE.get_rows();
	MO2 = mat_2_OE.get_rows();

  // This is where we specify which orbitals we are grabbing
	mat_1_HOMO_Coef = Matrix_getRow( mat_1_Coef, HOMO1);
	mat_2_HOMO_Coef = Matrix_getRow( mat_2_Coef, HOMO2);

  cout << "HOMO" << endl;
	cout << "MO for monomer 1 is: " << HOMO1 << "\n";
	cout << "MO for monomer 2 is: " << HOMO2 << "\n";

  {	
    Matrix mat_1_HOMO_Coefinv = Matrix_Invert( mat_1_HOMO_Coef);
    Matrix mat_2_HOMO_Coefinv = Matrix_Invert( mat_2_HOMO_Coef);
    Matrix mat_P_Coefinv = Matrix_Invert( mat_P_Coef);

    Matrix zerosA(MO2,mat_1_HOMO_Coefinv.get_cols(),mat_1_HOMO_Coefinv.get_shel());
    Matrix zerosB(MO1,mat_2_HOMO_Coefinv.get_cols(),mat_2_HOMO_Coefinv.get_shel());

    Matrix zetaA = Matrix_concatenate_rows( mat_1_HOMO_Coefinv, zerosA );
    Matrix zetaB = Matrix_concatenate_rows( zerosB, mat_2_HOMO_Coefinv );

    zetaAinv = Matrix_Invert(zetaA);
    zetaBinv = Matrix_Invert(zetaB);

    Inter = mat_S * mat_P_Coefinv;

    gammaA = zetaAinv * Inter ;
    gammaB = zetaBinv * Inter ;

    gammaA_inv = Matrix_Invert(gammaA);
    gammaB_inv = Matrix_Invert(gammaB);

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
  }
////////////////////LUMO 

  if( LUMO_pr.second.compare("Alpha")==0){
    // Use Alpha for dimer
    mat_P_Coef = mat_P_Coef_Alpha;
    mat_P_OE   = mat_P_OE_Alpha;
  }else{
    // Use Beta for dimer
    mat_P_Coef = mat_P_Coef_Beta;
    mat_P_OE   = mat_P_OE_Beta;
  }

  if(LUMO_pr1.second.compare("Alpha")==0){
    mat_1_Coef = mat_1_Coef_Alpha;
    HOMO1 = HOMO1_Alpha;
    mat_1_OE = mat_1_OE_Alpha;
  }else{
    mat_1_Coef = mat_1_Coef_Beta;
    HOMO1 = HOMO1_Beta;
    mat_1_OE = mat_1_OE_Beta;
  }

  if(LUMO_pr2.second.compare("Alpha")==0){
    mat_2_Coef = mat_2_Coef_Alpha;
    HOMO2 = HOMO2_Alpha;
    mat_2_OE = mat_2_OE_Alpha;
  }else{
    mat_2_Coef = mat_2_Coef_Beta;
    HOMO2 = HOMO2_Beta;
    mat_2_OE = mat_2_OE_Beta;

  }

	MO1 = mat_1_OE.get_rows();
	MO2 = mat_2_OE.get_rows();

  // This is where we specify which orbitals we are grabbing
	mat_1_LUMO_Coef = Matrix_getRow( mat_1_Coef, HOMO1+1);
	mat_2_LUMO_Coef = Matrix_getRow( mat_2_Coef, HOMO2+1);

  cout << "LUMO" << endl;
	cout << "MO for monomer 1 is: " << HOMO1+1 << "\n";
	cout << "MO for monomer 2 is: " << HOMO2+1 << "\n";
  {	
    Matrix mat_1_LUMO_Coefinv = Matrix_Invert( mat_1_LUMO_Coef);
    Matrix mat_2_LUMO_Coefinv = Matrix_Invert( mat_2_LUMO_Coef);
    Matrix mat_P_Coefinv = Matrix_Invert( mat_P_Coef);

    Matrix zerosA(MO2,mat_1_LUMO_Coefinv.get_cols(),mat_1_LUMO_Coefinv.get_shel());
    Matrix zerosB(MO1,mat_2_LUMO_Coefinv.get_cols(),mat_2_LUMO_Coefinv.get_shel());

    Matrix zetaA = Matrix_concatenate_rows( mat_1_LUMO_Coefinv, zerosA );
    Matrix zetaB = Matrix_concatenate_rows( zerosB, mat_2_LUMO_Coefinv );

    zetaAinv = Matrix_Invert(zetaA);
    zetaBinv = Matrix_Invert(zetaB);

    Inter = mat_S * mat_P_Coefinv;

    gammaA = zetaAinv * Inter ;
    gammaB = zetaBinv * Inter ;

    gammaA_inv = Matrix_Invert(gammaA);
    gammaB_inv = Matrix_Invert(gammaB);

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
  }

	return 0;
}
