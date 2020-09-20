
#ifndef _CATNIP_ARGUMENT_STRING_HPP
#define _CATNIP_ARGUMENT_STRING_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {

class ArgumentString : public ArgumentObject {
 public:

  virtual ArgumentType getArgumentType(void) const noexcept final { 
    return ArgumentType::STRING; 
  }

  virtual std::type_index getValueType(void) const noexcept final {
    return typeid(std::string);
  }

  ArgumentString(void);
};

}  // namespace catnip

#endif  // _CATNIP_ARGUMENT_STRING_HPP
