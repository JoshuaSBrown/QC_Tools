
#include "io.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include "../MATRIX/matrix.hpp"

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

  cerr << "Testing: pun_getMO" << endl;
  {
    
    string path = "../GAUSSIANFILES/90_u/90_pair.pun";
    int MO = pun_countMO(path,"Beta");
    Matrix mat;
    pun_getMO(path,&mat,"Beta");
    
    assert(static_cast<int>(mat.get_elem(1,1)*10000)==7018);
    assert(static_cast<int>(mat.get_elem(1,2)*10000)==345);
    assert(static_cast<int>(mat.get_elem(1,3)*10000)==1);

    assert(static_cast<int>(mat.get_elem(1,5)*100000)==2);    
 
    assert(static_cast<int>(mat.get_elem(MO,MO-1)*1000)==77);
    assert(static_cast<int>(mat.get_elem(MO,MO)*100)==-27);
  
  }

  cerr << "Testing: log_countMO" << endl;
  {
    string path = "../GAUSSIANFILES/90_u/90_pair.log";
    int MO = log_countMO(path,"Alpha");
    assert(MO==92);
    MO = log_countMO(path,"Beta");
    assert(MO==92);
  }
  return 0;
}
