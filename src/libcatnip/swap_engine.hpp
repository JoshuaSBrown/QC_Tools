
#pragma once
#ifndef _CATNIP_SWAP_ENGINE_HPP
#define _CATNIP_SWAP_ENGINE_HPP

#include "atom.hpp"
#include "atom_group.hpp"
#include "index_map.hpp"
#include "size_type.hpp"

namespace catnip {

  // Swaps rows columns and keeps up with how future swaps should be updated
  class SwapEngine {
    private:
      Size size_;
      IndexMap ind_map_;
      Eigen::MatrixXd scratch_;

    public:
      SwapEngine(IndexMap ind_map, Size size);

      void arrange(Eigen::MatrixXd & mat);
  };


}  // namespace catnip

#endif // _CATNIP_SWAP_ENGINE_HPP
