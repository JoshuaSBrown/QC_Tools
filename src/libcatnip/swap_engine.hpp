#pragma once
#ifndef _CATNIP_SWAP_ENGINE_HPP
#define _CATNIP_SWAP_ENGINE_HPP

#include "atom.hpp"
#include "atom_group.hpp"
#include "basis_map.hpp"
#include "size_type.hpp"

namespace catnip {

  // Swaps rows columns and keeps up with how future swaps should be updated
  class SwapEngine {
    private:
      size_t size_;
      std::vector<std::pair<int,int>> swap_order_;

      void arrangeRows_(Eigen::MatrixXd & mat);
      void arrangeCols_(Eigen::MatrixXd & mat);
    public:
      SwapEngine(BasisMap basis_map);

      void arrange(Eigen::MatrixXd & mat);
      void arrangeRows(Eigen::MatrixXd & mat);
      void arrangeCols(Eigen::MatrixXd & mat);
  };


}  // namespace catnip

#endif // _CATNIP_SWAP_ENGINE_HPP
