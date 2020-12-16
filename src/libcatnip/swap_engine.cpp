
// Local private includes
#include "swap_engine.hpp"

#include "atom.hpp"
#include "atom_group.hpp"
#include "basis_map.hpp"
#include "size_type.hpp"

// Standard includes
#include <algorithm>

namespace catnip {


  /**
   * @brief Serves as scracth space 
   *
   * Can store two rows at a time, the order they are loaded and unloaded is
   * FIFO - first in first out
   */
  class Scratch {
    private:
      Eigen::MatrixXd scratch_;
      int size_;
      bool empty_ = true;
    public:
      Scratch(int size) : size_(size) {
        scratch_ = Eigen::MatrixXd::Zero(1,size_);
      }

      bool empty() const noexcept { return empty_; }

      void loadFromRow(const Eigen::MatrixXd & mat, int row){
        for ( int i = 0; i < size_ ; ++i ){
          scratch_(0, i) = mat(row,i);
        }
        empty_ = false;
      }

      void loadFromCol(const Eigen::MatrixXd & mat, int col){
        for ( int i = 0; i < size_ ; ++i ){
          scratch_(0, i) = mat(i,col);
        }
        empty_ = false;
      }
      void unloadToCol(Eigen::MatrixXd & mat, int col) {
        for ( int i = 0; i < size_ ; ++i ){
          mat(i,col) = scratch_(0, i);
        } 
        empty_ = true;
      }
      void unloadToRow(Eigen::MatrixXd & mat, int row) {
        for ( int i = 0; i < size_ ; ++i ){
          mat(row, i) = scratch_(0, i);
        } 
        empty_ = true;
      }

  };

  // Swaps rows columns and keeps up with how future swaps should be updated
  SwapEngine::SwapEngine(BasisMap basis_map) {

    size_ = 0;
    // The  with the largest index will tell us the size we need to allocate
    for ( std::pair<int,std::vector<int>> grp_rows : basis_map.row_col_final ){
      size_t max_value = *(std::max_element(grp_rows.second.begin(), grp_rows.second.end()));
      size_ = std::max( max_value+1 , size_ ); 
    }

    // Want to order these 
    // 4 
    // 3
    // 1
    // 2

    // How can we order
    // 4 <-> 2   2 <-> 3   3 <-> 1
    //
    //  > 2       > 3       > 1
    //    3         2         2
    //    1         1         3
    //    4         4         4
    //

    // Already have enough information to calculate the correct swap order
    // Note the order of swaps is important.
    for ( std::pair<const int, std::vector<int>> & group_row : basis_map.row_col_current ){
      int group_ind = group_row.first;
      size_t index = 0;

      assert(group_row.second.size()>0 && "ERROR a component has no rows.");
      do {
        int current_row_in_complex = group_row.second.at(index);
        int final_row_in_complex = basis_map.row_col_final[group_ind].at(index);


        if ( current_row_in_complex != final_row_in_complex ){
          // Row that currently occupies the location where the current row should
          // go
          auto group_and_row_ind = basis_map.findLocation(final_row_in_complex);
          int replacement_row = basis_map.row_col_current[group_and_row_ind.first].at(group_and_row_ind.second);
          std::pair<int,int> swap(replacement_row, current_row_in_complex);

          // Final row location needs to be pointed to the starting row
          basis_map.row_col_current[group_ind].at(index) = replacement_row;
          basis_map.row_col_current[group_and_row_ind.first].at(group_and_row_ind.second) = current_row_in_complex;

          swap_order_.push_back(swap);
        } else {
          ++index;
        }
      } while (index < group_row.second.size());
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
    if( mat.rows() != static_cast<Eigen::Index>(size_) ) {
      throw std::runtime_error("Scratch space, swap procedure is not "
          "consistent with the matrix.");
    }
     
    arrangeRows(mat);
    // Repeat for columns 
    arrangeCols(mat);
  } 

  void SwapEngine::arrangeCols(Eigen::MatrixXd & mat) {
    // Check that matrix is square
    if( mat.rows() != mat.cols() ) {
      throw std::runtime_error("Can only arrange square matrices");
    }
    if( mat.rows() != static_cast<Eigen::Index>(size_) ) {
      throw std::runtime_error("Scratch space, swap procedure is not "
          "consistent with the matrix.");
    }
    arrangeCols_(mat);
  }

  void SwapEngine::arrangeCols_(Eigen::MatrixXd & mat) {

    Scratch scratch(size_);
    for ( size_t swap_ind = 0; swap_ind < swap_order_.size(); ++swap_ind ){
   
      auto swap = swap_order_.at(swap_ind); 
      if (scratch.empty() ) {
        scratch.loadFromCol(mat, swap.first);
      }
      // Write too the first instance
      mat.col(swap.first) = mat.col(swap.second);
      // If the next swap involves the row we just loaded don't bother writing
      // it back into mat just keep it in scratch
      if ( swap_ind + 1 < swap_order_.size() ){
        if( swap_order_.at(swap_ind+1).first == swap.second ) continue;
      }  
      scratch.unloadToCol(mat, swap.second);
    }
  }

  void SwapEngine::arrangeRows(Eigen::MatrixXd & mat) {
    // Check that matrix is square
    if( mat.rows() != mat.cols() ) {
      throw std::runtime_error("Can only arrange square matrices");
    }
    if( mat.rows() != static_cast<Eigen::Index>(size_) ) {
      std::string error_msg = "Scratch space, swap procedure is not "
          "consistent with the matrix. Rows in matrix "
          + std::to_string(mat.rows()) + " size of scratch space "
          + std::to_string(size_);
      throw std::runtime_error(error_msg);
    }
    arrangeRows_(mat);
  }

  void SwapEngine::arrangeRows_(Eigen::MatrixXd & mat) {
  
    Scratch scratch(size_);
    for ( size_t swap_ind = 0; swap_ind < swap_order_.size(); ++swap_ind ){
   
      auto swap = swap_order_.at(swap_ind); 
      if (scratch.empty() ) {
        scratch.loadFromRow(mat, swap.first);
      }
      // Write too the first instance
      mat.row(swap.first) = mat.row(swap.second);
      // If the next swap involves the row we just loaded don't bother writing
      // it back into mat just keep it in scratch
      if ( swap_ind + 1 < swap_order_.size() ){
        if( swap_order_.at(swap_ind+1).first == swap.second ) continue;
      }  
      scratch.unloadToRow(mat, swap.second);
    }

  }
}  // namespace catnip
