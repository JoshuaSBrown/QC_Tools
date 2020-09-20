
#ifndef _CATNIP_ARGUMENT_SWITCH_HPP
#define _CATNIP_ARGUMENT_SWITCH_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {
class ArgumentSwitch : public ArgumentObject {

 public:

  virtual ArgumentType getArgumentType(void) const noexcept final {
    return ArgumentType::SWITCH;
  }

  ArgumentSwitch(void);

  virtual std::type_index getValueType(void) const noexcept final {
    return typeid(bool);
  }

  virtual bool requiresParameter(void) final { return false; }
  bool positive(int val) const noexcept { return ((val == 1) ? true : false); }
  bool positive(std::string val) const;
};

}  // namespace catnip
#endif  // _CATNIP_ARGUMENT_SWITCH_HPP
