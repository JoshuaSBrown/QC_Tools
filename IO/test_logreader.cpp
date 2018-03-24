
#include "io.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include "logreader.hpp"

using namespace std;

int main(void){
  
  cerr << "Testing: LogReader Constructor" << endl;
  {
    LogReader lr("file.log");
  }

  cerr << "Testing: LogReader read" << endl;
  {
    LogReader lr("../GAUSSIANFILES/90_unordered/90_pair.log");
    lr.read(); 
    auto orb_info = lr.getOrbitalInfo();

    cout << "Size " << orb_info.size() << endl;
  }
  return 0;
}
