#include "matrix.hpp"
#include <iostream>

int main() {
  {
    std::cout << "Testing Constructors\n" << std::endl;
    std::cout << "Testing: Matrix::Matrix()\n" << std::endl;
    Matrix mat0;
    std::cout <<  mat0 << std::endl;
    std::cout << "Testing: Matrix::Matrix(int r)\n" << std::endl; 
    Matrix mat1(3);
    std::cout <<  mat1 << std::endl;
    std::cout << "Testing: Matrix::Matrix(int r, int c)\n" << std::endl;
    Matrix mat2(2,3);
    std::cout <<  mat2 << std::endl;
    std::cout << "Testing: Matrix::Matrix(int r, int c, int s)\n" << std::endl;
    Matrix mat3(3,1, 2);
    std::cout <<  mat3 << std::endl;
    std::cout << "\nTesting: Matrix::set_rows(int r)\n" << std::endl;
    mat0.set_rows(3);
    std::cout <<  mat0 << std::endl;
    std::cout << "\nTesting: Matrix::set_cols(int c)\n" << std::endl;
    mat0.set_cols(4);
    std::cout <<  mat0 << std::endl;
    std::cout << "\nTesting: Matrix::set_shel(int s)\n" << std::endl;
    mat0.set_shel(2);
    std::cout <<  mat0 << std::endl;
    std::cout << "\nTesting: Matrix::set_elem(double val)\n" << std::endl;
    mat0.set_elem(5.4);
    std::cout <<  mat0 << std::endl;
    std::cout << "\nTesting: Matrix::set_elem(double val, int r)\n" << std::endl;
    mat0.set_elem(4.4, 3);
    std::cout <<  mat0 << std::endl;
    std::cout << "\nTesting: Matrix::set_elem(double val, int r, int c)\n" << std::endl;
    mat0.set_elem(4.2, 3, 4);
    std::cout <<  mat0 << std::endl;
    std::cout <<  "\nTesting: Matrix::set_elem(double val, int r, int c)\n" << std::endl;
    mat0.set_elem(0.3, 3, 4, 2);
    std::cout <<  mat0 << std::endl;

    Matrix mat4(2,3);
    Matrix mat5(3,2);

    mat4.set_elem(5,1,1);
    mat4.set_elem(3,1,2);
    mat4.set_elem(1,1,3);
    mat4.set_elem(2,2,2);
    mat4.set_elem(1,2,3);

    mat5.set_elem(2,1,1);
    mat5.set_elem(1,1,2);
    mat5.set_elem(1,2,1);
    mat5.set_elem(3,3,1);
    mat5.set_elem(4,3,2);

    std::cout <<  mat4 << std::endl;
    std::cout <<  mat5 << std::endl;

    Matrix mat6 = mat4 * mat5;
    std::cout <<  mat6 << std::endl;

    std::cout <<  "\nResizing matrix 6" << std::endl; 
    mat6.resize(3,3);
    std::cout << mat6 << std::endl;
  }
  return 0;
}
