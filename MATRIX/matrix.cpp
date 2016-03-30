#include <iostream>
#include <cstdlib>
#include <exception>
#include "matrix.hpp"

Matrix::Matrix() {
	rows = 1;
	cols = 1;
	shel = 1;
	elem = new double[rows*cols];
	elem[0] = 0;
}

Matrix::Matrix(int r) {
	if(r<1){
		printf("ERROR negative number of rows submitted to Matrix\n");
		exit(1);
	}
	int i;
	rows = r;
	cols = 1;
	shel = 1;
	
	elem = new double[rows];
	for(i=1;i<=rows;i++){
		elem[i-1] = 0;
	}
}

Matrix::Matrix(int r, int c) {
	if(r<1){
		printf("ERROR negative number of rows submitted to Matrix\n");
		exit(1);
	} else if(c<1){
		printf("ERROR negative number of cols submitted to Matrix\n");
		exit(1);
	}
	int i;
	int j;
	rows = r;
	cols = c;
	shel = 1;
	elem = new double[rows*cols];
	for(i=1;i<=rows;i++){
		for(j=1;j<=cols;j++){
			elem[(i-1)*cols+j-1] = 0;
		}
	}
}

Matrix::Matrix(int r, int c, int s) {
	if(r<1){
		printf("ERROR negative number of rows submitted to Matrix\n");
		exit(1);
	} else if(c<1){
		printf("ERROR negative number of cols submitted to Matrix\n");
		exit(1);
	} else if(s<1){
		printf("ERROR negative number of shelves submitted to Matrix\n");
		exit(1);
	}
	int i;
	int j;
	int k;
	rows = r;
	cols = c;
	shel = s;
	elem = new double[rows*cols*shel];
	for(i=1;i<=rows;i++){
		for(j=1;j<=cols;j++){
			for(k=1;k<=shel;k++){
				elem[(i-1)*cols*shel+(j-1)*shel+k-1] = 0;
			}
		}
	}
}

int Matrix::index(int r,int c,int s){
	return (r-1)*cols*shel+(c-1)*shel+s-1;
}

void Matrix::print(){
	printf("\nMatrix Attributes\n");
	printf("rows %d\n",rows);
	printf("cols %d\n",cols);
	printf("shel %d\n",shel);
	int i;
	int j;
	int k;
	for(k=1;k<=shel;k++){
		printf("\nShelf %d\n",k);
		printf("\t");
		for(j=1;j<=cols;j++){
			printf("Col %d\t",j);
		}
			printf("\n");
		for(i=1;i<=rows;i++){
			printf("Row %d\t",i);
			for(j=1;j<=cols;j++){
				printf("%g\t",elem[index(i,j,k)]);
			}
			printf("\n");
		}
	}
}

void Matrix::set_rows(int r){
	if(r<0){
		printf("ERROR negative number submitted to set_rows\n");
		exit(1);
	}
	int i;
	int j;
	int k;

	if(r>rows){
	
		double * temp = new double[r*cols*shel];
		for(i=1;i<=r;i++){
			for(j=1;j<=cols;j++){
				for(k=1;k<=shel;k++){
					if(r<=rows){
						temp[index(i,j,k)] = elem[index(i,j,k)];
					} else {
						temp[index(i,j,k)] = 0;
					}
				}
			}
		}
		delete elem;
		rows = r;
		elem = temp;
	}else if(r<rows){
		
		double * temp = new double[r*cols*shel];
		printf("WARNING reducing matrix rows below previous value\n");
		printf("could lose data in the process\n");
		for(i=1;i<=r;i++){
			for(j=1;j<=cols;j++){
				for(k=1;k<=shel;k++){
					temp[index(i,j,k)] = elem[index(i,j,k)];
				}
			}
		}
		delete elem;
		rows = r;
		elem = temp;
	}
}

void Matrix::set_cols(int c){
	if(c<0){
		printf("ERROR negative number submitted to set_cols\n");
		exit(1);
	}
	int i;
	int j;
	int k;

	if(c>cols){
	
		double * temp = new double[rows*c*shel];
		for(i=1;i<=rows;i++){
			for(j=1;j<=c;j++){
				for(k=1;k<=shel;k++){
					if(c<=cols){
						temp[index(i,j,k)] = elem[index(i,j,k)];
					} else {
						temp[index(i,j,k)] = 0;
					}
				}
			}
		}
		delete elem;
		cols = c;
		elem = temp;
	}else if(c<cols){
		
		double * temp = new double[rows*c*shel];
		printf("WARNING reducing matrix cols below previous value\n");
		printf("could lose data in the process\n");
		for(i=1;i<=rows;i++){
			for(j=1;j<=c;j++){
				for(k=1;k<=shel;k++){
					temp[index(i,j,k)] = elem[index(i,j,k)];
				}
			}
		}
		delete elem;
		cols = c;
		elem = temp;
	}
}

void Matrix::set_shel(int s){
	if(s<0){
		printf("ERROR negative number submitted to set_shel\n");
		exit(1);
	}
	int i;
	int j;
	int k;

	if(s>shel){
	
		double * temp = new double[rows*cols*s];
		for(i=1;i<=rows;i++){
			for(j=1;j<=cols;j++){
				for(k=1;k<=s;k++){
					if(s<=shel){
						temp[index(i,j,k)] = elem[index(i,j,k)];
					} else {
						temp[index(i,j,k)] = 0;
					}
				}
			}
		}
		delete elem;
		shel = s;
		elem = temp;
	}else if(s<shel){
		
		double * temp = new double[rows*cols*s];
		printf("WARNING reducing matrix shel below previous value\n");
		printf("could lose data in the process\n");
		for(i=1;i<=rows;i++){
			for(j=1;j<=cols;j++){
				for(k=1;k<=s;k++){
					temp[index(i,j,k)] = elem[index(i,j,k)];
				}
			}
		}
		delete elem;
		shel = s;
		elem = temp;
	}
}

void Matrix::set_elem(double val){
	elem[0] = val;
}

void Matrix::set_elem(double val, int r){
	if(r>rows){
		printf("ERROR r value is greater than rows in set_elem(double val, int r)\n");
		exit(1);
	}else if(r<1){
		printf("ERROR r value is less than or equal to 0 in set_elem(double val, int r)\n");
		exit(1);
	};
	
	elem[index(r,1,1)] = val;
}

void Matrix::set_elem(double val, int r, int c){
	if(r>rows){
		printf("ERROR r value is greater than rows in set_elem(double val, int r, int c)\n");
		exit(1);
	}else if(r<1){
		printf("ERROR r value is less than or equal to 0 in set_elem(double val, int r, int c)\n");
		exit(1);
	};
	if(c>cols){
		printf("ERROR c value is greater than rows in set_elem(double val, int r, int c)\n");
		exit(1);
	}else if(c<1){
		printf("ERROR c value is less than or equal to 0 in set_elem(double val, int r, int c)\n");
		exit(1);
	};

	elem[index(r,c,1)] = val;
}

void Matrix::set_elem(double val, int r, int c, int s){
	if(r>rows){
		printf("ERROR r value is greater than rows in set_elem(double val, int r, int c, int s)\n");
		exit(1);
	}else if(r<1){
		printf("ERROR r value is less than or equal to 0 in set_elem(double val, int r, int c, int s)\n");
		exit(1);
	};
	if(c>cols){
		printf("ERROR c value is greater than rows in set_elem(double val, int r, int c, int s)\n");
		exit(1);
	}else if(c<1){
		printf("ERROR c value is less than or equal to 0 in set_elem(double val, int r, int c, int s)\n");
		exit(1);
	};
	if(s>shel){
		printf("ERROR s value is greater than rows in set_elem(double val, int r, int c, int s)\n");
		exit(1);
	}else if(s<1){
		printf("ERROR s value is less than or equal to 0 in set_elem(double val, int r, int c, int s)\n");
		exit(1);
	};

	elem[index(r,c,s)]=val;
}

int Matrix::get_rows(){
	return rows;
}

int Matrix::get_cols(){
	return cols;
}

int Matrix::get_shel(){
	return shel;
}

int Matrix::total_elem () {
	return rows*cols*shel;
}

double Matrix::get_elem(){
	return elem[0];
}

double Matrix::get_elem(int r){
	if(r<1){
		printf("ERROR get_elem(int r): 0 or negative row submitted\n");
		exit(1);
	}else if(r>rows){
		printf("ERROR get_elem(int r): row value larger than the scope of the Matrix\n");
		exit(1);
	}
	return elem[index(r,1,1)];
}

double Matrix::get_elem(int r, int c){
	if(r<1){
		printf("ERROR get_elem(int r, int c): 0 or negative row submitted\n");
		exit(1);
	}else if(r>rows){
		printf("ERROR get_elem(int r, int c): row value larger than the scope of the Matrix\n");
		exit(1);
	}else if(c<1){
		printf("ERROR get_elem(int r, int c): 0 or negative col submitted\n");
		exit(1);
	}else if(c>cols){
		printf("ERROR get_elem(int r, int c): col value larger than the scope of the Matrix\n");
		exit(1);
	}
	return elem[index(r,c,1)];
}

double Matrix::get_elem(int r, int c, int s){
	if(r<1){
		printf("ERROR get_elem(int r, int c, int s): 0 or negative row submitted\n");
		exit(1);
	}else if(r>rows){
		printf("ERROR get_elem(int r, int c, int s): row value larger than the scope of the Matrix\n");
		exit(1);
	}else if(c<1){
		printf("ERROR get_elem(int r, int c, int s): 0 or negative col submitted\n");
		exit(1);
	}else if(c>cols){
		printf("ERROR get_elem(int r, int c, int s): col value larger than the scope of the Matrix\n");
		exit(1);
	}else if(s<1){
		printf("ERROR get_elem(int r, int c, int s): 0 or negative shelf submitted\n");
		exit(1);
	}else if(s>shel){
		printf("ERROR get_elem(int r, int c, int s): shelf value larger than the scope of the Matrix\n");
		exit(1);
	}
	return elem[index(r,c,s)];
}

