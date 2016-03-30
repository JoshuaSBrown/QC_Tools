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

void show_usage(std::string name); 

std::string lastN(std::string input, int n);

std::string firstN(std::string input, int n);

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

int check_arguments(char * argv[], int argc, std::string *log, std::string *pun);
#endif
