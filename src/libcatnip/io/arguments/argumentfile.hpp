
#ifndef _CATNIP_ARGUMENT_FILE_HPP
#define _CATNIP_ARGUMENT_FILE_HPP

#include <set>
#include "argumentobject.hpp"

namespace catnip {

class ArgumentFile : public ArgumentObject<std::string> {
  private:
    std::string getName_(void) const { return "ARGUMENT_FILE"; }
    void registerProperties_(void);
  public:
    ArgumentFile(void);
};

}

#endif // _CATNIP_ARGUMENT_FILE_HPP

