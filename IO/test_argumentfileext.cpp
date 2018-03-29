
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>
#include "argumentfileext.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentFileExt" << endl;
  cerr << "Testing: constructor" << endl;
  {
    ArgumentFileExt argFileExt1("*");
    ArgumentFileExt argFileExt2("");
    ArgumentFileExt argFileExt3(".jpg");
    bool excep = false;
    try {
      ArgumentFileExt argFileExt4("ffda.fd");
    } catch(...) {
      excep = true;
    }
    assert(excep);

    set<string> exts = {".png",".gjf"};
    ArgumentFileExt argFileExt5(exts);
  }
  cerr << "Testing: argValid" << endl;
  {
    ArgumentFileExt argFileExt(".jpg");
    bool valid = argFileExt.argValid("dir/file.jpg");
    assert(valid);
    bool excep = false;
    try{
      argFileExt.argValid("dir/file.jp");
    }catch(...){
      excep = true;
    }
    assert(excep);

    ArgumentFileExt argFileExt2("*");
    valid = argFileExt2.argValid("dir/file.jpg");
    assert(valid);
    valid = argFileExt2.argValid("dir/file.jp");
    assert(valid);

    set<string> exts = {".png",".gjf"};
    ArgumentFileExt argFileExt3(exts);
    valid = argFileExt3.argValid("Dir2/Path/File.png");
    assert(valid);
    valid = argFileExt3.argValid("Dir2/Path/File.gjf");
    assert(valid);
    excep = false;
    try{
      argFileExt3.argValid("dir/file.com");
    }catch(...){
      excep = true;
    }
    assert(excep);

  }
  return 0;
}
