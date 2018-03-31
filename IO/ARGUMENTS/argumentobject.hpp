
#ifndef _ARGUMENTOBJECT_HPP
#define _ARGUMENTOBJECT_HPP

#include <string>
#include <memory>
#include <exception>
#include <map>
#include <iostream>

#include "PROPERTIES/propertyobject.hpp"
// The template is simply for specifying the type of the argument coming
// from the command line
template<typename S>
class ArgumentObject{
  protected:

    std::vector<PropertyObject<S,int>*> int_propobjs_;
    std::vector<PropertyObject<S,double>*> double_propobjs_;
    std::vector<PropertyObject<S,bool>*> bool_propobjs_;
    std::vector<PropertyObject<S,size_t>*> size_t_propobjs_;
    std::vector<PropertyObject<S,std::string>*> string_propobjs_;
    std::vector<PropertyObject<S,std::set<std::string>>*> string_set_propobjs_;

    virtual std::string getName_(void){return "UNKNOWN";}

    void checkValid_(S val){
      for(auto prop : int_propobjs_){
        prop->propValid(val);
      }
      for(auto prop : double_propobjs_){
        prop->propValid(val);
      }
      for(auto prop : bool_propobjs_){
        prop->propValid(val);
      }
      for(auto prop : size_t_propobjs_){
        prop->propValid(val);
      }
      for(auto prop : string_propobjs_){
        prop->propValid(val);
      }
      for(auto prop : string_set_propobjs_){
        prop->propValid(val);
      }
    }

    // Register the relevant properties
    virtual void registerProperties_(void) {};
    
  public:

    virtual ~ArgumentObject(void){
      while(!int_propobjs_.empty()){
        auto prop = int_propobjs_.back();
        delete prop;
        int_propobjs_.pop_back();
      }
      while(!double_propobjs_.empty()){
        auto prop = double_propobjs_.back();
        delete prop;
        double_propobjs_.pop_back();
      }
      while(!bool_propobjs_.empty()){
        auto prop = bool_propobjs_.back();
        delete prop;
        bool_propobjs_.pop_back();
      }
      while(!size_t_propobjs_.empty()){
        auto prop = size_t_propobjs_.back();
        delete prop;
        size_t_propobjs_.pop_back();
      }
      while(!string_propobjs_.empty()){
        auto prop = string_propobjs_.back();
        delete prop;
        string_propobjs_.pop_back();
      }
      while(!string_set_propobjs_.empty()){
        auto prop = string_propobjs_.back();
        delete prop;
        string_propobjs_.pop_back();
      }
    }

    std::string getArgumentName(void) { return getName_(); }
 
    std::vector<std::string> getProperties(void){
      std::vector<std::string> props;
      for(auto prop : int_propobjs_){
        props.push_back(prop->getPropertyName());
      }
      for(auto prop : double_propobjs_){
        props.push_back(prop->getPropertyName());
      }
      for(auto prop : bool_propobjs_){
        props.push_back(prop->getPropertyName());
      }
      for(auto prop : size_t_propobjs_){
        props.push_back(prop->getPropertyName());
      }
      for(auto prop : string_propobjs_){
        props.push_back(prop->getPropertyName());
      }
      for(auto prop : string_set_propobjs_){
        props.push_back(prop->getPropertyName());
      }
  
      if(props.size()==0) props.push_back("NO_PROPERTIES");
      return props;
    }

    std::vector<std::string> getPropertyOptions(void){
      std::vector<std::string> ops;
      for(auto prop : int_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        ops.insert(ops.end(),vec_opt.begin(),vec_opt.end());
      }
      for(auto prop : double_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        ops.insert(ops.end(),vec_opt.begin(),vec_opt.end());
      }
      for(auto prop : bool_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        ops.insert(ops.end(),vec_opt.begin(),vec_opt.end());
      }
      for(auto prop : size_t_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        ops.insert(ops.end(),vec_opt.begin(),vec_opt.end());
      }
      for(auto prop : string_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        ops.insert(ops.end(),vec_opt.begin(),vec_opt.end());
      }
      for(auto prop : string_set_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        ops.insert(ops.end(),vec_opt.begin(),vec_opt.end());
      }
      if(ops.size()==0) ops.push_back("NO_OPTIONS");
      return ops;

    }
 
    std::map<std::string,std::string> getPropertyValues(void){
      std::map<std::string,std::string> opts_values;
      for(auto prop : int_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        for(auto opt : vec_opt ){
          auto value = std::to_string(prop->getPropOption(opt));
          opts_values[opt]=value;
        }
      }
      for(auto prop : double_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        for(auto opt : vec_opt ){
          auto value = std::to_string(prop->getPropOption(opt));
          opts_values[opt]=value;
        }
      }
      for(auto prop: bool_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        for(auto opt : vec_opt ){
          if(prop->getPropOption(opt)){
            opts_values[opt]="true";
          }else{
            opts_values[opt]="false";
          }
        }
      }
      for(auto prop: size_t_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        for(auto opt : vec_opt ){
          auto value = std::to_string(prop->getPropOption(opt));
          opts_values[opt]=value;
        }
      }
      for(auto prop: string_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        for(auto opt : vec_opt ){
          auto value = prop->getPropOption(opt);
          opts_values[opt]=value;
        }
      }
      for(auto prop: string_set_propobjs_){
        auto vec_opt = prop->getPropertyOptions();
        for(auto opt : vec_opt ){
          std::set<std::string> value_set = prop->getPropOption(opt);
          std::string values = "";
          for(auto val : value_set ){
            values.append(val);
            values.append(" ");
          }
          opts_values[opt]=values;
        }
      }
      return opts_values;
    }

    // Setup the valid options associated with the parameter
    void setArgPropertyOpt(std::string property, std::string option, double val){
      bool setval = false;
      for(auto prop : double_propobjs_){
        if(property.compare(prop->getPropertyName())==0){
          prop->setPropOption(option,val);
          setval = true;
        }
      }      
      if(!setval){
        std::string err = "Argument property: "+property+" with option: "+""
          ""+option+" of type double is unrecognized for argument "+getName_();
        throw std::invalid_argument(err);
      }
    }

    void setArgPropertyOpt(std::string property,std::string option, std::set<std::string> val){
      bool setval = false;
      for(auto prop : string_set_propobjs_){
        if(property.compare(prop->getPropertyName())==0){
          prop->setPropOption(option,val);
          setval = true;
        }
      }      
      if(!setval){
        std::string err = "Argument property: "+property+" with option: "+""
          ""+option+" of type set<string> is unrecognized for argument "+""
          ""+getName_();
        throw std::invalid_argument(err);
      }
    }

    void setArgPropertyOpt(std::string property,std::string option, std::string val){
      bool setval = false;
      for(auto prop : string_propobjs_){
        std::cerr << "property " << property << " option " << option << " string " << val << std::endl;
        std::cerr << "Prop.name " << prop->getPropertyName() << std::endl;
        std::cerr << "compare " << property.compare(prop->getPropertyName()) << std::endl;
        if(property.compare(prop->getPropertyName())==0){
          prop->setPropOption(option,val);
          setval = true;
        }
      }      
      if(!setval){
        std::string err = "Argument property: "+property+" with option: "+""
          ""+option+" of type string is unrecognized for argument "+getName_();
        throw std::invalid_argument(err);
      }
    }

    void setArgPropertyOpt(std::string property,std::string option, int val){
      bool setval = false;
      for(auto prop : int_propobjs_){
        if(property.compare(prop->getPropertyName())==0){
          prop->setPropOption(option,val);
          setval = true;
        }
      }      
      if(!setval){
        std::string err = "Argument property: "+property+" with option: "+""
          ""+option+" of type int is unrecognized for argument "+getName_();
        throw std::invalid_argument(err);
      }
    }

    void setArgPropertyOpt(std::string property,std::string option, bool val){
      bool setval = false;
      for(auto prop : bool_propobjs_){
        if(property.compare(prop->getPropertyName())==0){
          prop->setPropOption(option,val);
          setval = true;
        }
      }      
      if(!setval){
        std::string err = "Argument property: "+property+" with option: "+""
          ""+option+" of type bool is unrecognized for argument "+getName_();
        throw std::invalid_argument(err);
      }
    }

    bool argValid(S value){
      checkValid_(value);
      return true;
    }
};
#endif
