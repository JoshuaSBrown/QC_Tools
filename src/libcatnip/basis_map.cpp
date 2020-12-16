

#include "basis_map.hpp"

#include "atom.hpp"
#include "atom_system.hpp"

// Standard includes
#include <cassert>
#include <iostream>

namespace catnip {

  BasisMap::BasisMap(const AtomSystem & atm_sys){
    if ( not atm_sys.systemComplete() ) {
      throw std::runtime_error("Cannot build an index map from an incomplete "
          "atom system.");
    }

    // 1. map each of the atoms in complex to an offset based on the number 
    // of basis functions up to that point, 
    // E.g.
    // atom1 2 basis functs
    // atom2 3 basis functs
    // atom3 1 basis functs
    //
    // offset_map[0] = 0; // atom1
    // offset_map[1] = 2; // atom2
    // offset_map[2] = 5; // atom3

    std::unordered_map<int,int> offset_map;
    const std::unique_ptr<AtomGroup> & complex_grp = atm_sys.getComplex();
    int offset = 0;
    int complex_atm_ind = 0;
    for ( const std::shared_ptr<Atom> & atm_ptr : *complex_grp){
      offset_map[complex_atm_ind] = offset;
      offset += atm_ptr->getBasisFuncCount();
      ++complex_atm_ind;
    } 

    std::vector<int> component_indices = atm_sys.getComponentIndices();
    int sys_ind = 0;
    for ( int & component_ind : component_indices) {
      const std::unique_ptr<AtomGroup> & component_grp = atm_sys.at(component_ind);
      std::cout << "Component grp " << component_grp->getName() << " component ind " << component_ind << " size " << component_grp->size() << std::endl;
      for( const std::shared_ptr<Atom> & atm_ptr : *component_grp){
        std::vector<int> atm_indices = complex_grp->find(atm_ptr->getPos());
        assert(atm_indices.size() == 1);
        int atm_ind = atm_indices.at(0);
        int num_basis = complex_grp->at(atm_ind)->getBasisFuncCount();
        int complex_offset = offset_map[atm_ind];

        for ( int ind_temp = 0; ind_temp<num_basis; ++ind_temp){
          // Where the row would ideally be located
          row_col_final[component_ind].push_back(sys_ind); 
          // Where the row of the component is currently located
          row_col_current[component_ind].push_back(complex_offset+ind_temp);
          sys_ind++;
        }
      }
    }
  }

  // Searches the current row for the provided row and returns the group it is
  // in as well as its current index
  std::pair<int,int> BasisMap::findLocation(int row){
    for( std::pair<int,std::vector<int>> group_rows : row_col_current){
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
