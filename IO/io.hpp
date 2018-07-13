#ifndef _CATNIP_IO_HPP
#define _CATNIP_IO_HPP
#include <string>
#include <map>
#include <set>
#include <utility>
#include <memory>
#include "../MATRIX/matrix.hpp"
#include "../PARAMETERS/parameters.hpp"
#include "argumentparser.hpp"

namespace catnip {

std::unique_ptr<ArgumentParser> prepareParser(void);

std::unique_ptr<Parameters> prepareParameters(std::unique_ptr<ArgumentParser>& ArgParse);

}

#endif // _CATNIP_IO_HPP
