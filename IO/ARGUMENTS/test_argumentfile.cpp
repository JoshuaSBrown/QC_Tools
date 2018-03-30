
#include "argumentfile.hpp"
#include <cassert>

using namespace std;

int main(void){

  cerr << "Testing: argumentfile" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentFile argFile;
  }

  cerr << "Testing: getArgumentName" << endl;
  {
    ArgumentFile argFile;
    string name = "ARGUMENT_FILE";
    assert(name.compare(argFile.getArgumentName())==0);
  }

  cerr << "Testing: getProperties" << endl;
  {
    ArgumentFile argFile;
    auto props = argFile.getProperties();

    bool file_exist = false;
    bool file_ext = false;

    for(auto prop : props){
      if(prop.compare("PROPERTY_FILE_EXIST")==0){
        file_exist = true;
      }
      if(prop.compare("PROPERTY_FILE_EXT")==0){
        file_ext = true;
      }
    }
    assert(file_exist);
    assert(file_ext);
  }

  cerr << "Testing: getPropertyOptions" << endl;
  {
    ArgumentFile argFile;
    auto prop_opts = argFile.getPropertyOptions();
    
    bool file_must_exist = false;
    bool allowed_ext = false;
  
    for( auto opt : prop_opts ){
      if(opt.compare("ALLOWED_FILE_EXT")==0){
        allowed_ext = true;
      }
      if(opt.compare("FILE_MUST_EXIST")==0){
        file_must_exist = true;
      }
    }

    assert(file_must_exist);
    assert(allowed_ext);
  }

  cerr << "Testing: getArgPropertyValues" << endl;
  {
    ArgumentFile argFile;
    auto prop_values = argFile.getPropertyValues();
    
    bool file_must_exist = false;
    bool allowed_ext = false;
    bool file_must_exist_val = false;
    bool allowed_ext_val = false;
  
    for( auto val : prop_values ){
      if(val.first.compare("ALLOWED_FILE_EXT")==0){
        allowed_ext = true;
        if(val.second[0]=='*'){
          allowed_ext_val = true;
        }
      }
      if(val.first.compare("FILE_MUST_EXIST")==0){
        file_must_exist = true;
        if(val.second.compare("false")==0){
          file_must_exist_val = true;
        }
      }
    }

    assert(file_must_exist);
    assert(allowed_ext);
    assert(file_must_exist_val);
    assert(allowed_ext_val);
  }


  return 0;
}
