#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "../IO/io.hpp"
#include "../MATRIX/matrix.hpp"

using namespace std;

unordered_map<int,pair<double,string>> findRank(Matrix & Orb_E_Alpha, Matrix & Orb_E_Beta){

  vector<pair<double,string>> all;
  
  auto m_a = Orb_E_Alpha.getCol(1);

//  for(int i=1;i<=Orb_E_Alpha.get_rows();i++){
//    cerr << "Value " << Orb_E_Alpha.get_elem(i) << endl;
//  }
 
  for( auto val : m_a ){
//    cerr << "Value in m_a " << val << endl;
    pair<double,string> pr(val, "Alpha");
    all.push_back(pr);
  }

  if(Orb_E_Beta.get_cols()>0){
    auto m_b = Orb_E_Beta.getCol(1);

    for( auto val : m_b ){
      pair<double,string> pr(val,"Beta");
      all.push_back(pr);
    }
  }
  // Sort the vectors
//  cerr << "Sorting pairs " << endl;
  sort(all.begin(),all.end(),[](const pair<double,string>& P1, const pair<double,string>& P2) -> bool 
  {
    return P1.first < P2.first;
  });
//  cerr << "Finished Sorting " << endl;
  // Now that they are sorted we are going to update the ranks
  int rank = 1;

  unordered_map<int,pair<double,string>> rank_map;

  for( auto pr : all ){
//    cerr << "Rank " << rank << " Energy " << pr.first << " Spin " << pr.second << endl;
    rank_map[rank] = pr;
    rank++;
  }
  return rank_map;
}
/*
Matrix calculate_zeta1( Matrix Mon1, int MO ){

	if(Mon1.get_rows()>MO || Mon1.get_cols()>MO){
		std:err << "ERROR calculate_zeta1 function fails because Mon1\n";
		std:err << "matrix is larger than that of the dimer system\n";
		exit(1);
	}

	//For all MO levels not just a sigle one
	if(Mon1.col>1){
		Matrix zeta1(MO,MO);

		int i;
		int j;

		for(i=1;i<=MO;i++){
			for(j=1;j<=MO;j++){
				if(i<=Mon1.get_rows() && j<=Mon1.get_cols()){
					zeta1.set_elem(Mon1.get_elem(i,j),i,j);
				}
			}
		}
		return zeta1;
	//For a single MO e.g. the HOMO or LUMO
	}else{
		Matrix zeta1(MO,1);
		
		int i;
		for(i=1;i<=MO;i++){
			if(i<=Mon1.get_rows()){
				zeta1.set_elem(Mon1.get_elem(i),i);
			}
		}
		return zeta1;
	}
}

Matrix calculate_zeta2( Matrix Mon2, int MO ){

	if(Mon2.get_rows()>MO || Mon2.get_cols()>MO){
		std:err << "ERROR calculate_zeta2 function fails because Mon1\n";
		std:err << "matrix is larger than that of the dimer system\n";
		exit(1);
	}

	//For all MO levels not just a sigle one
	if(Mon2.get_cols()>1){
		Matrix zeta2(MO,MO);

		int i;
		int j;
		int dffr;
		int diffc;

		diffr = MO-Mon2.get_rows();
		diffc = MO-Mon2.get_cols();

		for(i=1;i<=MO;i++){
			for(j=1;j<=MO;j++){
				if(i>diffr && j>diffc){
					zeta2.set_elem(Mon2.get_elem(i,j),i,j);
				}
			}
		}
		return zeta1;
	//For a single MO e.g. the HOMO or LUMO
	}else{
		Matrix zeta2(MO,1);
		
		int i;
		int dffr;

		diffr = MO-Mon2.get_rows();

		for(i=1;i<=MO;i++){
			if(i>diffr){
				zeta2.set_elem(Mon2.get_elem(i),i);
			}
		}
		return zeta1;
	}
}*/
