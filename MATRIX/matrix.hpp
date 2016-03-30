#ifndef _Matrix_HPP
#define _Matrix_HPP

class Matrix {
	private :
		int rows;
		int cols;
		int shel;
		double * elem;
	public :
		Matrix(void);
		Matrix(int r);
		Matrix(int r, int c);
		Matrix(int r, int c, int s);
		int index(int r, int c, int s);
		void print(void);
		void set_rows(int r);
		void set_cols(int c);
		void set_shel(int s);
		void set_elem(double val);
		void set_elem(double val, int r);
		void set_elem(double val, int r, int c);
		void set_elem(double val, int r, int c, int s);
		int get_rows();
		int get_cols();
		int get_shel();
		int total_elem();
		double get_elem();
		double get_elem(int r);
		double get_elem(int r, int c);
		double get_elem(int r, int c, int s);
};	

#endif
