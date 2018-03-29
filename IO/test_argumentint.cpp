
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>
#include "argumentint.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentInt" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentInt argInt;
  }
 
  cerr << "Testing: argValid" << endl;
  {
    ArgumentInt argInt;
    bool valid = argInt.argValid(0);
    assert(valid);
  }

  cerr << "Testing: setArgOption" << endl;
  {
    ArgumentInt argInt;
    argInt.setArgOption("MIN",0);
    argInt.argValid(0);
    bool excep = false;
    try {
      argInt.argValid(-1);
    } catch (...) {
      excep = true;
    }
    assert(excep);

    excep = false;
    try {
      argInt.setArgOption("MAXimum",3);
    } catch (...){
      excep = true;
    }
    assert(excep);
  } 
  return 0;
}
