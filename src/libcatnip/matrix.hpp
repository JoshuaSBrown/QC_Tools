#ifndef _CATNIP_Matrix_HPP
#define _CATNIP_Matrix_HPP
#include <iostream>
#include <stdlib.h>
#include <vector>

namespace catnip {

class Matrix {
 private:
  int rows;
  int cols;
  int shel;
  double *elem;

 public:
  // constructors
  explicit Matrix(void);
  explicit Matrix(std::vector<double>);
  explicit Matrix(std::vector<int>);
  explicit Matrix(std::vector<std::vector<double>>);
  explicit Matrix(const int r);
  Matrix(const int r, const int c);
  Matrix(const int r, const int c, const int s);

  Matrix &operator=(const Matrix &m);
  // manipulators
  void set_rows(int r);
  void set_cols(int c);
  void set_shel(int s);
  void set_row(std::vector<double> row, int r);
  void set_col(std::vector<double> col, int c);
  int resize(int r, int c);
  void set_elem(double val);
  void set_elem(double val, int r);
  void set_elem(double val, int r, int c);
  void set_elem(double val, int r, int c, int s);
  // accessors
  int index(const int r, const int c, const int s) const;
  int get_rows() const;
  int get_cols() const;
  int get_shel() const;
  int total_elem();
  double get_elem() const;
  double get_elem(const int r) const;
  double get_elem(const int r, const int c) const;
  double get_elem(const int r, const int c, const int s) const;
  double *get_elem_ptr(const int r, const int c) const;
  std::vector<double> get_col(int c) const;
  std::vector<double> get_row(int r) const;

  // WARNING This is not a swap row operation
  // All rows are shifted to make way
  // for the new row/col. 'r_from' is moved to 'r_to' while
  // all rows are appropriately shifted to make this happen.
  void move_row(int r_from, int r_to);
  void move_col(int c_from, int c_to);

  // The intial and final row/col simply change places
  // none of the rows and cols other than the ones swapped
  // are affected
  void swap_row(int r_from, int r_to);
  void swap_col(int c_from, int c_to);

  Matrix getCol(int c) const;
  Matrix getRow(int r) const;
  // Basically switches the rows and columns
  Matrix invert(void) const;
  // Responsible for matching the rows between two matrices they do not need
  // Returns which row in the current matrix matches which row in the one
  // passed in, -1 means there is no match
  // sf is the number of significant figures that will be checked to ensure
  // the same value
  std::vector<int> matchRow(const Matrix & mat, int sf);
  std::vector<int> matchCol(const Matrix &mat, const int sf) const;
};

std::ostream &operator<<(std::ostream &out, Matrix &mat);

Matrix &operator*(const Matrix &mat1, const Matrix &mat2);

// Takes a vector and diagonalized the
// vector in a 2 dimensional matrix where
// everything but the diagonal is assigned
// a 0
Matrix Matrix_diag(const Matrix &mat);

// Copies a matrix and returns the new
// matrix
Matrix Matrix_copy(const Matrix &mat);

// Add two matrices together to create a third
// mat1 will always appear above mat2 in the rows
// of the matrix that is returned.
Matrix Matrix_concatenate_rows(Matrix mat1,const Matrix & mat2);
Matrix Matrix_concatenate_cols(const Matrix &mat1, const Matrix &mat2);

}  // namespace catnip

#endif  // _CATNIP_Matrix_HPP
