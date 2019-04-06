
#ifndef _CATNIP_ARGUMENT_STRING_HPP
#define _CATNIP_ARGUMENT_STRING_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {

class ArgumentString : public ArgumentObject<std::string> {
 private:
  std::string getName_(void) const { return "ARGUMENT_STRING"; }
  void registerProperties_(void);

 public:
  ArgumentString(void);
};

}  // namespace catnip

#endif  // _CATNIP_ARGUMENT_STRING_HPP
