
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
    
    for( auto en : Alpha ) {
      cout << en << endl;
    }

    cout << "Alpha HOMO " << lr.getHOMOLevel("Alpha") << endl;
  }
  return 0;
}
