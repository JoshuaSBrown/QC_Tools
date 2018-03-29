
#ifndef _ARGUMENTSTRING_HPP
#define _ARGUMENTSTRING_HPP

#include "argumentobject.hpp"

class ArgumentString : public ArgumentObject<std::string,size_t>{ 
  private:
    void stringValid(std::string val);
    std::string getType_(void) { return "STRING";}
  public:
    ArgumentString(void);
    bool argValid(std::string value) { stringValid(value); return true;}
};

#endif 

