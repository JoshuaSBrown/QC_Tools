#ifndef _Matrix_HPP
#define _Matrix_HPP
#include <vector>
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
    void set_row(std::vector<double> row,int r);
    void set_col(std::vector<double> col,int c);
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
    std::vector<double> get_col(int c);
    std::vector<double> get_row(int r);

    // All rows are shifted to make way 
    // for the new row/col
    void move_row(int r_from, int r_to);
    void move_col(int c_from, int c_to);

    // The intial and final row/col simply change places
    // none of the rows and cols other than the ones swapped
    // are affected
    void swap_row(int r_from, int r_to);
    void swap_col(int c_from, int c_to);

    // Responsible for matching the rows between two matrices they do not need
    // Returns which row in the current matrix matches which row in the one passed
    // in, -1 means there is no match
    // sf is the number of sf will be checked to ensure the same value
    std::vector<int> matchRow(Matrix mat,int sf);
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
