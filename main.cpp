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
#include "PARAMETERS/parameters.hpp"

using namespace std;

int main(int argc, char *argv[]){

	const double hartreeToeV = 27.2114;

	string line;
	string str;

	Parameters par = check_arguments(argv, argc);

  int HOMO1_Alpha = par.getHOMO_MO1Alpha();
  int HOMO1_Beta  = par.getHOMO_MO1Beta();
  int HOMO2_Alpha = par.getHOMO_MO2Alpha();
  int HOMO2_Beta  = par.getHOMO_MO2Beta();
  
  cout << "log file for first monomer is:      "+par.getLog1()+'\n';
  cout << "log file for second monomer is:     "+par.getLog2()+'\n';
	cout << "log file for dimer is:              "+par.getLogP()+'\n';
	cout << "pun file for the first monomer is:  "+par.getPun2()+'\n';
	cout << "pun file for the second monomer is: "+par.getPun1()+'\n';
	cout << "pun file for the dimer is:          "+par.getPunP()+'\n';
	
	//Open the .pun file find the total number of molecular orbitals

	int MOPAlpha = log_countMOAlpha(par.getLogP().c_str());
	int MOPBeta  = log_countMOBeta(par.getLogP().c_str());

  // Load in general coefficients 

  // If there are only alpha orbitals then we can assume that restricted HF was used
  bool rest = restrictedHF(MOPAlpha, MOPBeta);
  if(rest){
    // No need to worry about beta orbitals
    int MOP = MOPAlpha;
    
  }else{

    int MOP = MOPAlpha;
    if(MOPBeta>MOPAlpha){
      MOP = MOPBeta;
    } 
  }

  // Determine which spin has more orbitals use this to count the orbitals
	
  Matrix mat_S;
	log_getS(par.getLogP(),&mat_S,MOP);

  cout << "Read in Alpha Overlap matrix\n";
  // Orbital Energies from the .pun file are questionable
  Matrix mat_P_Coef_Alpha;
	pun_getMOAlpha(par.getPunP(),&mat_P_Coef_Alpha);
  auto vecP_MOEnergy_Alpha = log_getMOEnergiesAlpha(par.getLogP());
	cout << "Read in Alpha Overlap Coefficients and Energies for dimer\n";
  Matrix mat_1_Coef_Alpha;
	pun_getMOAlpha(par.getPun1(),&mat_1_Coef_Alpha);
  auto vec1_MOEnergy_Alpha = log_getMOEnergiesAlpha(par.getLog1());
	cout << "Read in Alpha Overlap Coefficients and Energies for monomer A\n";
  Matrix mat_2_Coef_Alpha;
	pun_getMOAlpha(par.getPun2(),&mat_2_Coef_Alpha);
  auto vec2_MOEnergy_Alpha = log_getMOEnergiesAlpha(par.getLog2());
	cout << "Read in Alpha Overlap Coefficients and Energies for monomer B\n";

  cout << "Read in Beta Overlap matrix\n";
  // Orbital Energies from the .pun file are questionable
  Matrix mat_P_Coef_Beta;
	pun_getMOBeta(par.getPunP(),&mat_P_Coef_Beta);
  auto vecP_MOEnergy_Beta = log_getMOEnergiesBeta(par.getLogP());
	cout << "Read in Beta Overlap Coefficients and Energies for dimer\n";
  Matrix mat_1_Coef_Beta;
	pun_getMOBeta(par.getPun1(),&mat_1_Coef_Beta);
  auto vec1_MOEnergy_Beta = log_getMOEnergiesBeta(par.getLog1());
	cout << "Read in Beta Overlap Coefficients and Energies for monomer A\n";
  Matrix mat_2_Coef_Beta;
	pun_getMOBeta(par.getPun2(),&mat_2_Coef_Beta);
  auto vec2_MOEnergy_Beta = log_getMOEnergiesBeta(par.getLog2());
	cout << "Read in Beta Overlap Coefficients and Energies for monomer B\n";

  // Returns an unordered map with the rank as the key stores a pair where the first
  // value is the energy and the second is a string indicating if it is in the alpha
  // or beta orbitals
  Matrix mat_1_OE_Alpha;
  Matrix mat_1_OE_Beta;
  auto rank_1 = findRank(mat_1_OE_Alpha, mat_1_OE_Beta);
  Matrix mat_2_OE_Alpha;
  Matrix mat_2_OE_Beta;
  auto rank_2 = findRank(mat_2_OE_Alpha, mat_2_OE_Beta);
  Matrix mat_P_OE_Alpha;
  Matrix mat_P_OE_Beta;
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

  Matrix mat_P_Coef;
  Matrix mat_P_OE;
  if( pr.second.compare("Alpha")==0){
    // Use Alpha for dimer
    mat_P_Coef = mat_P_Coef_Alpha;
    mat_P_OE   = mat_P_OE_Alpha;
  }else{
    // Use Beta for dimer
    mat_P_Coef = mat_P_Coef_Beta;
    mat_P_OE   = mat_P_OE_Beta;
  }

  int HOMO1 = 0;
  Matrix mat_1_Coef;
  Matrix mat_1_OE;
  if(pr1.second.compare("Alpha")==0){
    mat_1_Coef = mat_1_Coef_Alpha;
    HOMO1 = HOMO1_Alpha;
    mat_1_OE = mat_1_OE_Alpha;
  }else{
    mat_1_Coef = mat_1_Coef_Beta;
    HOMO1 = HOMO1_Beta;
    mat_1_OE = mat_1_OE_Beta;
  }

  int HOMO2 = 0;
  Matrix mat_2_Coef;
  Matrix mat_2_OE;
  if(pr2.second.compare("Alpha")==0){
    mat_2_Coef = mat_2_Coef_Alpha;
    HOMO2 = HOMO2_Alpha;
    mat_2_OE = mat_2_OE_Alpha;
  }else{
    mat_2_Coef = mat_2_Coef_Beta;
    HOMO2 = HOMO2_Beta;
    mat_2_OE = mat_2_OE_Beta;

  }

	int MO1 = mat_1_OE.get_rows();
	int MO2 = mat_2_OE.get_rows();

  // This is where we specify which orbitals we are grabbing
	Matrix mat_1_HOMO_Coef = Matrix_getRow( mat_1_Coef, HOMO1);
	Matrix mat_2_HOMO_Coef = Matrix_getRow( mat_2_Coef, HOMO2);

  cout << "HOMO" << endl;
	cout << "MO for monomer 1 is: " << HOMO1 << "\n";
	cout << "MO for monomer 2 is: " << HOMO2 << "\n";


  calculate_transfer_integral(
    mat_1_HOMO_Coef,
    mat_2_HOMO_Coef,
    mat_P_HOMO_Coef,
    MO1,
    MO2,
    mat_S,
    mat_P_OE);

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
	Matrix mat_1_LUMO_Coef = Matrix_getRow( mat_1_Coef, HOMO1+1);
	Matrix mat_2_LUMO_Coef = Matrix_getRow( mat_2_Coef, HOMO2+1);

  cout << "LUMO" << endl;
	cout << "MO for monomer 1 is: " << HOMO1+1 << "\n";
	cout << "MO for monomer 2 is: " << HOMO2+1 << "\n";

  calculate_transfer_integral(
    mat_1_LUMO_Coef,
    mat_2_LUMO_Coef,
    mat_P_LUMO_Coef,
    MO1,
    MO2,
    mat_S,
    mat_P_OE);

	return 0;
}
