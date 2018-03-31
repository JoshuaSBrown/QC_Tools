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
#include "IO/punreader.hpp"
#include "IO/logreader.hpp"
#include "IO/argumentparser.hpp"

using namespace std;

ArgumentParser setupParser(void){

  // Creating Flags and their descriptions
  string longFlag = "--log1";
  string shortFlag = "-l_1";
  string desc = "Specify the Gaussian .log file for monomer 1 this is "
    "important for determining the HOMO and LUMO";

  vector<string> flag1 = {longFlag,shortFlag,desc};
 
  longFlag = "--log2";
  shortFlag = "-l_2";
  desc = "Specify the Gaussian .log file for monomer 2 this is important for "
    "determining the HOMO and LUMO";

  vector<string> flag2 = {longFlag,shortFlag,desc};

  longFlag = "--logP";
  shortFlag = "-l_P";
  desc = "Specify the Gaussian .log file for the dimer this is important for "
    "determining the HOMO and LUMO, the dimer.log file is also needed inorder "
    "to get the overlap matrix S.";

  vector<string> flag3 = {longFlag,shortFlag,desc};

  longFlag = "--pun1";
  shortFlag = "-p_1";
  desc = "Specify the Gaussian fort.7 file for monomer 1. This file contains "
    "the AO coefficients. Acceptable file extensions are .orb/.7/.pun";

  vector<string> flag4 = {longFlag,shortFlag,desc};
 
  longFlag = "--pun2";
  shortFlag = "-p_2";
  desc = "Specify the Gaussian fort.7 file for monomer 2. This file contains "
    "the AO coefficients. Acceptable file extensions are .orb/.7/.pun";

  vector<string> flag5 = {longFlag,shortFlag,desc};

  longFlag = "--punP";
  shortFlag = "-p_P";
  desc = "Specify the Gaussian fort.7 file for the dimer. This file contains "
    "the AO coefficients. Acceptable file extensions are .orb/.7/.pun";

  vector<string> flag6 = {longFlag,shortFlag,desc};

  set<vector<string>> flags;
  flags.insert(flag1);
  flags.insert(flag2);
  flags.insert(flag3);
  flags.insert(flag4);
  flags.insert(flag5);
  flags.insert(flag6);

  ArgumentParser ArgPars(flags);

  // Creating Rules for the flags
  set<string> pun_exts = {".pun",".7",".orb"};
  string log_ext = ".log";

  ArgPars.setFlagRule("--pun1","FILE_EXT","ALLOWED_FILE_TYPE",pun_exts);
  ArgPars.setFlagRule("--pun2","FILE_EXT","ALLOWED_FILE_TYPE",pun_exts);
  ArgPars.setFlagRule("--punP","FILE_EXT","ALLOWED_FILE_TYPE",pun_exts);

  ArgPars.setFlagRule("--log1","FILE_EXT","ALLOWED_FILE_TYPE",log_ext);
  ArgPars.setFlagRule("--log2","FILE_EXT","ALLOWED_FILE_TYPE",log_ext);
  ArgPars.setFlagRule("--logP","FILE_EXT","ALLOWED_FILE_TYPE",log_ext);

  ArgPars.setFlagRule("--pun1","FILE_EXIST","FILE_MUST_EXIST",true);
  ArgPars.setFlagRule("--pun2","FILE_EXIST","FILE_MUST_EXIST",true);
  ArgPars.setFlagRule("--punP","FILE_EXIST","FILE_MUST_EXIST",true);
  ArgPars.setFlagRule("--log1","FILE_EXIST","FILE_MUST_EXIST",true);
  ArgPars.setFlagRule("--log2","FILE_EXIST","FILE_MUST_EXIST",true);
  ArgPars.setFlagRule("--logP","FILE_EXIST","FILE_MUST_EXIST",true);

  return ArgPars;
}


int main(int argc, char *argv[]){

	string line;

  ArgumentParser ArgPars = setupParser();
  ArgPars.parse(argv,argc);

//	Parameters par = check_arguments(argv, argc);

  cout << "Grabbed arguments" << endl;
  
  cout << "log file for first monomer is:      "+par.getLog1()+'\n';
  cout << "log file for second monomer is:     "+par.getLog2()+'\n';
	cout << "log file for dimer is:              "+par.getLogP()+'\n';
	cout << "pun file for the first monomer is:  "+par.getPun1()+'\n';
	cout << "pun file for the second monomer is: "+par.getPun2()+'\n';
	cout << "pun file for the dimer is:          "+par.getPunP()+'\n';
	
	//Open the .pun file find the total number of molecular orbitals

  PunReader pr_P(par.getPunP());
  pr_P.read();
  PunReader pr_1(par.getPun1());
  pr_1.read();
  PunReader pr_2(par.getPun2());
  pr_2.read();

  LogReader lr_P(par.getLogP());
  lr_P.read();
  LogReader lr_1(par.getLog1());
  lr_1.read();
  LogReader lr_2(par.getLog2());
  lr_2.read();
  // Load in general coefficients 

  // If there are only alpha orbitals then we can assume that restricted HF 
  // was used

  // No need to worry about beta orbitals
	string orb_type = "Alpha";

  {
    Matrix * mat_S = lr_P.getOverlapMatrix();

    Matrix * mat_P_Coef = pr_P.getCoefsMatrix(orb_type);
    auto vec_P_OE = lr_P.getOE(orb_type);
    Matrix * mat_P_OE = new Matrix(vec_P_OE);

    int HOMO1 = lr_1.getHOMOLevel(orb_type);
    Matrix * mat_1_Coef = pr_1.getCoefsMatrix(orb_type);
    auto vec_1_OE = lr_1.getOE(orb_type);
    Matrix * mat_1_OE = new Matrix(vec_1_OE);

    int HOMO2 = lr_2.getHOMOLevel(orb_type);
    Matrix * mat_2_Coef = pr_2.getCoefsMatrix(orb_type);
    auto vec_2_OE = lr_2.getOE(orb_type);
    Matrix * mat_2_OE = new Matrix(vec_2_OE);

    // Unscramble dimer coef and energies first need to see how the dimer
    // and monomer coefficients line up. To determine how the ceofficients
    // line up we will first look at how the atoms appear in each of the 
    // .gjf files. We will also check to see how many coefficients are 
    // assocaited with each of the atoms by checking the .log files. Given
    // the position of the atoms in the monomer unit and the positions of
    // the atoms in the dimer we can determine how the coefficients need 
    // to be rearranged.     
    auto coord_P = lr_P.getCoords();
    auto coord_1 = lr_1.getCoords();
    auto coord_2 = lr_2.getCoords();

    // Convert coords to matrices
    Matrix coord_P_mat(coord_P);
    Matrix coord_1_mat(coord_1);
    Matrix coord_2_mat(coord_2);

    auto basis_P = lr_P.getBasisFuncCount();  
    auto basis_1 = lr_1.getBasisFuncCount();  
    auto basis_2 = lr_2.getBasisFuncCount();  

    int MO1 = mat_1_OE->get_rows();
    int MO2 = mat_2_OE->get_rows();

    pair<int,int> Orbs1 = { MO1, HOMO1 };
    pair<int,int> Orbs2 = { MO2, HOMO2 };

    TransferComplex TC(
        mat_1_Coef,
        mat_2_Coef,
        mat_P_Coef,
        Orbs1,
        Orbs2,
        mat_S,
        mat_P_OE);

    // If the basis function search returns 0 for any of the components then
    // we cannot automatically determine what the transfer integral is
    if(basis_1.size()!=0 && basis_2.size()!=0 && basis_P.size()!=0){
      TC.unscramble(
          coord_1_mat,
          coord_2_mat,
          coord_P_mat,
          basis_P);
    }

    cout << endl;
    cout << "Values calculated for " << orb_type << " orbitals" << endl;
    TC.calcJ("HOMO",0);
    TC.calcJ("LUMO",0);
  }

  orb_type = "Beta";
  if(!pr_P.restrictedShell()){
    Matrix * mat_S = lr_P.getOverlapMatrix();

    Matrix * mat_P_Coef = pr_P.getCoefsMatrix(orb_type);
    auto vec_P_OE = lr_P.getOE(orb_type);
    Matrix * mat_P_OE = new Matrix(vec_P_OE);

    int HOMO1 = lr_1.getHOMOLevel(orb_type);
    Matrix * mat_1_Coef = pr_1.getCoefsMatrix(orb_type);
    auto vec_1_OE = lr_1.getOE(orb_type);
    Matrix * mat_1_OE = new Matrix(vec_1_OE);

    int HOMO2 = lr_2.getHOMOLevel(orb_type);
    Matrix * mat_2_Coef = pr_2.getCoefsMatrix(orb_type);
    auto vec_2_OE = lr_2.getOE(orb_type);
    Matrix * mat_2_OE = new Matrix(vec_2_OE);

    // Unscramble dimer coef and energies first need to see how the dimer
    // and monomer coefficients line up. To determine how the ceofficients
    // line up we will first look at how the atoms appear in each of the 
    // .gjf files. We will also check to see how many coefficients are 
    // assocaited with each of the atoms by checking the .log files. Given
    // the position of the atoms in the monomer unit and the positions of
    // the atoms in the dimer we can determine how the coefficients need 
    // to be rearranged.     
    auto coord_P = lr_P.getCoords();
    auto coord_1 = lr_1.getCoords();
    auto coord_2 = lr_2.getCoords();

    // Convert coords to matrices
    Matrix coord_P_mat(coord_P);
    Matrix coord_1_mat(coord_1);
    Matrix coord_2_mat(coord_2);

    auto basis_P = lr_P.getBasisFuncCount();  
    auto basis_1 = lr_1.getBasisFuncCount();  
    auto basis_2 = lr_2.getBasisFuncCount();  

    int MO1 = mat_1_OE->get_rows();
    int MO2 = mat_2_OE->get_rows();

    pair<int,int> Orbs1 = { MO1, HOMO1 };
    pair<int,int> Orbs2 = { MO2, HOMO2 };

    TransferComplex TC(
        mat_1_Coef,
        mat_2_Coef,
        mat_P_Coef,
        Orbs1,
        Orbs2,
        mat_S,
        mat_P_OE);

    // If the basis function search returns 0 for any of the components then
    // we cannot automatically determine what the transfer integral is
    if(basis_1.size()!=0 && basis_2.size()!=0 && basis_P.size()!=0){
      TC.unscramble(
          coord_1_mat,
          coord_2_mat,
          coord_P_mat,
          basis_P);
    }
    
    cout << endl;
    cout << "Values calculated for " << orb_type << " orbitals" << endl;
    TC.calcJ("HOMO",0);
    TC.calcJ("LUMO",0);

  }

	return 0;
}
