
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

  cerr << "Testing: log_getHOMO" << endl;
  {
    string path = "../GAUSSIANFILES/90_u/90_pair.log";
    int HOMO = log_getHOMO(path,"Alpha");
    assert(HOMO==16);
    HOMO = log_getHOMO(path,"Beta");
    assert(HOMO==16);
  }

  cerr << "Testing: log_getMOEnergies" << endl;
  {
    string orb_t = "Alpha";
    string path = "../GAUSSIANFILES/90_u/90_pair.log";
    auto energies = log_getMOEnergies(path,orb_t);
    int HOMO = log_getHOMO(path,orb_t);
    int MO = log_countMO(path,orb_t);
 
    assert(static_cast<int>(energies.at(0)*10)==-102);
    assert(static_cast<int>(energies.at(HOMO-1)*1000)==-273);
    assert(static_cast<int>(energies.at(HOMO)*10000)==-124);
    assert(static_cast<int>(energies.at(MO-1)*100)==427);
    
    orb_t = "Beta";
    energies = log_getMOEnergies(path,orb_t);
    HOMO = log_getHOMO(path,orb_t);
    MO = log_countMO(path,orb_t);
    
    assert(static_cast<int>(energies.at(0)*10)==-102);
    assert(static_cast<int>(energies.at(HOMO-1)*1000)==-273);
    assert(static_cast<int>(energies.at(HOMO)*10000)==-124);
    assert(static_cast<int>(energies.at(MO-1)*100)==427);
  }

  cerr << "Testing: log_getCoord" << endl;
  {
    string path = "../GAUSSIANFILES/90_u/90_pair.log";
    auto coord = log_getCoord(path);
    assert(static_cast<int>(coord.at(0).at(0)*1000)==672);
    assert(static_cast<int>(coord.at(1).at(0)*1000)==0);
    assert(static_cast<int>(coord.at(2).at(0)*1000)==0);

    int countAtms = static_cast<int>(coord.at(0).size());
    assert(static_cast<int>(coord.at(0).at(countAtms-1)*1000)==-1242);
    assert(static_cast<int>(coord.at(1).at(countAtms-1)*1000)==0);
    assert(static_cast<int>(coord.at(2).at(countAtms-1)*1000)==4065);
  }

  cerr << "Testing: log_countAtomBasisFunc" << endl;
  {
    string path = "../GAUSSIANFILES/90_u/90_pair.log";
    auto basisCount = log_countAtomBasisFunc(path);
    auto num_atoms = basisCount.size();
    assert(num_atoms==12);
    assert(basisCount.at(0)==19);
    assert(basisCount.at(11)==2);
  }
  return 0;
}
