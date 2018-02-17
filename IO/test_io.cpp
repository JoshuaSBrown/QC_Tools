
#include "io.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

int main(void){
  
  cerr << "Testing: fileExt" << endl;
  {
    string filePath = "Dir1/Dir2/file4.xt";
    string ext = fileExt(filePath);
    assert(ext.compare(".xt")==0);    
  }

  cerr << "Testing: pun_countMO" << endl;
  {
    string path = "../GAUSSIANFILES/90_u/90_pair.pun";
    int MO = pun_countMO(path,"Beta");
    assert(MO==92);
    MO = pun_countMO(path,"Alpha");
    assert(MO==92);
  }
  return 0;
}
