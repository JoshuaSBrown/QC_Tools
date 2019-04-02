
#ifndef _CATNIP_PROPERTY_STRING_CHOICE_HPP
#define _CATNIP_PROPERTY_STRING_CHOICE_HPP

#include <set>
#include "propertyobject.hpp"

namespace catnip {

class PropertyStringChoice : 
  public PropertyObject<std::string,std::set<std::string>>{
  private:
    std::string getName_(void) const { return "PROPERTY_STRING_CHOICE"; }
    std::vector<std::string> getOpts_(void) const;
  public:
    PropertyStringChoice(void);
    void setPropOption(std::string option,std::string var);
    void setPropOption(std::string option,std::set<std::string> var);
    bool propValid(const std::string & string_choice);
};

}
#endif // _CATNIP_PROPERTY_STRING_CHOICE_HPP
