
#ifndef _CATNIP_ARGUMENT_DOUBLE_HPP
#define _CATNIP_ARGUMENT_DOUBLE_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {

class ArgumentNumerical : public ArgumentObject {
 private:
  std::string getName_(void) const { return "ARGUMENT_DOUBLE"; }
  void registerProperties_(void);

 public:

  virtual ArgumentType getArgumentType(void) const noexcept final 
  { return ArgumentType::NUMERICAL; }

  virtual std::type_index getValueType(void) const noexcept final {
    return typeid(double);
  }

  ArgumentNumerical(void);
};

}  // namespace catnip
#endif  // _CATNIP_ARGUMENT_DOUBLE_HPP
