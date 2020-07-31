
#include "property_numeric.hpp"

#include "propertyobject.hpp"

#include <limits>
#include <typeinfo>

using namespace catnip;
using namespace std;

void PropertyNumeric::setPropOption_(const Option & option, const std::any & val) {
  if ( val.type() != typeid(double)){
    string err = "The Min and Max options of the numeric property must be a double.";
    throw invalid_argument(err);
  }
  PropertyObject::setPropOption_(option, val);
}

bool PropertyNumeric::propValid(const std::any & val) {

  if( val.type() != typeid(int) || val.type() != typeid(double) ){
    string err = "The value provided to the property is not a supported type.";
    throw invalid_argument(err);
  }
  // If the properties have not been set then they will be 
  // valid as long as they are of a numeric type
  double converted_val;
  if ( val.type() == typeid(int) ){
    converted_val = static_cast<double>(any_cast<int>(val));
  } else {
    converted_val = any_cast<double>(val);
  }

  // ignore it otherwise, there is no min option 
  double min_val = any_cast<double>(options_[Option::MIN]);
  if ( converted_val < min_val ) {
    string err = "The value provided to the property is less than the minimum allowed.";
    throw invalid_argument(err);
  }
  // ignore it otherwise, there is no min option 
  double max_val = any_cast<double>(options_[Option::MAX]);
  if ( converted_val < max_val ) {
    string err = "The value provided to the property is greater than the maximum allowed.";
    throw invalid_argument(err);
  }
  return true;
}
