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

void show_usage(std::string name); 

std::vector<std::string> splitSt(std::string input);

//Checks if the arguments provided by the user
//match with arguments avaiable. If they match
//returns 0 if not -1
int check_string_input(std::string str);

std::string lastStringInPath(std::string input);

std::string lastN(std::string input, int n);

//cut off end n characters
std::string cut_end(std::string input, int n);

std::string firstN(std::string input, int n);

//cut off beginning n characters
std::string cut_beg(std::string input, int n);

// trim from start (in place)
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s); 

// trim from start (copying)
std::string ltrimmed(std::string s); 

// trim from end (copying)
std::string rtrimmed(std::string s); 

// trim from both ends (copying)
std::string trimmed(std::string s);

int file_exist(char *name);

void removeSpace(char* s);

int check_arguments(char * argv[], int argc, std::string *log, std::string *pun1, std::string *pun2, std::string *punP, int * HOMO1_Alpha, int * HOMO1_Beta, int * HOMO2_Alpha, int *  HOMO2_Beta);

//Count the total number of molecular orbitals in the .pun file
int pun_countMOAlpha(std::string *pun);
int pun_countMOBeta(std::string *pun);

//Grab the MO information from the .pun file and store them in
//the mat_Coef and mat_OE. Coefficients for the atomic orbitals
//are stored in mat_Coef, and the Molecular orbital energies are
//stored in the mat_OE file.
int pun_getMOAlpha(std::string *pun, Matrix *mat_Coef, Matrix *mat_OE);
int pun_getMOBeta(std::string *pun, Matrix *mat_Coef, Matrix *mat_OE);

//Get the overlap matrix from the .log file and store in the
//mat_S matrix.
int log_getS(std::string *log, Matrix *mat_S, int MO);

//Open the .log file and determine which molecular orbital is the
//highest occupied molecular orbital (HOMO) and return
int log_getHOMOAlpha(std::string *log);
int log_getHOMOBeta(std::string *log);

//Open the .log file and determine which molecular orbital is the
//lowest unoccupied molecular orbital (LUMO) and return
int log_getLUMOAlpha(std::string *log);
int log_getLUMOBeta(std::string *log);

//Count the total number of Alpha molecular orbitals in the .log file
int log_countMOAlpha(std::string *log);
int log_countMOBeta(std::string *log);

std::vector<double> log_getMOEnergiesAlpha(std::string *log);
std::vector<double> log_getMOEnergiesBeta(std::string *log);
#endif
