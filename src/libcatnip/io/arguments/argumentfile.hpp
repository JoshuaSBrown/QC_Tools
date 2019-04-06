
#ifndef _CATNIP_ARGUMENT_FILE_HPP
#define _CATNIP_ARGUMENT_FILE_HPP

#include "argumentobject.hpp"
#include <set>

namespace catnip {

class ArgumentFile : public ArgumentObject<std::string> {
 private:
  std::string getName_(void) const { return "ARGUMENT_FILE"; }
  void registerProperties_(void);

 public:
  ArgumentFile(void);
};

}  // namespace catnip

#endif  // _CATNIP_ARGUMENT_FILE_HPP
