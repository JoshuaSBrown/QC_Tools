
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>
#include "argumentstring.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentString" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentString argString;
  }
 
  cerr << "Testing: argValid" << endl;
  {
    ArgumentString argString;
    bool valid = argString.argValid("Hello");
    assert(valid);
  }

  cerr << "Testing: setArgOption" << endl;
  {
    ArgumentString argString;
    size_t val = 3;
    argString.setArgOption("MAX_LENGTH",val);
    argString.argValid("");
    bool excep = false;
    try {
      argString.argValid("Hello");
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      val = 4;
      argString.setArgOption("MAXimum",val);
    } catch (...){
      excep = true;
    }
    assert(excep);
  } 
  return 0;
}
