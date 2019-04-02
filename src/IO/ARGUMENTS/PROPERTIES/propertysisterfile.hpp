
#ifndef _CATNIP_PROPERTY_SISTER_FILE_HPP
#define _CATNIP_PROPERTY_SISTER_FILE_HPP

#include <vector>
#include "propertyobject.hpp"

namespace catnip {

class PropertySisterFile : 
  public PropertyObject<std::string,std::vector<std::string>>{
  private:
    std::string getName_(void) const { return "PROPERTY_SISTER_FILE"; }
    std::vector<std::string> getOpts_(void) const;
    bool fileExist(const std::string &) const;
    void extSupported(const std::string &) const;
    void checkExt(const std::string &) const;
  public:
    PropertySisterFile(void);
    void setPropOption(std::string option,std::string var);
    void setPropOption(std::string option,std::vector<std::string> var);
    bool propValid(const std::string & fileNamePath);
};

}
#endif // _CATNIP_PROPERTY_SISTER_FILE_HPP
