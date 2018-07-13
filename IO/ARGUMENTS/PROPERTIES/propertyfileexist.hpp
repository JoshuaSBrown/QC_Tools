
#ifndef _CATNIP_PROPERTY_FILEEXIST_HPP
#define _CATNIP_PROPERTY_FILEEXIST_HPP

#include <set>
#include "propertyobject.hpp"

namespace catnip {

class PropertyFileExist : public PropertyObject<std::string,int> {
  private:
    bool fileExist(std::string);
    std::string getName_(void) { return "PROPERTY_FILE_EXIST"; }
    std::vector<std::string> getOpts_(void);
  public:
    PropertyFileExist(void);
    PropertyFileExist(int fileMustExist);
    bool propValid(std::string fileName);
    void postCheck(void);
};

}

#endif // _CATNIP_PROPERTY_FILEEXIST_HPP

