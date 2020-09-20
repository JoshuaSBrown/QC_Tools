
#ifndef _CATNIP_ARGUMENT_FILE_HPP
#define _CATNIP_ARGUMENT_FILE_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {

class ArgumentFile : public ArgumentObject {

 public:

  virtual ArgumentType getArgumentType(void) const noexcept final {
    return ArgumentType::FILES;
  }

  virtual std::type_index getValueType(void) const noexcept final { 
    return typeid(std::string);
  }

  ArgumentFile(void);
};

}  // namespace catnip

#endif  // _CATNIP_ARGUMENT_FILE_HPP
