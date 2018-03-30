
#include "argumentstring.hpp"
#include <cassert>
#include <limits>

using namespace std;

int main(void){

  cerr << "Testing: argumentint" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentString argString;
  }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentString argString;
    string name = "ARGUMENT_STRING";
    assert(name.compare(argString.getArgumentName())==0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentString argString;
    auto props = argString.getProperties();

    bool string_prop = false;

    for(auto prop : props){
      if(prop.compare("PROPERTY_STRING")==0){
        string_prop = true;
      }
    }
    assert(string_prop);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentString argString;
    auto prop_opts = argString.getPropertyOptions();
    
    bool opt_min = false;
    bool opt_max = false;
  
    for( auto opt : prop_opts ){
      if(opt.compare("MIN_LENGTH")==0){
        opt_min = true;
      }
      if(opt.compare("MAX_LENGTH")==0){
        opt_max = true;
      }
    }

    assert(opt_min);
    assert(opt_max);
  }

  cerr << "Testing: getArgPropertyValues" << endl;
  {
    ArgumentString argString;
    auto prop_values = argString.getPropertyValues();
    
    bool opt_min = false;
    bool opt_max = false;
    bool opt_min_val = false;
    bool opt_max_val = false;
  
    for( auto val : prop_values ){
      if(val.first.compare("MIN_LENGTH")==0){
        opt_min = true;
        if(val.second.compare(to_string(0))==0){
          opt_min_val = true;
        }
      }
      if(val.first.compare("MAX_LENGTH")==0){
        opt_max = true;
        if(val.second.compare(to_string((size_t)-1))==0){
          opt_max_val = true;
        }
      }
    }

    assert(opt_min);
    assert(opt_max);
    assert(opt_min_val);
    assert(opt_max_val);
  }


  return 0;
}
