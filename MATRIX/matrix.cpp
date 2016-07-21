#include <iostream>
#include <stdio.h>
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

int Matrix::resize( int r, int c ){

	if(r<1){
		printf("ERROR must resize row to a value of 1 or greater\n");
		exit(1);
	}
	if(c<1){
		printf("ERROR must resize column to a value of 1 or greater\n");
		exit(1);
	}
	
	double * temp;
	temp = new double[ r*c ];

	//adding values from old matrix to new
	int i;
	int j;
	if(r<=rows){
		for( i=1; i<=r;i++){
			if(c<=cols){
				for( j=1; j<=c;j++){
					temp[(i-1)*c*1+(j-1)*1+1-1] = elem[index(i,j,1)];
				}
			}else{
				for( j=1; j<=c;j++){
					if(j<=cols){
						temp[(i-1)*c*1+(j-1)*1+1-1] = elem[index(i,j,1)];
					}else{
						temp[(i-1)*c*1+(j-1)*1+1-1] = 0;
					}
				}
			}
		}
	}else{
		for( i=1; i<=r;i++){
			if(c<=cols){
				for( j=1; j<=c;j++){
					if(i<=rows){
						temp[(i-1)*c*1+(j-1)*1+1-1] = elem[index(i,j,1)];
					}else{
						temp[(i-1)*c*1+(j-1)*1+1-1] = 0;
					}
				}
			}else{
				for( j=1; j<=c;j++){
					if(i<=rows && j<=cols){
						temp[(i-1)*c*1+(j-1)*1+1-1] = elem[index(i,j,1)];
					}else{
						temp[(i-1)*c*1+(j-1)*1+1-1] = 0;
					}
				}
			}
		}
	}

  delete elem;
	rows = r;
	cols = c;
	shel = 1;
	elem = temp;

	return 0;
}

int Matrix::index(int r,int c,int s){
	return (r-1)*cols*shel+(c-1)*shel+s-1;
}

std::ostream &operator<<(std::ostream &out, Matrix &mat){
	out << "Matrix Attributes\n";
	out << "rows " << mat.get_rows();
	out << "\ncols " << mat.get_cols();
	out << "\nshel " << mat.get_shel();
	int i;
	int j;
	int k;
	for(k=1;k<=mat.get_shel();k++){
		out << "\nShelf " << k << "\n\t";

		for(j=1;j<=mat.get_cols();j++){
			out << "Col " << j << "\t";
		}
			out << "\n";
		for(i=1;i<=mat.get_rows();i++){
			out << "Row " << i << "\t";
			for(j=1;j<=mat.get_cols();j++){
				out << mat.get_elem(i,j) << "\t";
			}
			out << "\n";
		}
	}
	return out;
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

Matrix Matrix_Multiply( Matrix mat1, Matrix mat2){

	if(mat1.get_shel() != 1 || mat2.get_shel() !=1){
		printf("ERROR Matrix_Multiply only allowed for 2d nxm matrix not 3d nxmxl\n");
		exit(1);
	}
	if(mat1.get_cols() != mat2.get_rows()){
		printf("ERROR Matrix_Multiply only allowed for nxm by lxn matrices\n");
		printf("      second matrix must have same number of colums as first\n");
		printf("      matrix has number of rows\n");
		exit(1);
	}

	Matrix mat3(mat1.get_rows(), mat2.get_cols());

	int i;
	int j;
	int k;
	double sum;

	for(i=1;i<=mat1.get_rows();i++){
		for(j=1;j<=mat2.get_cols();j++){
			sum = 0;
			for(k=1;k<=mat1.get_cols();k++){
				sum += mat1.get_elem(i,k)*mat2.get_elem(k,j);
			}
			mat3.set_elem(sum,i,j);
		}
	}
	return mat3;
}

Matrix &operator* (Matrix &mat1, Matrix &mat2){

	if(mat1.get_shel() != 1 || mat2.get_shel() !=1){
		printf("ERROR Matrix_Multiply only allowed for 2d nxm matrix not 3d nxmxl\n");
		exit(1);
	}
	if(mat1.get_cols() != mat2.get_rows()){
		printf("ERROR Matrix_Multiply only allowed for nxm by lxn matrices\n");
		printf("      second matrix must have same number of colums as first\n");
		printf("      matrix has number of rows\n");
		exit(1);
	}

	Matrix *mat3 = new Matrix(mat1.get_rows(), mat2.get_cols());

	int i;
	int j;
	int k;
	double sum;

	for(i=1;i<=mat1.get_rows();i++){
		for(j=1;j<=mat2.get_cols();j++){
			sum = 0;
			for(k=1;k<=mat1.get_cols();k++){
				sum += (mat1.get_elem(i,k))*(mat2.get_elem(k,j));
			}
			(*mat3).set_elem(sum,i,j);
		}
	}
	return *mat3;

}

Matrix Matrix_Invert( Matrix &mat){
	
	if(mat.get_shel() != 1){
		printf("ERROR Matrix_Invert only allowed for 1d and 2d arrays not 3d\n");
		exit(1);
	}

	Matrix * mat2 = new Matrix(mat.get_cols(), mat.get_rows());

	int i;
	int j;
	double val;

	for(i=1;i<=mat.get_rows();i++){
		for(j=1;j<=mat.get_cols();j++){
			val = mat.get_elem(i,j);
			(*mat2).set_elem(val,j,i);	
		}
	}
	return *mat2;
}

Matrix Matrix_getRow( Matrix mat, int R){

	if(mat.get_rows()<R){
		printf("ERROR Matrix_getRow cannot return R %d",R);
		printf(" because mat has only %d rows\n.",mat.get_rows());
		exit(1);
	}

	Matrix mat2( 1, mat.get_cols(), mat.get_shel());

	int j;
	int k;
	double val;

	for(j=1;j<=mat.get_cols();j++){
		for(k=1;k<=mat.get_shel();k++){
			val = mat.get_elem(R,j,k);
			mat2.set_elem(val,1,j,k);	
		}
	}
	return mat2;
}

Matrix Matrix_getCol( Matrix mat, int C){
	
	if(mat.get_cols()<C){
		printf("ERROR Matrix_getCol cannot return C %d",C);
		printf(" because mat has only %d cols\n.",mat.get_cols());
		exit(1);
	}
	Matrix mat2( mat.get_cols(),1, mat.get_shel());

	int i;
	int k;
	double val;

	for(i=1;i<=mat.get_rows();i++){
		for(k=1;k<=mat.get_shel();k++){
			val = mat.get_elem(i,C,k);
			mat2.set_elem(val,i,1,k);	
		}
	}
	return mat2;
}

Matrix Matrix_diag(Matrix mat){

	if(mat.get_cols()>1 && mat.get_rows()>1){
		printf("ERROR Matrix_diag can only create a diagonal matrix\n");
		printf("from a vector.\n");
		exit(1);
	}

	if(mat.get_shel()>1){
		printf("ERROR Matrix_diag cannot create diagonal matrix from\n");
		printf("a 3d matrix must be passed a vector.\n");
		exit(1);
	}

	int i;
	int len;
	double val;

	if(mat.get_cols()>mat.get_rows()){
		len = mat.get_cols();
		Matrix mat2(len,len);
		for(i=1;i<=len;i++){
			val = mat.get_elem(1,i);
			mat2.set_elem(val,i,i);
		}
 		return mat2;
	}else{
		len = mat.get_rows();
		Matrix mat2(len,len);
		for(i=1;i<=len;i++){
			val = mat.get_elem(i,1);
			mat2.set_elem(val,i,i);
		}
 		return mat2;
	}


}

Matrix Matrix_copy( Matrix mat){

	Matrix mat2(mat.get_rows(),mat.get_cols(),mat.get_shel());

	int i;
	int j;
	int k;
	double val;

	for( i=1;i<=mat.get_rows();i++){
		for(j=1;j<=mat.get_cols();j++){
			for(k=1;k<=mat.get_shel();k++){
				val = mat.get_elem(i,j,k);
				mat2.set_elem(val,i,j,k);
			}
		}
	}

	return mat2;
}

Matrix Matrix_concatenate_rows( Matrix mat1, Matrix mat2 ){

	//For this function to work both mat1 and mat2 must have
	//the same number of columns and shelves

	if(mat1.get_cols()!=mat2.get_cols()){
		std::cerr << "ERROR to concatenate the rows mat1 and mat2 must\n";
		std::cerr << " 			have the same number of cols!\n";
		Matrix m;
		return m;
	}
	
	if(mat1.get_shel()!=mat2.get_shel()){
		std::cerr << "ERROR to concatenate the rows mat1 and mat2 must\n";
		std::cerr << " 			have the same number of shelves!\n";
		Matrix m;
		return m;
	}

	int totalrows;
	int rowsMat1 = mat1.get_rows();
	totalrows = rowsMat1+mat2.get_rows();
	Matrix mat3(totalrows,mat1.get_cols(),mat1.get_shel());

	int i;
	int j;
	int k;
	double val;

	for(i=1;i<=mat1.get_rows();i++){
		for(j=1;j<=mat1.get_cols();j++){
			for(k=1;k<=mat1.get_shel();k++){
				val = mat1.get_elem(i,j,k);
				mat3.set_elem(val,i,j,k);
			}
		}
	}

	for(i=1;i<=mat2.get_rows();i++){
		for(j=1;j<=mat2.get_cols();j++){
			for(k=1;k<=mat2.get_shel();k++){
				val = mat2.get_elem(i,j,k);
				mat3.set_elem(val,i+rowsMat1,j,k);
			}
		}
	}

	return mat3;

}

Matrix Matrix_concatenate_cols( Matrix mat1, Matrix mat2 ){

	//For this function to work both mat1 and mat2 must have
	//the same number of columns and shelves

	if(mat1.get_rows()!=mat2.get_rows()){
		std::cerr << "ERROR to concatenate the cols mat1 and mat2 must\n";
		std::cerr << " 			have the same number of rows!\n";
		Matrix m;
		return m;
	}
	
	if(mat1.get_shel()!=mat2.get_shel()){
		std::cerr << "ERROR to concatenate the rows mat1 and mat2 must\n";
		std::cerr << " 			have the same number of shelves!\n";
		Matrix m;
		return m;
	}

	int totalcols;
	int colsMat1 = mat1.get_cols();
	totalcols = colsMat1+mat2.get_cols();
	Matrix mat3(mat1.get_rows(),totalcols,mat1.get_shel());

	int i;
	int j;
	int k;
	double val;

	for(i=1;i<=mat1.get_rows();i++){
		for(j=1;j<=mat1.get_cols();j++){
			for(k=1;k<=mat1.get_shel();k++){
				val = mat1.get_elem(i,j,k);
				mat3.set_elem(val,i,j,k);
			}
		}
	}

	for(i=1;i<=mat2.get_rows();i++){
		for(j=1;j<=mat2.get_cols();j++){
			for(k=1;k<=mat2.get_shel();k++){
				val = mat2.get_elem(i,j,k);
				mat3.set_elem(val,i,j+colsMat1,k);
			}
		}
	}

	return mat3;

}
