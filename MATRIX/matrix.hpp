#ifndef _Matrix_HPP
#define _Matrix_HPP

#include <stdlib.h>
#include <iostream>

class Matrix {
	private :
		int rows;
		int cols;
		int shel;
		double * elem;
	public :
		//constructors
		Matrix(void);
		Matrix(int r);
		Matrix(int r, int c);
		Matrix(int r, int c, int s);
		//manipulators
		void set_rows(int r);
		void set_cols(int c);
		void set_shel(int s);
		int resize( int r, int c );
		void set_elem(double val);
		void set_elem(double val, int r);
		void set_elem(double val, int r, int c);
		void set_elem(double val, int r, int c, int s);
		//accessors
		int index(int r, int c, int s);
		int get_rows();
		int get_cols();
		int get_shel();
		int total_elem();
		double get_elem();
		double get_elem(int r);
		double get_elem(int r, int c);
		double get_elem(int r, int c, int s);
};	

std::ostream &operator<<(std::ostream &out, Matrix &mat); 

Matrix &operator*(Matrix &mat1, Matrix &mat2); 

Matrix Matrix_Invert( Matrix &mat );

Matrix Matrix_getRow( Matrix mat, int R );

Matrix Matrix_getCol( Matrix mat, int C );

//Takes a vector and diagonalized the 
//vector in a 2 dimensional matrix where
//everything but the diagonal is assigned
//a 0
Matrix Matrix_diag( Matrix mat );

//Copies a matrix and returns the new
//matrix
Matrix Matrix_copy( Matrix mat );

//Add two matrices together to create a third
//mat1 will always appear above mat2 in the rows
//of the matrix that is returned.
Matrix Matrix_concatenate_rows( Matrix mat1, Matrix mat2 );
Matrix Matrix_concatenate_cols( Matrix mat1, Matrix mat2 );
#endif
