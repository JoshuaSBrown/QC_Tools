
#ifndef _ARGUMENTOBJECT_HPP
#define _ARGUMENTOBJECT_HPP

#include <string>
#include <memory>
#include <exception>
#include <map>
#include <iostream>

template<typename S, typename T> 
class ArgumentObject{
  protected:
    // Map key is the option name and the type "int, double etc"
    // void * points to the correct value for that type
    std::map<std::string,std::map<std::string,void *>> options_;

    bool argOptionValid_(std::string option) { 
      for(auto opt : options_ ){
        if(opt.first.compare(option)==0) return true;
      }
      return false; 
    }
    
    void setArgOption_(std::string option, T val) {
      T * opt = new T(val);
      options_[option][getType_()]=static_cast<void *>(opt);
    }

    virtual std::string getType_(void){return "UNKNOWN";}
  public:
    virtual bool argValid(S value) { return true;}
    std::string getRule(void) { return getType_(); }
    // Setup the valid options associated with the parameter
    void setArgOption(std::string option, T val) {
      if(!argOptionValid_(option)){
        throw std::invalid_argument("Argument option is unrecognized "+option);
      }
      setArgOption_(option,val);
    }

    T getArgOption(std::string option){
      return *(static_cast<T *>(options_[option][getType_()]));
    }
};
#endif
