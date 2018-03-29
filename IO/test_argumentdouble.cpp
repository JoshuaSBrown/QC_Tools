
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>
#include "argumentdouble.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentDouble" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentDouble argDouble;
  }
 
  cerr << "Testing: argValid" << endl;
  {
    ArgumentDouble argDouble;
    bool valid = argDouble.argValid(0.0);
    assert(valid);
  }

  cerr << "Testing: setArgOption" << endl;
  {
    ArgumentDouble argDouble;
    double val = -1.2;
    argDouble.setArgOption("MIN",val);
    argDouble.argValid(0.0);
    bool excep = false;
    try {
      val = -2.3;
      argDouble.argValid(val);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      val = 3.5;
      argDouble.setArgOption("MAXimum",val);
    } catch (...){
      excep = true;
    }
    assert(excep);
  } 
  return 0;
}
