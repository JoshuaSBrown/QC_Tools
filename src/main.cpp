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
#include "IO/argumentparser.hpp"
#include "QC_FUNCTIONS/qc_functions.hpp"
#include "PARAMETERS/parameters.hpp"
#include "LOG/log.hpp"
#include "IO/FILE_READERS/punreader.hpp"
#include "IO/FILE_READERS/logreader.hpp"

#include "calcJconfig.hpp"

using namespace catnip;
using namespace std;

int main(int argc,const char *argv[]){
  cout << endl;
  cout << "Running calc_J VERSION " << calcJ_VERSION_MAJOR << ".";
  cout << calcJ_VERSION_MINOR << endl;

	string line;
  LOG("Preparing parser",1);
  auto ArgPars = prepareParser();
  LOG("Parsing arguments",1);
  ArgPars->parse(argv,argc);
  LOG("Preparing parameter object",1);
  auto par = prepareParameters(ArgPars);

  cout << "log file for first monomer is:      "+par->getLog1()+'\n';
  cout << "log file for second monomer is:     "+par->getLog2()+'\n';
	cout << "log file for dimer is:              "+par->getLogP()+'\n';
	cout << "pun file for the first monomer is:  "+par->getPun1()+'\n';
	cout << "pun file for the second monomer is: "+par->getPun2()+'\n';
	cout << "pun file for the dimer is:          "+par->getPunP()+'\n';
	
	//Open the .pun file find the total number of molecular orbitals

  LOG("Reading pun files",1);
  LOG("Reading pun file: "+par->getPunP(),2);
  PunReader pun_reader_P(par->getPunP());
  pun_reader_P.read();
  LOG("Reading pun file: "+par->getPun1(),2);
  PunReader pun_reader_1(par->getPun1());
  pun_reader_1.read();
  LOG("Reading pun file: "+par->getPun2(),2);
  PunReader pun_reader_2(par->getPun2());
  pun_reader_2.read();

  LOG("Reading log files",1);
  LOG("Reading log file: "+par->getLogP(),2);
  LogReader log_reader_P(par->getLogP());
  log_reader_P.read();
  LOG("Reading log file: "+par->getLog1(),2);
  LogReader log_reader_1(par->getLog1());
  log_reader_1.read();
  LOG("Reading log file: "+par->getLog2(),2);
  LogReader log_reader_2(par->getLog2());
  log_reader_2.read();
  // Load in general coefficients 

  // If there are only alpha orbitals then we can assume that restricted HF 
  // was used

  // No need to worry about beta orbitals

  {
    Matrix * mat_S = log_reader_P.getOverlapMatrix();

    Matrix * mat_P_Coef = pun_reader_P.getCoefsMatrix(par->getSpinP());
    auto vec_P_OE = log_reader_P.getOE(par->getSpinP());
    Matrix * mat_P_OE = new Matrix(vec_P_OE);

    int HOMO1 = log_reader_1.getHOMOLevel(par->getSpin1());
    LOG("Getting "+par->getSpin1()+" of monomer 1",2);
    Matrix * mat_1_Coef = pun_reader_1.getCoefsMatrix(par->getSpin1());
    auto vec_1_OE = log_reader_1.getOE(par->getSpin1());
    Matrix * mat_1_OE = new Matrix(vec_1_OE);

    int HOMO2 = log_reader_2.getHOMOLevel(par->getSpin2());
    LOG("Getting "+par->getSpin2()+" of monomer 2",2);
    Matrix * mat_2_Coef = pun_reader_2.getCoefsMatrix(par->getSpin2());
    auto vec_2_OE = log_reader_2.getOE(par->getSpin2());
    Matrix * mat_2_OE = new Matrix(vec_2_OE);
    // Unscramble dimer coef and energies first need to see how the dimer
    // and monomer coefficients line up. To determine how the ceofficients
    // line up we will first look at how the atoms appear in each of the 
    // .gjf files. We will also check to see how many coefficients are 
    // assocaited with each of the atoms by checking the .log files. Given
    // the position of the atoms in the monomer unit and the positions of
    // the atoms in the dimer we can determine how the coefficients need 
    // to be rearranged.    
    vector<vector<double>> coord_P = log_reader_P.getCoords();
    vector<vector<double>> coord_1 = log_reader_1.getCoords();
    vector<vector<double>> coord_2 = log_reader_2.getCoords();

    if(coord_P.size()==0){
      throw runtime_error("Unable to read in coordinates from "+par->getLogP()); 
    }
    if(coord_1.size()==0){
      throw runtime_error("Unable to read in coordinates from "+par->getLog1()); 
    }
    if(coord_2.size()==0){
      throw runtime_error("Unable to read in coordinates from "+par->getLog2()); 
    }
    if(par->getCounterPoise()){
      if(coord_P.at(0).size()!=coord_1.at(0).size() && coord_P.at(0).size() != coord_2.at(0).size()){
        stringstream ss;
        ss << "Error the number of atom coordinates read in by "
          << par->getLogP() << " is " << coord_P.at(0).size()
          << ", it is not matched by the number of coordinates in "
          << par->getLog1() << " with " << coord_1.at(0).size() 
          << " atoms and " << par->getLog2() << " with "
          << coord_2.at(0).size() << " atoms."
          << " In a counter poise calculation all atoms must appear in both"
          << " files. Ghost atoms can be specifed with the -Bq keyword";
        throw runtime_error(ss.str());
      }

    }else{
      if(coord_P.at(0).size()!=(coord_1.at(0).size()+coord_2.at(0).size())){
        stringstream ss;
        ss << "Error the number of atom coordinates read in by "
          << par->getLogP() << " is " << coord_P.at(0).size()
          << ", it is not matched by the number of coordinates in "
          << par->getLog1() << " with " << coord_1.at(0).size() 
          << " atoms and " << par->getLog2() << " with "
          << coord_2.at(0).size() << " atoms.";
        throw runtime_error(ss.str());
      }
    }
    // Convert coords to matrices
    Matrix coord_P_mat(coord_P);
    Matrix coord_1_mat(coord_1);
    Matrix coord_2_mat(coord_2);

    auto basis_P = log_reader_P.getBasisFuncCount();  
    auto basis_1 = log_reader_1.getBasisFuncCount();  
    auto basis_2 = log_reader_2.getBasisFuncCount();  

    int MO1 = mat_1_OE->get_rows();
    int MO2 = mat_2_OE->get_rows();

    pair<int,int> Orbs1 = { MO1, HOMO1 };
    pair<int,int> Orbs2 = { MO2, HOMO2 };

    LOG("Creating transfercomplex",1);
    TransferComplex TC(
        mat_1_Coef,
        mat_2_Coef,
        mat_P_Coef,
        Orbs1,
        Orbs2,
        mat_S,
        mat_P_OE,
        par->getCounterPoise());

    // Set the transfer complex to counterpoise if it is the case. 

    // If the basis function search returns 0 for any of the components then
    // we cannot automatically determine what the transfer integral is
    if(basis_1.size()!=0 && basis_2.size()!=0 && basis_P.size()!=0){
      LOG("Unscrambling matrices",1);
      TC.unscramble(
          coord_1_mat,
          coord_2_mat,
          coord_P_mat,
          basis_P,
          basis_2);
    }

    cout << endl;
    cout << "Dimer     Spin " << par->getSpinP() << endl;

    cout << "Monomer 1 Spin " << par->getSpin1() << " ";
    if(par->getOrbNum1()==0){
      cout << "Orbital " << par->getOrbType1() << endl;
    }else if(par->getOrbNum1()>0){
      cout << "Orbital " << par->getOrbType1();
      cout << "+" << par->getOrbNum1() << endl;
    }else{
      cout << "Orbital " << par->getOrbType1();
      cout << par->getOrbNum1() << endl;
    }

    cout << "Monomer 2 Spin " << par->getSpin2() << " ";
    if(par->getOrbNum2()==0){
      cout << "Orbital " << par->getOrbType2() << endl;
    }else if(par->getOrbNum2()>0){
      cout << "Orbital " << par->getOrbType2();
      cout << "+" << par->getOrbNum2() << endl;
    }else{
      cout << "Orbital " << par->getOrbType2();
      cout << par->getOrbNum2() << endl;
    }

    map<string,string> orbitaltypes;
    map<string,int> orbitalnums;
    orbitaltypes["mon1"]=par->getOrbType1();
    orbitaltypes["mon2"]=par->getOrbType2();
    orbitalnums["mon1"]=par->getOrbNum1();
    orbitalnums["mon2"]=par->getOrbNum2();

    LOG("Calculating transfer integral",1);
    TC.calcJ(orbitaltypes,orbitalnums);

  }

	return 0;
}
