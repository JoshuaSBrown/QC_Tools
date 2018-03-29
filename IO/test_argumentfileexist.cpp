
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>
#include "argumentfileexist.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentFileExist" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentFileExist argFileExist1("test_argumentfileexist.cpp");
    ArgumentFileExist argFileExist2("fake");
  }
  cerr << "Testing: argValid" << endl;
  {
    ArgumentFileExist argFileExist1("test_argumentfileexist.cpp");
    ArgumentFileExist argFileExist2("fake");

    bool valid = argFileExist1.argValid("test_argumentfileexist.cpp");
    assert(valid);
    
    valid = argFileExist2.argValid("fake");
    assert(valid);
  }

  cerr << "Testing: getArgOption" << endl;
  {
    ArgumentFileExist argFileExist1("test_argumentfileexist.cpp");
    ArgumentFileExist argFileExist2("fake");

    bool fileExist = argFileExist1.getArgOption("FILE_EXIST");
    assert(fileExist);
    
    fileExist = argFileExist2.getArgOption("FILE_EXIST");
    assert(fileExist==false);
  }

  return 0;
}
