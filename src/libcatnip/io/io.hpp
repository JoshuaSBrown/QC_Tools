#ifndef _CATNIP_IO_HPP
#define _CATNIP_IO_HPP
#include "../parameters.hpp"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

namespace mamap {
  class ArgumentParser;
}

namespace catnip {

std::unique_ptr<mamap::ArgumentParser> prepareParser(void);

std::unique_ptr<Parameters> prepareParameters(
    std::unique_ptr<mamap::ArgumentParser>& ArgParse);

}  // namespace catnip

#endif  // _CATNIP_IO_HPP
