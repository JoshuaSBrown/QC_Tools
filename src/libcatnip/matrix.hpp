
#ifndef MATRIX_HELPER_HPP
#define MATRIX_HELPER_HPP

namespace catnip {
#include <vector>
#include <Eigen/Dense>

  /** 
   * Responsible for matching the rows between two matrices Returns which row
   * in matrix 1 matches which row in matrix 2, -1 means there is no match sf
   * is the number of significant figures that will be checked to ensure the
   * same value.
   * 
   * E.g. 
   *
   * Matrix 1
   *
   * 1.0 0.0 0.0
   * 0.0 3.0 0.0
   * 0.0 2.5 1.0
   * 0.0 0.0 5.0
   * 6.0 0.0 0.0
   *
   * Matrix 2 
   *
   * 1.0 0.0 0.0
   * 8.0 8.0 8.0
   * 0.0 0.0 5.0
   *
   * The vector returned would return 
   *
   * vector matched rows
   *  0
   * -1 // indicates no match
   *  3 
   **/ 
  std::vector<int> matchRow(
      const Eigen::MatrixXd &mat1, 
      const Eigen::MatrixXd &mat2, 
      const int sig_fig);

  std::vector<int> matchCol(
      const Eigen::MatrixXd &mat1, 
      const Eigen::MatrixXd &mat2, 
      const int sig_fig);

} // namespace catnip
#endif // MATRIX_HELPER_HPP
