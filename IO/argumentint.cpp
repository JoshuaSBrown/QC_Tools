
#include "argumentint.hpp"

#include <limits>

using namespace std;

ArgumentInt::ArgumentInt(){
  int minvalue_ = numeric_limits<int>::min();
  int maxvalue_ = numeric_limits<int>::max();

  setArgOption_("MIN",minvalue_);
  setArgOption_("MAX",maxvalue_);
  
}

void ArgumentInt::intValid(int val){
  if(val<getArgOption("MIN")){
    string err = "The value is smaller than allowed "+to_string(val)+" the "
      "minimum allowed value is "+to_string(getArgOption("MIN"));
    throw invalid_argument(err);
  }else if(val>getArgOption("MAX")){
    string err = "The value is greater than allowed "+to_string(val)+" the "
      "maximum allowed value is "+to_string(getArgOption("MAX"));
    throw invalid_argument(err);
  }
}

