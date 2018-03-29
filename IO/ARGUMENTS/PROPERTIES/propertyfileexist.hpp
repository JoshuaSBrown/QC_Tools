
#ifndef _PROPERTY_FILEEXIST_HPP
#define _PROPERTY_FILEEXIST_HPP

#include <set>
#include "propertyobject.hpp"

class PropertyFileExist : public PropertyObject<std::string,bool> {
  private:
    bool fileExist(std::string);
    std::string getName_(void) { return "PROPERTY_FILE_EXIST"; }
    std::vector<std::string> getOpts_(void);
  public:
    PropertyFileExist(void);
    PropertyFileExist(bool fileMustExist);
    bool propValid(std::string fileName);
};

#endif
