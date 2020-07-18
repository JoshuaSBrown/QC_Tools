

#include "index_map.hpp"

#include "atom.hpp"
#include "atom_group.hpp"

// Standard includes
#include <cassert>

namespace catnip {

  IndexMap::IndexMap(AtomSystem atm_sys){
    std::vector<int> comp_indices = atm_sys.getComponentIndices();
    const AtomGroup & complex_grp = atm_sys.getComplex();
    int sys_ind = 0;
    for ( int & comp_ind : comp_indices) {
      const AtomGroup & comp_grp = atm_sys.at(comp_ind);
      for( const std::shared_ptr<Atom> & atm_ptr : comp_grp){
        std::vector<int> atm_indices = complex_grp.find(atm_ptr);
        assert(atm_indices.size() == 1);
        int atm_ind = atm_indices.at(0);
        int num_basis = comp_grp.at(atm_ind)->getBasisFuncCount();
        for ( int ind_temp = 0; ind_temp<num_basis; ++ind_temp){
          row_col_final[comp_ind].push_back(sys_ind); 
          row_col_init[comp_ind].push_back(atm_ind);
          sys_ind++;
        }
      }
    }
  }
  // Searches the init row for the provided row and returns the group it is
  // in as well as its current index
  std::pair<int,int> IndexMap::findLocation(int row){
    for( std::pair<int,std::vector<int>> group_rows : row_col_init){
      int row_ind = 0;
      for ( int row_ : group_rows.second ){
        if (row_ == row){
          return std::pair<int,int>(group_rows.first, row_ind);
        }
        ++row_ind;
      } 
    }
    return std::pair<int,int>();
  }

}  // namespace catnip
