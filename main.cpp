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
	int HOMO1;
	int HOMO2;
	
	double hartreeToeV = 27.2114;

	string log;
	string pun1;
	string pun2;
	string punP;
	string line;
	string str;

	Matrix mat_S;
	Matrix mat_P_Coef;
	Matrix mat_P_OE;

	Matrix mat_1_Coef;
	Matrix mat_1_OE;
	Matrix mat_1_HOMO_Coef;

	Matrix mat_2_Coef;
	Matrix mat_2_OE;
	Matrix mat_2_HOMO_Coef;

	Matrix zetaAinv;
	Matrix zetaBinv;

	Matrix Inter;

	Matrix gammaA;
	Matrix gammaB;

	Matrix gammaA_inv;
	Matrix gammaB_inv;

	HOMO1 = 0;
	HOMO2 = 0;

	rv = check_arguments(argv, argc, &log, &pun1, &pun2, &punP, &HOMO1, &HOMO2);

	cout << "log file is: " << log << "\n";
	cout << "pun file for the first monomer is: "+pun1+"\n";
	cout << "pun file for the second monomer is: "+pun2+"\n";
	cout << "pun file for the dimer is: " << punP << "\n";
	
	if(rv==-1){
		exit(1);
	}

	cout << "HOMO for monomer 1 is: " << HOMO1 << "\n";
	cout << "HOMO for monomer 2 is: " << HOMO2 << "\n";

	//Open the .pun file find the total number of molecular orbitals

	MOP = log_countMO(&log);

	printf("Number of MO %d\n",MOP);
	
	log_getS(&log,&mat_S,MOP);
    cout << "Read in Overlap matrix\n";
	pun_getMO(&punP,&mat_P_Coef,&mat_P_OE);
	cout << "Read in Overlap Coefficients and Energies for dimer\n";
	pun_getMO(&pun1,&mat_1_Coef,&mat_1_OE);
	cout << "Read in Overlap Coefficients and Energies for monomer A\n";
	pun_getMO(&pun2,&mat_2_Coef,&mat_2_OE);
	cout << "Read in Overlap Coefficients and Energies for monomer B\n";

	cout << "mat_S\n";

	MO1 = mat_1_OE.get_rows();
	MO2 = mat_2_OE.get_rows();

	mat_1_HOMO_Coef = Matrix_getRow( mat_1_Coef, HOMO1);
	mat_2_HOMO_Coef = Matrix_getRow( mat_2_Coef, HOMO2);

	cout << "mat_1_HOMO_Coef\n";
	cout << "mat_2_HOMO_Coef\n";
	
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
	return 0;
}
