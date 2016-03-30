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

int main(int argc, char *argv[]){

	int flag;
	int temp;
	int indent;
	int i;
	int j;
	double temp_d;
	double temp_d2;
	int MO;
	std::string log;
	std::string pun;
	std::string line;
	std::string str;
	std::size_t found;
	check_arguments(argv, argc, &log, &pun);

	std::cout << "log file is: "+log+"\n";
	std::cout << "pun file is: "+pun+"\n";

	//Open the .pun file find the total number of molecular orbitals
	std::ifstream PunFile;
	char output[100];

	//Determining the number of Molecular Orbitals
	PunFile.open(const_cast<char*>(pun.c_str()),std::ifstream::in);
	if(PunFile.is_open()){
		temp = 0;
		MO = 0;
		while(std::getline(PunFile,line)){
			if(((int)(found=line.find("Alpha")))!=-1){
				str = trimmed(firstN(line,(int)found));
				temp = atoi(str.c_str());
				if(temp>MO){
					MO=temp;
				}
			}
		}
	}
	PunFile.close();
	printf("\nMolecular Orbitals %d \n",MO);

	//Create a matrix to store the coefficients and orbital energies
	Matrix mat_Coef(MO,MO);
	Matrix mat_OE(MO);

	PunFile.open(const_cast<char*>(pun.c_str()),std::ifstream::in);
	if(PunFile.is_open()){
		i = 0;
		j = 0;
		//Skip the first line
		std::getline(PunFile,line);

		while(std::getline(PunFile,line)){
			flag = (int)(found=line.find("OE"));
			if(flag!=-1){
				i++;
				str = line.substr(found+3,11);
				str = trimmed(str);
				temp_d = atof(str.c_str());
				str = line.substr(found+15,4);
				str = trimmed(str);
				temp_d2 = atof(str.c_str());
				mat_OE.set_elem(temp_d*pow(10,temp_d2),i);
			}else{
				temp = 1;
				indent = 0;
				while (temp<6 && j<MO){
					j++;
					str = line.substr(0+indent,11);
					str = trimmed(str);
					temp_d = atof(str.c_str());
					str = line.substr(12+indent,3);
					str = trimmed(str);
					temp_d2 = atof(str.c_str());
					mat_Coef.set_elem(temp_d*pow(10,temp_d2),i,j);
					indent += 15;
					temp++;
				}
				if(j>=MO){
					j=0;
				}
			}
		}
	}
	//mat_OE.print();
	//mat_Coef.print();
	return 0;
}
