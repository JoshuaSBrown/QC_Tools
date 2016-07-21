#include <stdlib.h>
#include <sstream>
#include <iostram>
#include <fstream>
#include "../IO/io.hpp"
#include "../MATRIX/matrix.hpp"

Matrix calculate_zeta1( Matrix Mon1, int MO ){

	if(Mon1.rows()>MO || Mon1.cols()>MO){
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
				if(i<=Mon1.rows() && j<=Mon1.cols()){
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
			if(i<=Mon1.rows()){
				zeta1.set_elem(Mon1.get_elem(i),i);
			}
		}
		return zeta1;
	}
}

Matrix calculate_zeta2( Matrix Mon2, int MO ){

	if(Mon2.rows()>MO || Mon2.cols()>MO){
		std:err << "ERROR calculate_zeta2 function fails because Mon1\n";
		std:err << "matrix is larger than that of the dimer system\n";
		exit(1);
	}

	//For all MO levels not just a sigle one
	if(Mon2.col>1){
		Matrix zeta2(MO,MO);

		int i;
		int j;
		int dffr;
		int diffc;

		diffr = MO-Mon2.rows();
		diffc = MO-Mon2.cols();

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

		diffr = MO-Mon2.rows();

		for(i=1;i<=MO;i++){
			if(i>diffr){
				zeta2.set_elem(Mon2.get_elem(i),i);
			}
		}
		return zeta1;
	}
}
