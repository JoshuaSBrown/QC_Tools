
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include "punreader.hpp"

using namespace std;

int main(void){
  
  cerr << "Testing: PunReader Constructor" << endl;
  {
    PunReader pr("file.pun");
  }

  cerr << "Testing: PunReader read" << endl;
  {
    PunReader pr("../GAUSSIANFILES/90_unordered/90_pair.pun");
    pr.read(); 
    
    auto m = pr.getCoefsMatrix("Alpha");

    assert(pr.restrictedShell());
//    cout << *m << endl;
  }
  return 0;
}
