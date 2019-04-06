#ifndef _CATNIP_IO_HPP
#define _CATNIP_IO_HPP
#include "../matrix.hpp"
#include "../parameters.hpp"
#include "argumentparser.hpp"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

namespace catnip {

std::unique_ptr<ArgumentParser> prepareParser(void);

std::unique_ptr<Parameters> prepareParameters(
    std::unique_ptr<ArgumentParser>& ArgParse);

}  // namespace catnip

#endif  // _CATNIP_IO_HPP
