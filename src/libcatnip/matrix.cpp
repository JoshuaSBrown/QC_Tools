#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "matrix.hpp"

using namespace std;

namespace catnip {

Matrix Matrix_diag(const Matrix &mat) {

  if (mat.get_cols() > 1 && mat.get_rows() > 1) {
    throw invalid_argument(
        "ERROR Matrix_diag can only create a diagonal "
        "matrix from a vector.\n");
  }

  if (mat.get_shel() > 1) {
    throw invalid_argument(
        "ERROR Matrix_diag cannot create diagonal matrix "
        "from a 3d matrix, it must be passed a vector.\n");
  }

  if (mat.get_cols() > mat.get_rows()) {
    int len = mat.get_cols();
    Matrix mat2(len, len);
    for (int i = 1; i <= len; i++) {
      double val = mat.get_elem(1, i);
      mat2.set_elem(val, i, i);
    }
    return mat2;
  } else {
    int len = mat.get_rows();
    Matrix mat2(len, len);
    for (int i = 1; i <= len; i++) {
      double val = mat.get_elem(i, 1);
      mat2.set_elem(val, i, i);
    }
    return mat2;
  }
}

Matrix Matrix_copy(const Matrix &mat) {

  Matrix mat2(mat.get_rows(), mat.get_cols(), mat.get_shel());

  int i;
  int j;
  int k;
  double val;

  for (i = 1; i <= mat.get_rows(); i++) {
    for (j = 1; j <= mat.get_cols(); j++) {
      for (k = 1; k <= mat.get_shel(); k++) {
        val = mat.get_elem(i, j, k);
        mat2.set_elem(val, i, j, k);
      }
    }
  }

  return mat2;
}

Matrix Matrix_concatenate_rows(Matrix mat1,const Matrix & mat2) {

  // For this function to work both mat1 and mat2 must have
  // the same number of columns and shelves

  if (mat1.get_cols() != mat2.get_cols()) {
    std::cerr << "ERROR to concatenate the rows mat1 and mat2 must\n";
    std::cerr << " 			have the same number of cols!\n";
    Matrix m;
    return m;
  }

  if (mat1.get_shel() != mat2.get_shel()) {
    std::cerr << "ERROR to concatenate the rows mat1 and mat2 must\n";
    std::cerr << " 			have the same number of shelves!\n";
    Matrix m;
    return m;
  }

  int totalrows;
  int rowsMat1 = mat1.get_rows();
  totalrows = rowsMat1 + mat2.get_rows();
  Matrix mat3(totalrows, mat1.get_cols(), mat1.get_shel());

  int i;
  int j;
  int k;
  double val;

  for (i = 1; i <= mat1.get_rows(); i++) {
    for (j = 1; j <= mat1.get_cols(); j++) {
      for (k = 1; k <= mat1.get_shel(); k++) {
        val = mat1.get_elem(i, j, k);
        mat3.set_elem(val, i, j, k);
      }
    }
  }

  for (i = 1; i <= mat2.get_rows(); i++) {
    for (j = 1; j <= mat2.get_cols(); j++) {
      for (k = 1; k <= mat2.get_shel(); k++) {
        val = mat2.get_elem(i, j, k);
        mat3.set_elem(val, i + rowsMat1, j, k);
      }
    }
  }

  return mat3;
}

Matrix Matrix_concatenate_cols(const Matrix &mat1, const Matrix &mat2) {

  // For this function to work both mat1 and mat2 must have
  // the same number of columns and shelves

  if (mat1.get_rows() != mat2.get_rows()) {
    std::cerr << "ERROR to concatenate the cols mat1 and mat2 must\n";
    std::cerr << " 			have the same number of rows!\n";
    Matrix m;
    return m;
  }

  if (mat1.get_shel() != mat2.get_shel()) {
    std::cerr << "ERROR to concatenate the rows mat1 and mat2 must\n";
    std::cerr << " 			have the same number of shelves!\n";
    Matrix m;
    return m;
  }

  int totalcols;
  int colsMat1 = mat1.get_cols();
  totalcols = colsMat1 + mat2.get_cols();
  Matrix mat3(mat1.get_rows(), totalcols, mat1.get_shel());

  int i;
  int j;
  int k;
  double val;

  for (i = 1; i <= mat1.get_rows(); i++) {
    for (j = 1; j <= mat1.get_cols(); j++) {
      for (k = 1; k <= mat1.get_shel(); k++) {
        val = mat1.get_elem(i, j, k);
        mat3.set_elem(val, i, j, k);
      }
    }
  }

  for (i = 1; i <= mat2.get_rows(); i++) {
    for (j = 1; j <= mat2.get_cols(); j++) {
      for (k = 1; k <= mat2.get_shel(); k++) {
        val = mat2.get_elem(i, j, k);
        mat3.set_elem(val, i, j + colsMat1, k);
      }
    }
  }

  return mat3;
}

string double_tos(double f, int nd) {
  ostringstream ostr;
  int tens = stod("1" + string(nd, '0'));
  ostr << round(f * tens) / tens;
  return ostr.str();
}

std::ostream &operator<<(std::ostream &out, Matrix &mat) {
  out << "Matrix Attributes\n";
  out << "rows " << mat.get_rows();
  out << "\ncols " << mat.get_cols();
  out << "\nshel " << mat.get_shel();
  int i;
  int j;
  int k;
  for (k = 1; k <= mat.get_shel(); k++) {
    out << "\nShelf " << k << "\n\t";

    for (j = 1; j <= mat.get_cols(); j++) {
      out << "Col " << j << "\t";
    }
    out << "\n";
    for (i = 1; i <= mat.get_rows(); i++) {
      out << "Row " << i << "\t";
      for (j = 1; j <= mat.get_cols(); j++) {
        out << mat.get_elem(i, j) << "\t";
      }
      out << "\n";
    }
  }
  return out;
}

Matrix &operator*(const Matrix &mat1, const Matrix &mat2) {

  if (mat1.get_shel() != 1 || mat2.get_shel() != 1) {
    printf(
        "ERROR Matrix_Multiply only allowed for 2d nxm matrix not 3d nxmxl\n");
    exit(1);
  }
  if (mat1.get_cols() != mat2.get_rows()) {
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

  for (i = 1; i <= mat1.get_rows(); i++) {
    for (j = 1; j <= mat2.get_cols(); j++) {
      sum = 0;
      for (k = 1; k <= mat1.get_cols(); k++) {
        sum += (mat1.get_elem(i, k)) * (mat2.get_elem(k, j));
      }
      (*mat3).set_elem(sum, i, j);
    }
  }
  return *mat3;
}

}  // namespace catnip

using namespace catnip;

Matrix::Matrix() {
  rows = 1;
  cols = 1;
  shel = 1;
  elem = new double[rows * cols];
  elem[0] = 0;
}

Matrix::Matrix(vector<double> v_data) {
  rows = v_data.size();
  cols = 1;
  shel = 1;
  elem = new double[rows * cols];

  int r = 1;
  int c = 1;
  for (auto val : v_data) {
    elem[(r - 1) * cols + c - 1] = val;
    ++r;
  }
}

Matrix::Matrix(vector<int> v_data) {
  rows = v_data.size();
  cols = 1;
  shel = 1;
  elem = new double[rows * cols];

  int r = 1;
  int c = 1;
  for (auto val : v_data) {
    elem[(r - 1) * cols + c - 1] = static_cast<double>(val);
    ++r;
  }
}

Matrix::Matrix(vector<vector<double>> vv_data) {
  rows = vv_data.size();
  cols = vv_data.at(0).size();
  shel = 1;
  elem = new double[rows * cols];
  elem = new double[rows * cols];

  int r = 1;
  for (auto v : vv_data) {
    int c = 1;
    if (static_cast<int>(v.size()) != cols) {
      throw invalid_argument(
          "vector<vector<double>> not consistent "
          "in matrix constructor ");
    }
    for (auto val : v) {
      elem[(r - 1) * cols + c - 1] = val;
      ++c;
    }
    ++r;
  }
}

Matrix::Matrix(const int r) {
  if (r < 0) {
    cerr << "ERROR rows of matrix must be greater than or equal to 0" << endl;
    cerr << " you have r " << r << endl;
    exit(1);
  }
  if (r == 0) {
    rows = 0;
    cols = 0;
    shel = 0;
    elem = nullptr;
  } else {
    rows = r;
    cols = 1;
    shel = 1;

    elem = new double[rows];
    for (int i = 1; i <= rows; i++) {
      elem[i - 1] = 0;
    }
  }
}

Matrix::Matrix(const int r, const int c) {
  if (r < 1) {
    printf("ERROR negative number of rows submitted to Matrix\n");
    exit(1);
  } else if (c < 1) {
    printf("ERROR negative number of cols submitted to Matrix\n");
    exit(1);
  }
  rows = r;
  cols = c;
  shel = 1;
  elem = new double[rows * cols];
  for (int i = 1; i <= rows; i++) {
    for (int j = 1; j <= cols; j++) {
      elem[(i - 1) * cols + j - 1] = 0;
    }
  }
}

Matrix::Matrix(const int r, const int c, const int s) {
  if (r < 1) {
    printf("ERROR negative number of rows submitted to Matrix\n");
    exit(1);
  } else if (c < 1) {
    printf("ERROR negative number of cols submitted to Matrix\n");
    exit(1);
  } else if (s < 1) {
    printf("ERROR negative number of shelves submitted to Matrix\n");
    exit(1);
  }
  rows = r;
  cols = c;
  shel = s;
  elem = new double[rows * cols * shel];
  for (int i = 1; i <= rows; i++) {
    for (int j = 1; j <= cols; j++) {
      for (int k = 1; k <= shel; k++) {
        elem[(i - 1) * cols * shel + (j - 1) * shel + k - 1] = 0;
      }
    }
  }
}

int Matrix::resize(int r, int c) {

  if (r < 1) {
    printf("ERROR must resize row to a value of 1 or greater\n");
    exit(1);
  }
  if (c < 1) {
    printf("ERROR must resize column to a value of 1 or greater\n");
    exit(1);
  }

  double *temp;
  temp = new double[r * c];

  // adding values from old matrix to new
  if (r <= rows) {
    for (int i = 1; i <= r; i++) {
      if (c <= cols) {
        for (int j = 1; j <= c; j++) {
          temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = elem[index(i, j, 1)];
        }
      } else {
        for (int j = 1; j <= c; j++) {
          if (j <= cols) {
            temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = elem[index(i, j, 1)];
          } else {
            temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = 0;
          }
        }
      }
    }
  } else {
    for (int i = 1; i <= r; i++) {
      if (c <= cols) {
        for (int j = 1; j <= c; j++) {
          if (i <= rows) {
            temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = elem[index(i, j, 1)];
          } else {
            temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = 0;
          }
        }
      } else {
        for (int j = 1; j <= c; j++) {
          if (i <= rows && j <= cols) {
            temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = elem[index(i, j, 1)];
          } else {
            temp[(i - 1) * c * 1 + (j - 1) * 1 + 1 - 1] = 0;
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

Matrix &Matrix::operator=(const Matrix &m) {
  this->resize(m.get_rows(), m.get_cols());

  for (int i = 1; i <= m.get_rows(); ++i) {
    for (int j = 1; j <= m.get_cols(); ++j) {
      this->set_elem(m.get_elem(i, j), i, j);
    }
  }
  return *this;
}

int Matrix::index(const int r, const int c, const int s) const {
  return (r - 1) * cols * shel + (c - 1) * shel + s - 1;
}

void Matrix::set_row(vector<double> row, int r) {

  if (get_cols() != static_cast<int>(row.size())) {
    throw invalid_argument("vector is not the same size as matrix");
  }

  for (int j = 1; j <= get_cols(); j++) {
    if (r <= rows) {
      elem[index(r, j, 1)] = row.at(j - 1);
    } else {
      elem[index(r, j, 1)] = 0;
    }
  }
}

void Matrix::set_col(vector<double> col, int c) {

  if (get_cols() != static_cast<int>(col.size())) {
    throw invalid_argument("vector is not the same size as matrix");
  }
  for (int i = 1; i <= get_rows(); ++i) {
    if (c <= cols) {
      elem[index(i, c, 1)] = col.at(i - 1);
    } else {
      elem[index(i, c, 1)] = 0;
    }
  }
}

void Matrix::set_rows(int r) {
  if (r < 0) {
    printf("ERROR negative number submitted to set_rows\n");
    exit(1);
  }
  if (r > rows) {

    double *temp = new double[r * cols * shel];
    for (int i = 1; i <= r; i++) {
      for (int j = 1; j <= cols; j++) {
        for (int k = 1; k <= shel; k++) {
          if (r <= rows) {
            temp[index(i, j, k)] = elem[index(i, j, k)];
          } else {
            temp[index(i, j, k)] = 0;
          }
        }
      }
    }
    delete elem;
    rows = r;
    elem = temp;
  } else if (r < rows) {

    double *temp = new double[r * cols * shel];
    printf("WARNING reducing matrix rows below previous value\n");
    printf("could lose data in the process\n");
    for (int i = 1; i <= r; i++) {
      for (int j = 1; j <= cols; j++) {
        for (int k = 1; k <= shel; k++) {
          temp[index(i, j, k)] = elem[index(i, j, k)];
        }
      }
    }
    delete elem;
    rows = r;
    elem = temp;
  }
}

vector<double> Matrix::get_col(int c) const{
  vector<double> col_data;
  for (int i = 1; i <= rows; i++) {
    col_data.push_back(elem[index(i, c, 1)]);
  }
  return col_data;
}

vector<double> Matrix::get_row(int r) const {
  vector<double> row_data;
  for (int i = 1; i <= cols; i++) {
    row_data.push_back(elem[index(r, i, 1)]);
  }
  return row_data;
}

void Matrix::set_cols(int c) {
  if (c < 0) {
    printf("ERROR negative number submitted to set_cols\n");
    exit(1);
  }

  if (c > cols) {

    double *temp = new double[rows * c * shel];
    for (int i = 1; i <= rows; i++) {
      for (int j = 1; j <= c; j++) {
        for (int k = 1; k <= shel; k++) {
          if (c <= cols) {
            temp[index(i, j, k)] = elem[index(i, j, k)];
          } else {
            temp[index(i, j, k)] = 0;
          }
        }
      }
    }
    delete elem;
    cols = c;
    elem = temp;
  } else if (c < cols) {

    double *temp = new double[rows * c * shel];
    printf("WARNING reducing matrix cols below previous value\n");
    printf("could lose data in the process\n");
    for (int i = 1; i <= rows; i++) {
      for (int j = 1; j <= c; j++) {
        for (int k = 1; k <= shel; k++) {
          temp[index(i, j, k)] = elem[index(i, j, k)];
        }
      }
    }
    delete elem;
    cols = c;
    elem = temp;
  }
}

void Matrix::set_shel(int s) {
  if (s < 0) {
    printf("ERROR negative number submitted to set_shel\n");
    exit(1);
  }
  if (s > shel) {

    double *temp = new double[rows * cols * s];
    for (int i = 1; i <= rows; i++) {
      for (int j = 1; j <= cols; j++) {
        for (int k = 1; k <= s; k++) {
          if (s <= shel) {
            temp[index(i, j, k)] = elem[index(i, j, k)];
          } else {
            temp[index(i, j, k)] = 0;
          }
        }
      }
    }
    delete elem;
    shel = s;
    elem = temp;
  } else if (s < shel) {

    double *temp = new double[rows * cols * s];
    printf("WARNING reducing matrix shel below previous value\n");
    printf("could lose data in the process\n");
    for (int i = 1; i <= rows; i++) {
      for (int j = 1; j <= cols; j++) {
        for (int k = 1; k <= s; k++) {
          temp[index(i, j, k)] = elem[index(i, j, k)];
        }
      }
    }
    delete elem;
    shel = s;
    elem = temp;
  }
}

void Matrix::set_elem(double val) { elem[0] = val; }

void Matrix::set_elem(double val, int r) {
  if (r > rows) {
    printf(
        "ERROR r value is greater than rows in set_elem(double val, int r)\n");
    exit(1);
  } else if (r < 1) {
    printf(
        "ERROR r value is less than or equal to 0 in set_elem(double val, int "
        "r)\n");
    exit(1);
  };

  elem[index(r, 1, 1)] = val;
}

void Matrix::set_elem(double val, int r, int c) {
  if (r > rows) {
    printf(
        "ERROR r value is greater than rows in set_elem(double val, int r, int "
        "c)\n");
    exit(1);
  } else if (r < 1) {
    printf(
        "ERROR r value is less than or equal to 0 in set_elem(double val, int "
        "r, int c)\n");
    exit(1);
  };
  if (c > cols) {
    printf(
        "ERROR c value is greater than rows in set_elem(double val, int r, int "
        "c)\n");
    exit(1);
  } else if (c < 1) {
    printf(
        "ERROR c value is less than or equal to 0 in set_elem(double val, int "
        "r, int c)\n");
    exit(1);
  };

  elem[index(r, c, 1)] = val;
}

void Matrix::set_elem(double val, int r, int c, int s) {
  if (r > rows) {
    printf(
        "ERROR r value is greater than rows in set_elem(double val, int r, int "
        "c, int s)\n");
    exit(1);
  } else if (r < 1) {
    printf(
        "ERROR r value is less than or equal to 0 in set_elem(double val, int "
        "r, int c, int s)\n");
    exit(1);
  };
  if (c > cols) {
    printf(
        "ERROR c value is greater than rows in set_elem(double val, int r, int "
        "c, int s)\n");
    exit(1);
  } else if (c < 1) {
    printf(
        "ERROR c value is less than or equal to 0 in set_elem(double val, int "
        "r, int c, int s)\n");
    exit(1);
  };
  if (s > shel) {
    printf(
        "ERROR s value is greater than rows in set_elem(double val, int r, int "
        "c, int s)\n");
    exit(1);
  } else if (s < 1) {
    printf(
        "ERROR s value is less than or equal to 0 in set_elem(double val, int "
        "r, int c, int s)\n");
    exit(1);
  };

  elem[index(r, c, s)] = val;
}

int Matrix::get_rows() const { return rows; }

int Matrix::get_cols() const { return cols; }

int Matrix::get_shel() const { return shel; }

int Matrix::total_elem() { return rows * cols * shel; }

double Matrix::get_elem() const { return elem[0]; }

double Matrix::get_elem(const int r) const {
  if (r < 1) {
    printf("ERROR get_elem(int r): 0 or negative row submitted\n");
    exit(1);
  } else if (r > rows) {
    printf(
        "ERROR get_elem(int r): row value larger than the scope of the "
        "Matrix\n");
    exit(1);
  }
  return elem[index(r, 1, 1)];
}

double Matrix::get_elem(const int r, const int c) const {
  if (r < 1) {
    printf("ERROR get_elem(int r, int c): 0 or negative row submitted\n");
    exit(1);
  } else if (r > rows) {
    printf(
        "ERROR get_elem(int r, int c): row value larger than the scope of the "
        "Matrix\n");
    exit(1);
  } else if (c < 1) {
    printf("ERROR get_elem(int r, int c): 0 or negative col submitted\n");
    exit(1);
  } else if (c > cols) {
    printf(
        "ERROR get_elem(int r, int c): col value larger than the scope of the "
        "Matrix\n");
    exit(1);
  }
  return elem[index(r, c, 1)];
}

double *Matrix::get_elem_ptr(const int r, const int c) const {
  return &elem[index(r, c, 1)];
}

double Matrix::get_elem(const int r, const int c, const int s) const {
  if (r < 1) {
    printf(
        "ERROR get_elem(int r, int c, int s): 0 or negative row submitted\n");
    exit(1);
  } else if (r > rows) {
    printf(
        "ERROR get_elem(int r, int c, int s): row value larger than the scope "
        "of the Matrix\n");
    exit(1);
  } else if (c < 1) {
    printf(
        "ERROR get_elem(int r, int c, int s): 0 or negative col submitted\n");
    exit(1);
  } else if (c > cols) {
    printf(
        "ERROR get_elem(int r, int c, int s): col value larger than the scope "
        "of the Matrix\n");
    exit(1);
  } else if (s < 1) {
    printf(
        "ERROR get_elem(int r, int c, int s): 0 or negative shelf submitted\n");
    exit(1);
  } else if (s > shel) {
    printf(
        "ERROR get_elem(int r, int c, int s): shelf value larger than the "
        "scope of the Matrix\n");
    exit(1);
  }
  return elem[index(r, c, s)];
}

void Matrix::swap_row(int r_from, int r_to) {
  if (r_from == r_to) return;

  vector<double> row_f = get_row(r_from);
  vector<double> row_t = get_row(r_to);
  set_row(row_f, r_to);
  set_row(row_t, r_from);
}

void Matrix::swap_col(int c_from, int c_to) {
  if (c_from == c_to) return;

  vector<double> col_f = get_col(c_from);
  vector<double> col_t = get_col(c_to);
  set_col(col_f, c_to);
  set_col(col_t, c_from);
}

void Matrix::move_row(int r_from, int r_to) {

  if (r_from == r_to) return;

  // Shift all rows down
  vector<double> row = get_row(r_from);
  // Shift all values down
  if (r_from < r_to) {
    for (int r = r_from; r < r_to; r++) {
      for (int c = 1; c <= get_cols(); c++) {
        set_elem(get_elem(r + 1, c), r, c);
      }
    }
  } else {
    for (int r = r_from; r >= (r_to + 1); r--) {
      for (int c = 1; c <= get_cols(); c++) {
        set_elem(get_elem(r - 1, c), r, c);
      }
    }
  }
  // Insert the row we took out
  this->set_row(row, r_to);
}

void Matrix::move_col(int c_from, int c_to) {

  if (c_from == c_to) return;

  // Shift all rows down
  vector<double> col = get_col(c_from);
  // Shift all values down
  if (c_from < c_to) {
    for (int c = c_from; c < c_to; c++) {
      for (int r = 1; r <= get_rows(); r++) {
        set_elem(get_elem(r, c + 1), r, c);
      }
    }
  } else {
    for (int c = c_from; c >= (c_to + 1); c--) {
      for (int r = 1; r <= get_rows(); r++) {
        set_elem(get_elem(r, c - 1), r, c);
      }
    }
  }
  // Insert the row we took out
  set_col(col, c_to);
}

vector<int> Matrix::matchRow(const Matrix & mat, int sf) {
  if (mat.get_shel() != 1 || this->get_shel() != 1) {
    cerr << "ERROR shel should be 1" << endl;
    exit(1);
  }
  if (mat.get_cols() != this->get_cols()) {
    throw invalid_argument(
        "match function only works when matrices have same number of columns");
  }
  vector<int> m_vec(this->get_rows(), -1);

  for (int i = 1; i <= get_rows(); ++i) {
    for (int ii = 1; ii <= mat.get_rows(); ++ii) {
      bool match = true;
      for (int j = 1; j <= get_cols(); ++j) {
        string val1 = double_tos(get_elem(i, j), sf);
        string val2 = double_tos(mat.get_elem(ii, j), sf);
        if (val1.compare(val2) != 0) {
          match = false;
          break;
        }
      }
      if (match) m_vec.at(i - 1) = ii;
    }
  }
  return m_vec;
}

vector<int> Matrix::matchCol(const Matrix &mat, const int sf) const {
  if (mat.get_shel() != 1 || this->get_shel() != 1) {
    cerr << "ERROR shel should be 1" << endl;
    exit(1);
  }
  if (mat.get_rows() != this->get_rows()) {
    throw invalid_argument(
        "match function only works when matrices have same number of rows");
  }
  vector<int> m_vec(this->get_cols(), -1);

  for (int i = 1; i <= get_cols(); ++i) {
    for (int ii = 1; ii <= mat.get_cols(); ++ii) {
      bool match = true;
      for (int j = 1; j <= get_rows(); ++j) {
        string val1 = double_tos(get_elem(j, i), sf);
        string val2 = double_tos(mat.get_elem(j, ii), sf);
        if (val1.compare(val2) != 0) {
          match = false;
          break;
        }
      }
      if (match) m_vec.at(i - 1) = ii;
    }
  }
  return m_vec;
}

Matrix Matrix::invert() const{

  if (this->get_shel() != 1) {
    printf("ERROR Matrix_Invert only allowed for 1d and 2d arrays not 3d\n");
    exit(1);
  }

  Matrix *mat2 = new Matrix(this->get_cols(), this->get_rows());

  for (int i = 1; i <= this->get_rows(); i++) {
    for (int j = 1; j <= this->get_cols(); j++) {
      double val = this->get_elem(i, j);
      (*mat2).set_elem(val, j, i);
    }
  }
  return *mat2;
}

Matrix Matrix::getRow(int R) const{

  if (this->get_rows() < R) {
    printf("ERROR Matrix_getRow cannot return R %d", R);
    printf(" because mat has only %d rows\n.", this->get_rows());
    exit(1);
  }

  Matrix mat2(1, this->get_cols(), this->get_shel());

  for (int j = 1; j <= this->get_cols(); j++) {
    for (int k = 1; k <= this->get_shel(); k++) {
      double val = this->get_elem(R, j, k);
      mat2.set_elem(val, 1, j, k);
    }
  }
  return mat2;
}

Matrix Matrix::getCol(int C) const {

  if (this->get_cols() < C) {
    printf("ERROR Matrix_getCol cannot return C %d", C);
    printf(" because mat has only %d cols\n.", this->get_cols());
    exit(1);
  }
  Matrix mat2(this->get_cols(), 1, this->get_shel());

  for (int i = 1; i <= this->get_rows(); i++) {
    for (int k = 1; k <= this->get_shel(); k++) {
      double val = this->get_elem(i, C, k);
      mat2.set_elem(val, i, 1, k);
    }
  }
  return mat2;
}
