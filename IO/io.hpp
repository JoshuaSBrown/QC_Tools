#ifndef _IO_HPP
#define _IO_HPP
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

#include "../MATRIX/matrix.hpp"
#include "../PARAMETERS/parameters.hpp"

bool fileValid(std::string FileName,std::string ext);

void show_usage(std::string name); 

std::string fileExt(std::string);

int file_exist(char * name);

Parameters check_arguments(char * argv[], int argc);

int check_string_input(std::string str);

//Count the total number of molecular orbitals in the .pun file
int pun_countMO(std::string pun, std::string orb_type);

//Grab the MO information from the .pun file and store them in
//the mat_Coef. Coefficients for the atomic orbitals are stored in
//mat_Coef
int pun_getMO(std::string pun, Matrix *mat_Coef,std::string orb_type);

//Get the overlap matrix from the .log file and store in the
//mat_S matrix.
int log_getS(std::string log, Matrix *mat_S, int MO);

//Open the .log file and determine which molecular orbital is the
//highest occupied molecular orbital (HOMO) and return
int log_getHOMOAlpha(std::string log);
int log_getHOMOBeta(std::string log);

//Open the .log file and determine which molecular orbital is the
//lowest unoccupied molecular orbital (LUMO) and return
int log_getLUMOAlpha(std::string log);
int log_getLUMOBeta(std::string log);

//Count the total number of Alpha molecular orbitals in the .log file
int log_countMO(std::string log, std::string orb_type);

std::vector<double> log_getMOEnergiesAlpha(std::string log);
std::vector<double> log_getMOEnergiesBeta(std::string log);

//Used to determine if the orbital file is restricted or not
bool restricted(int MOPAlpha, int MOPBeta);
#endif
