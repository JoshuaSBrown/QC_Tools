
#ifndef _CATNIP_ARGUMENT_STRING_HPP
#define _CATNIP_ARGUMENT_STRING_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {

class ArgumentString : public ArgumentObject {
 public:

  virtual ArgumentType getArgumentType(void) const noexcept final 
  { return ArgumentType::STRING; }

  ArgumentString(void);
};

}  // namespace catnip

#endif  // _CATNIP_ARGUMENT_STRING_HPP
