

#include "index_map.hpp"

#include "atom.hpp"
#include "atom_group.hpp"

namespace catnip {

  IndexMap::IndexMap(AtomGroupContainer atom_groups){
    std::vector<int> group_indices = atom_groups.getGroups(GroupType::Component);
    int complex_ind = atom_groups.getGroups(GroupType::Complex).at(0);
    AtomGroup complex_group = atom_groups.getGroup(complex_ind);
    int sys_ind = 0;
    for ( int & group_ind : group_indices) {
      AtomGroup component_group = atom_groups.getGroup(group_ind);
      for( std::shared_ptr<Atom> & atom : component_group){
        int atom_ind = complex_group.findIndex(atom);
        int num_basis = complex_group.at(atom_ind)->getBasisFuncCount();
        for ( int ind_temp = 0; ind_temp<num_basis; ++ind_temp){
          row_col_final[group_ind].push_back(sys_ind); 
          row_col_init[group_ind].push_back(atom_ind);
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
