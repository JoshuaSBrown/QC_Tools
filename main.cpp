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

	string line;
	string str;

	Parameters par = check_arguments(argv, argc);

  cout << "Grabbed arguments" << endl;

  int HOMO1_Alpha = par.getHOMO_MO1Alpha();
  //int HOMO1_Beta  = par.getHOMO_MO1Beta();
  int HOMO2_Alpha = par.getHOMO_MO2Alpha();
  //int HOMO2_Beta  = par.getHOMO_MO2Beta();
  
  cout << "log file for first monomer is:      "+par.getLog1()+'\n';
  cout << "log file for second monomer is:     "+par.getLog2()+'\n';
	cout << "log file for dimer is:              "+par.getLogP()+'\n';
	cout << "pun file for the first monomer is:  "+par.getPun1()+'\n';
	cout << "pun file for the second monomer is: "+par.getPun2()+'\n';
	cout << "pun file for the dimer is:          "+par.getPunP()+'\n';
	
	//Open the .pun file find the total number of molecular orbitals

	int MOPAlpha = log_countMO(par.getLogP().c_str(),"Alpha");
	int MOPBeta  = log_countMO(par.getLogP().c_str(),"Beta");

  // Load in general coefficients 

  // If there are only alpha orbitals then we can assume that restricted HF 
  // was used
  bool rest = restricted(MOPAlpha, MOPBeta);
  if(rest){
    // No need to worry about beta orbitals
    int MOP = MOPAlpha;
 
    Matrix mat_S;
    cerr << "Getting overlap Matrix" << endl;
    log_getS(par.getLogP(),&mat_S,MOP);
    
    cerr << "Getting Alpha Coef" << endl;
    auto CoefsAlpha = readFilesCoef(par,"Alpha");

    cerr << "Getting Alpha Energies" << endl;
    auto EnergiesAlpha = readFilesEnergies(par,"Alpha");

    Matrix * mat_P_Coef = CoefsAlpha.at(0);
    Matrix * mat_P_OE   = EnergiesAlpha.at(0);

    int HOMO1 = HOMO1_Alpha;
    Matrix * mat_1_Coef = CoefsAlpha.at(1);
    Matrix * mat_1_OE   = EnergiesAlpha.at(1);

    int HOMO2 = HOMO2_Alpha;
    Matrix * mat_2_Coef = CoefsAlpha.at(2);
    Matrix * mat_2_OE   = EnergiesAlpha.at(2);

    // Unscramble dimer coef and energies first need to see how the dimer
    // and monomer coefficients line up. To determine how the ceofficients
    // line up we will first look at how the atoms appear in each of the 
    // .gjf files. We will also check to see how many coefficients are 
    // assocaited with each of the atoms by checking the .log files. Given
    // the position of the atoms in the monomer unit and the positions of
    // the atoms in the dimer we can determine how the coefficients need 
    // to be rearranged.     
    cerr << "Getting coordinates dimer" << endl;
    auto coord_P = log_getCoord(par.getLogP().c_str());
    cerr << "Getting coordinates mon1" << endl;
    auto coord_1 = log_getCoord(par.getLog1().c_str());
    cerr << "Getting coordinates mon2" << endl;
    auto coord_2 = log_getCoord(par.getLog2().c_str());

    // Convert coords to matrices
    cerr << "Size of coordinates P " << coord_P.at(0).size() << endl;
    cerr << "Size of coordinates 1 " << coord_1.at(0).size() << endl;
    cerr << "Size of coordinates 2 " << coord_2.at(0).size() << endl;

    Matrix coord_P_mat(coord_P);
    Matrix coord_1_mat(coord_1);
    Matrix coord_2_mat(coord_2);

    Matrix temp_P = coord_P_mat.invert();
    Matrix temp_1 = coord_1_mat.invert();
    Matrix temp_2 = coord_2_mat.invert();
    cerr << "Coordinate dimer" << endl;
    cerr << temp_P << endl;
    cerr << "Coordinate mon1" << endl;
    cerr << temp_1 << endl;
    cerr << "Coordinate mon2" << endl;
    cerr << temp_2 << endl;
    int sig_fig = 4;
    // Stores the rows in P that match 1
    cerr << "Matching rows mon1 with dimer" << endl;
    cerr << coord_P_mat << endl;
    auto match_1_P = coord_1_mat.matchCol(coord_P_mat,sig_fig);
    int ind = 1;
    for(auto val : match_1_P){
      cerr << "Matching atom " << ind << " of monomer 1 with atom " << val << " of dimer" << endl;
      ind++;
    }
    
    // Stores the rows in P that match 2
    cerr << "Matching rows mon2 with dimer" << endl;
    auto match_2_P = coord_2_mat.matchCol(coord_P_mat,sig_fig);

    ind = 1;
    for(auto val : match_2_P){
      cerr << "Matching atom " << ind << " of monomer 2 with atom " << val << " of dimer" << endl;
      ind++;
    }
    cerr << "counting atomic Basis mon1" << endl;
    auto basis_1 = log_countAtomBasisFunc(par.getLog1().c_str());
    cerr << "Size basis mon1 " << basis_1.size() << endl; 
    cerr << "counting atomic Basis mon2" << endl;
    auto basis_2 = log_countAtomBasisFunc(par.getLog2().c_str());
    cerr << "Size basis mon2 " << basis_2.size() << endl; 
    cerr << "counting atomic Basis Pair" << endl;
    auto basis_P = log_countAtomBasisFunc(par.getLogP().c_str());
    cerr << "Size basis func " << basis_P.size() << endl; 
    // rearrange the coefficients in the dimer matrix to line up with the 
    // coefficients in the monomer matrices
    
    // If the basis function search returns 0 for any of the components then
    // we cannot automatically determine what the transfer integral is

    int MO1 = mat_1_OE->get_rows();
    int MO2 = mat_2_OE->get_rows();

    // This is where we specify which orbitals we are grabbing
    Matrix mat_1_HOMO_Coef = mat_1_Coef->getRow( HOMO1 );
    Matrix mat_2_HOMO_Coef = mat_2_Coef->getRow( HOMO2 );

    if(basis_1.size()!=0 && basis_2.size()!=0 && basis_P.size()!=0){

      cerr << "unscrambling P" << endl;
      auto unscrambled_P_Coef = unscramble_P_Coef(
          match_1_P,
          match_2_P,
          basis_P,
          mat_P_Coef);

      cerr << "P MATRIX Before" << endl;
      cerr << *mat_P_Coef << endl;
      cerr << "P MATRIX" << endl;
      cerr << *unscrambled_P_Coef << endl;

      cerr << "unscrambling S" << endl;
      auto unscrambled_S = unscramble_S(
          match_1_P,
          match_2_P,
          basis_P,
          &mat_S);
      cerr << "S MATRIX Before" << endl;
      cerr << mat_S << endl;
      cerr << "S MATRIX" << endl;
      cerr << *unscrambled_S << endl;
/*
      cerr << "unscrambling OE" << endl;
      auto unscrambled_OE = unscramble_OE(
          match_1_P,
          match_2_P,
          basis_P,
          mat_P_OE);
*/
      cerr << "OE MATRIX" << endl;
      cerr << *mat_P_OE << endl;

      cerr << "1 MATRIX COEF" << endl;
      cerr << mat_1_HOMO_Coef << endl;
      cerr << "2 MATRIX COEF" << endl;
      cerr << mat_2_HOMO_Coef << endl;
      cout << endl;
      cout << "HOMO" << endl; 
      cerr << "HOMO" << endl;
      cerr << "MO1 " << MO1 << " MO2 " << MO2 << endl;
      cerr << "HOMO1 " << HOMO2 << " HOMO2 " << HOMO2 << endl;
      calculate_transfer_integral(
          mat_1_HOMO_Coef,
          mat_2_HOMO_Coef,
          *unscrambled_P_Coef,
          MO1,
          MO2,
          *unscrambled_S,
          *mat_P_OE);
    }else{

      cerr << "WARNING unable to automatically line up basis functions of"
              " monomers with dimers, you better make sure they correctly"
              " line up or run the calculations again with the correct "
              "flag" << endl;

      cerr << "P MATRIX" << endl;
      cerr << *mat_P_Coef << endl;

      cerr << "S MATRIX" << endl;
      cerr << mat_S << endl;

      cerr << "OE MATRIX" << endl;
      cerr << *mat_P_OE << endl;

      cerr << "1 MATRIX COEF" << endl;
      cerr << mat_1_HOMO_Coef << endl;
      cerr << "2 MATRIX COEF" << endl;
      cerr << mat_2_HOMO_Coef << endl;

      cout << endl;
      cout << "HOMO" << endl; 
      cerr << "HOMO" << endl;
      cerr << "MO1 " << MO1 << " MO2 " << MO2 << endl;
      cerr << "HOMO1 " << HOMO2 << " HOMO2 " << HOMO2 << endl;
      calculate_transfer_integral(
          mat_1_HOMO_Coef,
          mat_2_HOMO_Coef,
          *mat_P_Coef,
          MO1,
          MO2,
          mat_S,
          *mat_P_OE);


    }

    
    // Now we are going to get the LUMO
    int LUMO1 = HOMO1+1;
    int LUMO2 = HOMO2+1;
    Matrix mat_1_LUMO_Coef = mat_1_Coef->getRow( LUMO1 );
    Matrix mat_2_LUMO_Coef = mat_2_Coef->getRow( LUMO2 );
   
    if(basis_1.size()!=0 && basis_2.size()!=0 && basis_P.size()!=0){

      cerr << "unscrambling P" << endl;
      auto unscrambled_P_Coef = unscramble_P_Coef(
          match_1_P,
          match_2_P,
          basis_P,
          mat_P_Coef);

      cerr << "P MATRIX Before" << endl;
      cerr << *mat_P_Coef << endl;
      cerr << "P MATRIX" << endl;
      cerr << *unscrambled_P_Coef << endl;

      cerr << "unscrambling S" << endl;
      auto unscrambled_S = unscramble_S(
          match_1_P,
          match_2_P,
          basis_P,
          &mat_S);
      cerr << "S MATRIX Before" << endl;
      cerr << mat_S << endl;
      cerr << "S MATRIX" << endl;
      cerr << *unscrambled_S << endl;
/*
      cerr << "unscrambling OE" << endl;
      auto unscrambled_OE = unscramble_OE(
          match_1_P,
          match_2_P,
          basis_P,
          mat_P_OE);
*/
      cerr << "OE MATRIX" << endl;
      cerr << *mat_P_OE << endl;

      cerr << "1 MATRIX COEF" << endl;
      cerr << mat_1_LUMO_Coef << endl;
      cerr << "2 MATRIX COEF" << endl;
      cerr << mat_2_LUMO_Coef << endl;
      cout << endl;
      cout << "LUMO" << endl; 
      cerr << "LUMO" << endl;
      cerr << "MO1 " << MO1 << " MO2 " << MO2 << endl;
      cerr << "LUMO1 " << LUMO1 << " LUMO2 " << LUMO2 << endl;
      calculate_transfer_integral(
          mat_1_LUMO_Coef,
          mat_2_LUMO_Coef,
          *unscrambled_P_Coef,
          MO1,
          MO2,
          *unscrambled_S,
          *mat_P_OE);
    }else{
 
    cout << endl;
    cout << "LUMO" << endl; 
    calculate_transfer_integral(
        mat_1_LUMO_Coef,
        mat_2_LUMO_Coef,
        *mat_P_Coef,
        MO1,
        MO2,
        mat_S,
        *mat_P_OE);

    }
  
  }else{


/*
    int MOP = MOPAlpha;
    if(MOPBeta>MOPAlpha){
      MOP = MOPBeta;
    } 
    // Determine which spin has more orbitals use this to count the orbitals

    Matrix mat_S;
    log_getS(par.getLogP(),&mat_S,MOP);

    cout << "Read in Coefficients" << endl;
    auto CoefsAlpha = readFilesCoef(par,"Alpha");
    auto CoefsBeta = readFilesCoef(par,"Beta");

    cout << "Read in Energies" << endl;
    auto EnergiesAlpha = readFilesEnergies(par,"Alpha");
    auto EnergiesBeta = readFilesEnergies(par,"Beta");

    // Returns an unordered map with the rank as the key stores a pair where the 
    // first
    // value is the energy and the second is a string indicating if it is in the
    // alpha or beta orbitals
    auto rank_P = findRank(EnergiesAlpha.at(0), EnergiesBeta.at(0));
    auto rank_1 = findRank(EnergiesAlpha.at(1), EnergiesBeta.at(1));
    auto rank_2 = findRank(EnergiesAlpha.at(2), EnergiesBeta.at(2));

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

*/

  }

	return 0;
}
