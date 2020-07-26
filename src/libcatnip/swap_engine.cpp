

#include "swap_engine.hpp"

#include "atom.hpp"
#include "atom_group.hpp"
#include "size_type.hpp"

namespace catnip {

  // Swaps rows columns and keeps up with how future swaps should be updated
  SwapEngine::SwapEngine(IndexMap ind_map, Size size) : 
    size_(size), ind_map_(ind_map) {
      scratch_ = Eigen::MatrixXd::Zero(size, 1);


      // Already have enough information to calculate the correct swap order
      // Note the order of swaps is important.
      for ( std::pair<int, std::vector<int>> group_row : ind_map_.row_col_init ){
        int group_ind = group_row.first;
        for ( int row : group_row.second ) {
          int final_row = ind_map_.row_col_final[group_ind].at(row);
          std::pair<int,int> swap(row,final_row);
          // Now we need to update index map, the row located at the final 
          // position has now been moved     
          auto group_and_row_ind = ind_map_.findLocation(final_row);

          // Final row location needs to be pointed to the starting row
          ind_map_.row_col_init[group_and_row_ind.first].at(group_and_row_ind.second) = row;
        } 
      }
    }
  // This method of swapping was chosen to keep the memory footprint
  // reasonable.
  // Will correctly sort the rows and columns of the matrix
  void SwapEngine::arrange(Eigen::MatrixXd & mat) {

    // Check that matrix is square
    if( mat.rows() != mat.cols() ) {
      throw std::runtime_error("Can only arrange square matrices");
    }
    if( mat.rows() != size_ ) {
      throw std::runtime_error("Index map has different number of basis functions to matrix");
    }

    for ( std::pair<int, std::vector<int>> group_row : ind_map_.row_col_init ){
      int group_ind = group_row.first;
      for ( int row : group_row.second ) {
        int final_row = ind_map_.row_col_final[group_ind].at(row);
        if(final_row == row ) continue;
        // Copy the data at the final row into the scratch space 
        scratch_.col(0) = mat.row(final_row);
        // Copy the data from the initial row into the final row
        mat.row(final_row) = mat.row(row);
        // Copy the data from the temporary row into the initial row
        mat.row(row) = scratch_.col(0);
      }
    }

    // Repeat for columns 
    for ( std::pair<int, std::vector<int>> group_col : ind_map_.row_col_init ){
      int group_ind = group_col.first;
      for ( int col : group_col.second ) {
        int final_col = ind_map_.row_col_final[group_ind].at(col);
        if(final_col == col ) continue;
        // Copy the data at the final row into the scratch space
        scratch_.col(0) = mat.col(final_col);
        // Copy the data from the initial row into the final row
        mat.col(final_col) = mat.col(col);
        // Copy the data from the temporary row into the initial row
        mat.col(col) = scratch_.col(0);
      }
    }
  } 
}  // namespace catnip
