
#pragma once
#ifndef _CATNIP_INDEX_MAP_HPP
#define _CATNIP_INDEX_MAP_HPP

// Local private includes
#include "atom_system.hpp"

// Standard includes
#include <map>
#include <vector>

namespace catnip {

  class IndexMap {
    public:
      IndexMap() = default;
      IndexMap(AtomSystem atm_sys);
      // The component rows and colums in their current locations in  
      // in the full system matrix of the complex
      std::map<int,std::vector<int>> row_col_init;

      // Map indicate where the rows and cols should go in the final state
      // Each int in the vector represents the location in the full system matrix
      std::map<int,std::vector<int>> row_col_final;

      // Searches the init row for the provided row and returns the group it is
      // in as well as its current index
      std::pair<int,int> findLocation(int row);
  };


}  // namespace catnip
#endif // _CATNIP_INDEX_MAP_HPP
