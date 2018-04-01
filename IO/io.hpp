#ifndef _IO_HPP
#define _IO_HPP
#include <string>
#include <map>
#include <set>
#include <utility>
#include <memory>
#include "../MATRIX/matrix.hpp"
#include "../PARAMETERS/parameters.hpp"
#include "argumentparser.hpp"

std::unique_ptr<ArgumentParser> prepareParser(void);

std::unique_ptr<Parameters> prepareParameters(std::unique_ptr<ArgumentParser>& ArgParse);

#endif
