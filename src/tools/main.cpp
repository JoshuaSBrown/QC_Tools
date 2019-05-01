#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <math.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "../libcatnip/io/argumentparser.hpp"
#include "../libcatnip/io/file_readers/logreader.hpp"
#include "../libcatnip/io/file_readers/punreader.hpp"
#include "../libcatnip/io/io.hpp"
#include "../libcatnip/log.hpp"
#include "../libcatnip/parameters.hpp"
#include "../libcatnip/qc_functions.hpp"
#include "../libcatnip/matrix.hpp"
#include "../libcatnip/calcJconfig.hpp"

#include <eigen3/Eigen/Core>

using namespace catnip;
using namespace std;

int main(int argc, const char *argv[]) {
  cout << endl;
  cout << "Running calc_J VERSION " << calcJ_VERSION_MAJOR << ".";
  cout << calcJ_VERSION_MINOR << endl;

  string line;
  LOG("Preparing parser", 1);
  auto ArgPars = prepareParser();
  LOG("Parsing arguments", 1);
  ArgPars->parse(argv, argc);
  LOG("Preparing parameter object", 1);
  auto par = prepareParameters(ArgPars);

  cout << "log file for first monomer is:      " + par->getLog1() + '\n';
  cout << "log file for second monomer is:     " + par->getLog2() + '\n';
  cout << "log file for dimer is:              " + par->getLogP() + '\n';
  cout << "pun file for the first monomer is:  " + par->getPun1() + '\n';
  cout << "pun file for the second monomer is: " + par->getPun2() + '\n';
  cout << "pun file for the dimer is:          " + par->getPunP() + '\n';

  // Open the .pun file find the total number of molecular orbitals

  LOG("Reading pun files", 1);
  LOG("Reading pun file: " + par->getPunP(), 2);
  PunReader pr_P(par->getPunP());
  pr_P.read();
  LOG("Reading pun file: " + par->getPun1(), 2);
  PunReader pr_1(par->getPun1());
  pr_1.read();
  LOG("Reading pun file: " + par->getPun2(), 2);
  PunReader pr_2(par->getPun2());
  pr_2.read();

  LOG("Reading log files", 1);
  LOG("Reading log file: " + par->getLogP(), 2);
  LogReader lr_P(par->getLogP());
  lr_P.read();
  LOG("Reading log file: " + par->getLog1(), 2);
  LogReader lr_1(par->getLog1());
  lr_1.read();
  LOG("Reading log file: " + par->getLog2(), 2);
  LogReader lr_2(par->getLog2());
  lr_2.read();
  // Load in general coefficients

  // If there are only alpha orbitals then we can assume that restricted HF
  // was used

  // No need to worry about beta orbitals

  {
    //Matrix *mat_S = lr_P.getOverlapMatrix();
    Eigen::MatrixXd mat_S = lr_P.getOverlapMatrix();

    //Matrix *mat_P_Coef = pr_P.getCoefsMatrix(par->getSpinP());
    Eigen::MatrixXd mat_P_Coef = pr_P.getCoefsMatrix(par->getSpinP());
    
    //auto vec_P_OE = lr_P.getOE(par->getSpinP());
    Eigen::VectorXd vec_P_OE = Eigen::VectorXd(lr_P.getOE(par->getSpinP()));
    //Matrix *vec_P_OE = new Matrix(vec_P_OE);

    int HOMO1 = lr_1.getHOMOLevel(par->getSpin1());
    LOG("Getting " + par->getSpin1() + " of monomer 1", 2);
    Eigen::MatrixXd mat_1_Coef = pr_1.getCoefsMatrix(par->getSpin1());
    //Matrix *mat_1_Coef = pr_1.getCoefsMatrix(par->getSpin1());
    Eigen::VectorXd vec_1_OE = lr_1.getOE(par->getSpin1());
    //Matrix *mat_1_OE = new Matrix(vec_1_OE);
    //Eigen::MatrixXd mat_1_OE = vec_1_OE.asDiagonal();

    int HOMO2 = lr_2.getHOMOLevel(par->getSpin2());
    LOG("Getting " + par->getSpin2() + " of monomer 2", 2);
    //Matrix *mat_2_Coef = pr_2.getCoefsMatrix(par->getSpin2());
    Eigen::MatrixXd mat_2_Coef = pr_2.getCoefsMatrix(par->getSpin2());
    Eigen::VectorXd vec_2_OE = lr_2.getOE(par->getSpin2());
    //Matrix *mat_2_OE = new Matrix(vec_2_OE);
    //Eigen::MatrixXd mat_2_OE = vec_2_OE.asDiagonal();

    // Unscramble dimer coef and energies first need to see how the dimer
    // and monomer coefficients line up. To determine how the ceofficients
    // line up we will first look at how the atoms appear in each of the
    // .gjf files. We will also check to see how many coefficients are
    // assocaited with each of the atoms by checking the .log files. Given
    // the position of the atoms in the monomer unit and the positions of
    // the atoms in the dimer we can determine how the coefficients need
    // to be rearranged.
    vector<vector<double>> coord_P = lr_P.getCoords();
    vector<vector<double>> coord_1 = lr_1.getCoords();
    vector<vector<double>> coord_2 = lr_2.getCoords();

    // Convert coords to matrices
    Eigen::MatrixXd coord_P_mat = convert(coord_P);
    Eigen::MatrixXd coord_1_mat = convert(coord_1);
    Eigen::MatrixXd coord_2_mat = convert(coord_2);

    vector<int> basis_P = lr_P.getBasisFuncCount();
    vector<int> basis_1 = lr_1.getBasisFuncCount();
    vector<int> basis_2 = lr_2.getBasisFuncCount();

    LOG("Creating transfercomplex", 1);
    TransferComplex TC(mat_1_Coef, mat_2_Coef, mat_P_Coef, HOMO1, HOMO2, mat_S,
                       vec_P_OE, par->getCounterPoise());

    // Set the transfer complex to counterpoise if it is the case.

    // If the basis function search returns 0 for any of the components then
    // we cannot automatically determine what the transfer integral is
    if (basis_1.size() != 0 && basis_2.size() != 0 && basis_P.size() != 0) {
      LOG("Unscrambling matrices", 1);
      TC.unscramble(coord_1_mat, coord_2_mat, coord_P_mat, basis_P, basis_2);
    }

    cout << endl;
    cout << "Dimer     Spin " << par->getSpinP() << endl;

    cout << "Monomer 1 Spin " << par->getSpin1() << " ";
    if (par->getOrbNum1() == 0) {
      cout << "Orbital " << par->getOrbType1() << endl;
    } else if (par->getOrbNum1() > 0) {
      cout << "Orbital " << par->getOrbType1();
      cout << "+" << par->getOrbNum1() << endl;
    } else {
      cout << "Orbital " << par->getOrbType1();
      cout << par->getOrbNum1() << endl;
    }

    cout << "Monomer 2 Spin " << par->getSpin2() << " ";
    if (par->getOrbNum2() == 0) {
      cout << "Orbital " << par->getOrbType2() << endl;
    } else if (par->getOrbNum2() > 0) {
      cout << "Orbital " << par->getOrbType2();
      cout << "+" << par->getOrbNum2() << endl;
    } else {
      cout << "Orbital " << par->getOrbType2();
      cout << par->getOrbNum2() << endl;
    }

    map<string, string> orbitaltypes;
    map<string, int> orbitalnums;
    orbitaltypes["mon1"] = par->getOrbType1();
    orbitaltypes["mon2"] = par->getOrbType2();
    orbitalnums["mon1"] = par->getOrbNum1();
    orbitalnums["mon2"] = par->getOrbNum2();

    LOG("Calculating transfer integral", 1);
    TC.calcJ();
    TC.printTransferIntegral(orbitaltypes, orbitalnums);
  }

  return 0;
}
