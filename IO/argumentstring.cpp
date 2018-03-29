
#include "argumentstring.hpp"
#include <memory>

using namespace std;

ArgumentString::ArgumentString(void){
  setArgOption_("MIN_LENGTH",0);
  setArgOption_("MAX_LENGTH",(size_t)-1);
}

void ArgumentString::stringValid(string val){
  if(val.size()>getArgOption("MAX_LENGTH")){
    string err = "The string is larger than allowed "+val+" the "
      "maximum allowed value is "+to_string(getArgOption("MAX_LENGTH"));
    throw invalid_argument(err);
  }else if(val.size()<getArgOption("MIN_LENGTH")){
    string err = "The string is smaller than allowed "+val+" the "
      "maximum allowed value is "+to_string(getArgOption("MIN_LENGTH"));
    throw invalid_argument(err);
  }
}

