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
#include "io.hpp"

void show_usage(std::string name) {
	
	std::cerr << "Usage: " << name << " <options(s)> SOURCES"
						<< '\n'
						<< "Options:"
						<< "-h, --help\t\tShow this help message\n"
						<< "\t-l, --log PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t-p, --pun PATH/PUNFILE\tSpecify the Guassian .pun file"
						<< std::endl;
}

std::string lastN(std::string input, int n) {
	return input.substr(input.size()-n);
}

std::string firstN(std::string input, int n) {
	return input.substr(0,n);
}

// trim from start (in place)
void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
std::string ltrimmed(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
std::string rtrimmed(std::string s) {
	rtrim(s);
	return s;
}


// trim from both ends (copying)
std::string trimmed(std::string s) {
	trim(s);
	return s;
}

int file_exist(char *name) {
	struct stat buffer;
	return (stat (name, &buffer) == 0);
}

void removeSpace(char* s) {
	char* s2 = s;
	do {
		if (*s2 != ' ')
			*s++ = *s2;
	} while (*s2++);
}

int check_arguments(char * argv[], int argc, std::string *log, std::string *pun){
		
	if(argc <= 1) {
		show_usage(argv[0]);
		return 1;
	}
	
	std::string arg;
	std::string ext;

	for ( int i=1;i<argc;++i){
		arg=argv[i];
		if((arg=="-h")||(arg=="--help")){
			show_usage(argv[0]);
			return 0;
		}else if ((arg=="-l")||(arg=="--log")){
			if(i+1<argc){
				*log = argv[i+1];
				ext = lastN(*log,4);
				if(ext==".log"){
					if(file_exist(const_cast<char*>((*log).c_str()))==0){
						std::cerr << "The file "+(*log)+" does not exist." << std::endl;
						return 1;
					}
					i++;
				}else{
					std::cerr << "--log option requires *.log file." << std::endl;
					return 1;
				}
			}else{
				std::cerr << "--log option requires one argument." << std::endl;
				return 1;
			}
		}else if ((arg=="-p")||(arg=="--pun")){
			if(i+1<argc){
				*pun = argv[i+1];
				ext = lastN(*pun,4);
				if(ext==".pun"){
					if(file_exist(const_cast<char*>((*pun).c_str()))==0){
						std::cerr << "The file "+(*pun)+" does not exist." << std::endl;
					}
					i++;
				}else{
					std::cerr << "--pun option requires *.pun file" << std::endl;
					return 1;
				}
			}else{
				std::cerr << "--pun option requires one argument." << std::endl;
				return 1;
			}
		}else {
			show_usage(argv[0]);
			return 1;
		}
	}
	if(argc<5){
		std::cerr << "Must have a *.pun file and a *.log file to run." << std::endl;
		show_usage(argv[0]);
		return 1;
	}

	return 0;
}

