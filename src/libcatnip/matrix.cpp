
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <eigen3/Eigen/Core>

#include "matrix.hpp"

using namespace std;
namespace catnip { 
string double_tos(double f, int nd) {
  ostringstream ostr;
  int tens = stod("1" + string(nd, '0'));
  ostr << round(f * tens) / tens;
  return ostr.str();
}

vector<int> matchRow(const Eigen::MatrixXd & mat1, const Eigen::MatrixXd & mat2, const int sf) {

  if (mat1.cols() != mat2.cols()) {
    throw invalid_argument(
        "match function only works when matrices have same number of columns");
  }
  vector<int> m_vec(mat1.rows(), -1);

  for (int i = 0; i < mat1.rows(); ++i) {
    for (int ii = 0; ii < mat2.rows(); ++ii) {
      bool match = true;
      for (int j = 0; j < mat1.cols(); ++j) {
        string val1 = double_tos(mat1(i, j), sf);
        string val2 = double_tos(mat2(ii, j), sf);
        if (val1.compare(val2) != 0) {
          match = false;
          break;
        }
      }
      if (match) m_vec.at(i) = ii;
    }
  }
  return m_vec;
}

vector<int> matchCol(const Eigen::MatrixXd & mat1,
    const Eigen::MatrixXd &mat2, 
    const int sf) {

  if (mat1.rows() != mat2.rows()) {
    throw invalid_argument(
        "match function only works when matrices have same number of rows");
  }
  vector<int> m_vec(mat1.cols(), -1);

  for (int i = 0; i < mat1.cols(); ++i) {
    for (int ii = 0; ii < mat2.cols(); ++ii) {
      bool match = true;
      for (int j = 0; j < mat1.rows(); ++j) {
        string val1 = double_tos(mat1(j, i), sf);
        string val2 = double_tos(mat2(j, ii), sf);
        if (val1.compare(val2) != 0) {
          match = false;
          break;
        }
      }
      if (match) m_vec.at(i) = ii;
    }
  }
  return m_vec;
}

Eigen::MatrixXd convert(vector<vector<double>> coords){

  Eigen::MatrixXd coord_mat(coords.size(),coords.at(0).size());
  for ( size_t row_ind = 0; row_ind < coords.size();++row_ind){
    coord_mat.row(row_ind) = Eigen::Map<Eigen::VectorXd>(coords.at(row_ind).data(),coords.at(row_ind).size()); 
  }
  return coord_mat;
}

}
