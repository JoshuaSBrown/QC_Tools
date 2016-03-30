#include "matrix.hpp"
#include <stdio.h>

int main() {

	printf("Testing Constructors\n");
	printf("Testing: Matrix::Matrix()\n");
	Matrix mat0;
	mat0.print();
	printf("Testing: Matrix::Matrix(int r)\n"); 
	Matrix mat1(3);
	mat1.print();
	printf("Testing: Matrix::Matrix(int r, int c)\n");
	Matrix mat2(2,3);
	mat2.print();
	printf("Testing: Matrix::Matrix(int r, int c, int s)\n");
	Matrix mat3(3,1, 2);
	mat3.print();
	printf("\nTesting: Matrix::set_rows(int r)\n");
	mat0.set_rows(3);
	mat0.print();
	printf("\nTesting: Matrix::set_cols(int c)\n");
	mat0.set_cols(4);
	mat0.print();
	printf("\nTesting: Matrix::set_shel(int s)\n");
	mat0.set_shel(2);
	mat0.print();
	printf("\nTesting: Matrix::set_elem(double val)\n");
	mat0.set_elem(5.4);
	mat0.print();
	printf("\nTesting: Matrix::set_elem(double val, int r)\n");
	mat0.set_elem(4.4, 3);
	mat0.print();
	printf("\nTesting: Matrix::set_elem(double val, int r, int c)\n");
	mat0.set_elem(4.2, 3, 4);
	mat0.print();
	printf("\nTesting: Matrix::set_elem(double val, int r, int c)\n");
	mat0.set_elem(0.3, 3, 4, 2);
	mat0.print();
		
	return 0;
}
