
#pragma once
#ifndef CATNIP_ATOM_SYSTEM_HPP
#define CATNIP_ATOM_SYSTEM_HPP

// Local private includes
#include "atom_group_container.hpp"

// Standard includes
#include <map>
#include <vector>

namespace catnip {

  /**
   * @brief Unlike atom group container, this atom system does not allow to 
   * change group types
   *
   * It can only be created if there are known group types and there is only
   * a single complex, and it's components, 
   *
   * Each atom in a component must only be shared with the complex, the atom of
   * a component is allowed to be of a different element then what is in the 
   * complex, but must have the same position
   */
  class AtomSystem {
      // Atoms in each component that share the same location with atoms in
      // the complex but do not share the same element
      std::map<int,std::vector<std::pair<int,int>>> linked_atoms_;
      AtomGroupContainer atm_grp_cont_;    
    public:

      AtomSystem(AtomGroupContainer atm_cont);

      /**
       * @brief assign basis functions to atom group
       *
       * Note that atoms that are linked between component and complex must 
       * have the exact same number of basis functions.
       *
       * @param complex_basis_func_count
       */
      void assignBasisFunctions(int index, const std::vector<int>& basis_funcs);

      /**
       * @brief Determine if there is a conflict between the basis functions
       * you are attempting to assign and the currently assigned basis functions
       *
       * This is done by comparing the basis functions on atoms that are shared
       * between atom groups.
       *
       * @param index
       * @param basis_funcs
       *
       * @return 
       */
      bool conflictBasisFunctions(int index, const std::vector<int> & basis_funcs) const; 

      int getTotalBasisFunctions(const GroupType & type) const;

      int getMaxBasisFunctions(const GroupType & type) const;

      const AtomGroup & getComplex() const;       
      std::vector<int> getComponentIndices() const noexcept { atm_grp_cont_.getGroups(GroupType::Component);}
      const AtomGroup & at(int ind) const;       
      /**
       * @brief Check that every atom knows how many basis functions are
       * assigned to it
       *
       * @return 
       */
      bool systemComplete() const noexcept;
  };

};

#endif // CATNIP_ATOM_SYSTEM_HPP
