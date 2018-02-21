#include "matrix.hpp"
#include <iostream>
#include <cassert>
using namespace std;
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

  cerr << "Testing: matchRow" << endl;
  {
    Matrix mat4(2,3);
    Matrix mat5(3,3);

    // mat4
    // 5 3 1
    // 0 2 1
    
    mat4.set_elem(5,1,1);
    mat4.set_elem(3,1,2);
    mat4.set_elem(1,1,3);
    mat4.set_elem(2,2,2);
    mat4.set_elem(1,2,3);

    // mat5
    // 2 1 0
    // 1 0 0
    // 5 3 1
    mat5.set_elem(2,1,1);
    mat5.set_elem(1,1,2);
    mat5.set_elem(1,2,1);
    mat5.set_elem(5,3,1);
    mat5.set_elem(3,3,2);
    mat5.set_elem(1,3,3);
    
    auto m_vec = mat4.matchRow(mat5,4);
    assert(m_vec.at(0)==3);
    assert(m_vec.at(1)==-1);
    assert(m_vec.at(2)==-1);
  }

  cerr << "Testing: set_row & set_col" << endl;
  {
    // mat5
    // 2 1 0
    // 1 0 0
    // 5 3 1
    Matrix mat5(3,3);
    mat5.set_elem(2,1,1);
    mat5.set_elem(1,1,2);
    mat5.set_elem(0,1,3);
    mat5.set_elem(1,2,1);
    mat5.set_elem(0,2,2);
    mat5.set_elem(0,2,3);
    mat5.set_elem(5,3,1);
    mat5.set_elem(3,3,2);
    mat5.set_elem(1,3,3);
   
    vector<double> values{ 9, 12, 13 };
    mat5.set_row(values,1);
    assert(mat5.get_elem(1,1)==9);
    assert(mat5.get_elem(1,2)==12);
    assert(mat5.get_elem(1,3)==13);
    assert(mat5.get_elem(2,1)==1);
    assert(mat5.get_elem(2,2)==0);
    assert(mat5.get_elem(2,3)==0);
    assert(mat5.get_elem(3,1)==5);
    assert(mat5.get_elem(3,2)==3);
    assert(mat5.get_elem(3,3)==1);
  
    vector<double> values2{-12,23,101};
    mat5.set_col(values2,2);
    assert(mat5.get_elem(1,1)==9);
    assert(mat5.get_elem(1,2)==-12);
    assert(mat5.get_elem(1,3)==13);
    assert(mat5.get_elem(2,1)==1);
    assert(mat5.get_elem(2,2)==23);
    assert(mat5.get_elem(2,3)==0);
    assert(mat5.get_elem(3,1)==5);
    assert(mat5.get_elem(3,2)==101);
    assert(mat5.get_elem(3,3)==1);
  }

  cerr << "Testing: move_row & move_col" << endl;
  {
    // mat5
    // 2 1 0
    // 1 0 0
    // 5 3 1
    Matrix mat5(3,3);
    mat5.set_elem(2,1,1);
    mat5.set_elem(1,1,2);
    mat5.set_elem(0,1,3);
    mat5.set_elem(1,2,1);
    mat5.set_elem(0,2,2);
    mat5.set_elem(0,2,3);
    mat5.set_elem(5,3,1);
    mat5.set_elem(3,3,2);
    mat5.set_elem(1,3,3);
 
    // 5 3 1
    // 2 1 0
    // 1 0 0
    mat5.move_row(3,1);
    assert(mat5.get_elem(1,1)==5);
    assert(mat5.get_elem(1,2)==3);
    assert(mat5.get_elem(1,3)==1);
    assert(mat5.get_elem(2,1)==2);
    assert(mat5.get_elem(2,2)==1);
    assert(mat5.get_elem(2,3)==0);
    assert(mat5.get_elem(3,1)==1);
    assert(mat5.get_elem(3,2)==0);
    assert(mat5.get_elem(3,3)==0);

    // 1 5 3 
    // 0 2 1 
    // 0 1 0 
    mat5.move_col(3,1);
    cerr << mat5 << endl;
    assert(mat5.get_elem(1,1)==1);
    assert(mat5.get_elem(1,2)==5);
    assert(mat5.get_elem(1,3)==3);
    assert(mat5.get_elem(2,1)==0);
    assert(mat5.get_elem(2,2)==2);
    assert(mat5.get_elem(2,3)==1);
    assert(mat5.get_elem(3,1)==0);
    assert(mat5.get_elem(3,2)==1);
    assert(mat5.get_elem(3,3)==0);

    // 0 2 1 
    // 0 1 0 
    // 1 5 3 
    mat5.move_row(1,3);
    assert(mat5.get_elem(1,1)==0);
    assert(mat5.get_elem(1,2)==2);
    assert(mat5.get_elem(1,3)==1);
    assert(mat5.get_elem(2,1)==0);
    assert(mat5.get_elem(2,2)==1);
    assert(mat5.get_elem(2,3)==0);
    assert(mat5.get_elem(3,1)==1);
    assert(mat5.get_elem(3,2)==5);
    assert(mat5.get_elem(3,3)==3);

    // 2 1 0 
    // 1 0 0
    // 5 3 1
    mat5.move_col(1,3);
    assert(mat5.get_elem(1,1)==2);
    assert(mat5.get_elem(1,2)==1);
    assert(mat5.get_elem(1,3)==0);
    assert(mat5.get_elem(2,1)==1);
    assert(mat5.get_elem(2,2)==0);
    assert(mat5.get_elem(2,3)==0);
    assert(mat5.get_elem(3,1)==5);
    assert(mat5.get_elem(3,2)==3);
    assert(mat5.get_elem(3,3)==1);
  
  }
  return 0;
}
