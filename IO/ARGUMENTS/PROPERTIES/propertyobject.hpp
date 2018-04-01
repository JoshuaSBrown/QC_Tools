
#ifndef _PROPERTY_OBJECT_HPP
#define _PROPERTY_OBJECT_HPP

#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <map>
#include <iostream>
#include <vector>
#include <sstream>

template<typename S, typename T> 
class PropertyObject{
  protected:
    // Map key is the option name and the type "int, double etc"
    // void * points to the correct value for that type
    std::map<std::string,void *> options_;

    bool propOptionValid_(std::string option) { 
      for(auto opt : options_ ){
        if(opt.first.compare(option)==0) return true;
      }
      return false; 
    }
   
    void setPropOption_(std::string option, T val) {

      if(options_.count(option)==0){
        T * opt = new T(val);
        options_[option]=static_cast<void *>(opt);
      }else{
        T * opt = static_cast<T *>(options_[option]);
        *opt = val;
      }
    }

    virtual std::string getName_(void){return "UNKNOWN";}

    virtual std::vector<std::string> getOpts_(void){
      std::vector<std::string> options{"NO_OPTIONS"};
      return options;
    }
    
    
  public:
    virtual ~PropertyObject(void) {
      for( auto itr : options_){
        T * opt = static_cast<T *>(itr.second);
        delete opt;
      }
      options_.clear();
    }
    
    virtual bool propValid(S value) {
      std::ostringstream err;
      err << "propValid has been called from base class with value: ";
      err << value << ". It is only meant to be called from a derived class.";
      throw std::runtime_error(err.str());
      return true;
    }

    std::string getPropertyName(void) { return getName_(); }

    std::vector<std::string> getPropertyOptions(void) { return getOpts_(); }

    // Setup the valid options associated with the parameter
    void setPropOption(std::string option, T val) {
      if(!propOptionValid_(option)){
        throw std::invalid_argument("Property option is unrecognized "+option);
      }
      setPropOption_(option,val);
    }

    T getPropOption(std::string option){
      if(!propOptionValid_(option)){
        std::string err = ""+option+" is an unrecognized property option for "
          "property "+getName_();
        throw std::invalid_argument(err);  
      }
      return *(static_cast<T *>(options_[option]));
    }

    virtual void postCheck(void) { return; }
};
#endif
