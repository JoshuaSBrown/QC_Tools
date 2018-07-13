
#ifndef _CATNIP_PROPERTY_FILEEXT_HPP
#define _CATNIP_PROPERTY_FILEEXT_HPP

#include <set>
#include "propertyobject.hpp"

namespace catnip {

class PropertyFileExt : public PropertyObject<std::string,std::set< std::string > >{
  private:
    void checkExt(std::string);
    void extSupported(std::string);
    std::string getName_(void) { return "PROPERTY_FILE_EXT"; }
    std::vector<std::string> getOpts_(void);
  public:
    PropertyFileExt(void);
    PropertyFileExt(std::string ext);
    PropertyFileExt(std::set<std::string> exts);
    bool propValid(std::string fileNamePath);
    void setPropOption(std::string option,std::string var);
    void setPropOption(std::string option,std::set<std::string> var);
};

}
#endif // _CATNIP_PROPERTY_FILEEXT_HPP
