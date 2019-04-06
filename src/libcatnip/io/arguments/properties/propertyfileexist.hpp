
#ifndef _CATNIP_PROPERTY_FILEEXIST_HPP
#define _CATNIP_PROPERTY_FILEEXIST_HPP

#include "propertyobject.hpp"
#include <set>

namespace catnip {

class PropertyFileExist : public PropertyObject<std::string, int> {
 private:
  bool fileExist(const std::string &) const;
  std::string getName_(void) const { return "PROPERTY_FILE_EXIST"; }
  std::vector<std::string> getOpts_(void) const;

 public:
  explicit PropertyFileExist(void);
  PropertyFileExist(int fileMustExist);
  bool propValid(const std::string &fileName);
  void postCheck(void) const;
};

}  // namespace catnip

#endif  // _CATNIP_PROPERTY_FILEEXIST_HPP
