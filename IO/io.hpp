#ifndef _IO_HPP
#define _IO_HPP
#include <string>

#include "../MATRIX/matrix.hpp"
#include "../PARAMETERS/parameters.hpp"

void show_usage(std::string name); 

int file_exist(char * name);

Parameters check_arguments(char * argv[], int argc);

int check_string_input(std::string str);

#endif
