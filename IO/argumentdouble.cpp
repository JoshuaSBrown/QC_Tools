
#include "argumentdouble.hpp"

#include <limits>

using namespace std;

ArgumentDouble::ArgumentDouble(){
  setArgOption_("MIN",numeric_limits<double>::lowest());
  setArgOption_("MAX",numeric_limits<double>::max());
}

void ArgumentDouble::doubleValid(double val){
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

