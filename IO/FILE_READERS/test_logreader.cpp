
#include "io.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
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

    LogReader lr("../../GAUSSIANFILES/90_unordered/90_pair.log");
    lr.read(); 
    auto orb_info = lr.getOrbitalInfo();
/*
    for(auto pr : orb_info ){
      cout << "Num " << pr.first.first << " Elem " << pr.first.second << endl; 
      map<string,vector<double>> next = pr.second;
      
      for(auto obs : next ){
        cout << "Orb " << obs.first << " ";
        for( auto item : obs.second ){
          cout << item << " ";
        }
        cout << endl;
      }
    }
*/
    auto Soverlap = lr.getOverlapMatrix();
  //  cout << *Soverlap << endl;
    auto Alpha = lr.getOE("Alpha");
   
    auto basisFuncCount = lr.getBasisFuncCount(); 
    for( auto c : basisFuncCount ){
      cout << c << endl;
    }
//    for( auto en : Alpha ) {
//      cout << en << endl;
//    }

//    cout << "Alpha HOMO " << lr.getHOMOLevel("Alpha") << endl;

    auto xyz = lr.getCoords();
    auto x = xyz.at(0);
    auto y = xyz.at(1);
    auto z = xyz.at(2);
    for(size_t ind = 0;ind<x.size();++ind){
      cout << x.at(ind) << " " << y.at(ind) << " " << z.at(ind) << endl;
    }
  }
  return 0;
}
