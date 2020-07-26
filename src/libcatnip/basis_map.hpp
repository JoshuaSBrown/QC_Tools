
#pragma once
#ifndef _CATNIP_INDEX_MAP_HPP
#define _CATNIP_INDEX_MAP_HPP

// Local private includes
#include "atom_system.hpp"

// Standard includes
#include <map>
#include <vector>

namespace catnip {


  /**
   * @brief This class tracks how the basis functions map to the rows in the 
   * complex
   */
  class BasisMap {
    public:
      BasisMap() = default;
      BasisMap(AtomSystem atm_sys);
      // The component rows and colums in their current locations in  
      // in the full system matrix of the complex
      std::map<int,std::vector<int>> row_col_current;

      // Map indicate where the rows and cols should go in the final state
      // Each int in the vector represents the location in the full system matrix
      std::map<int,std::vector<int>> row_col_final;

      /**
       * @brief Given a row of the complex finds it's location in one of the 
       * components
       *
       * @param row_complex
       *
       * @return 
       */
      std::pair<int,int> findLocation(int row_complex);
  };


}  // namespace catnip
#endif // _CATNIP_INDEX_MAP_HPP
