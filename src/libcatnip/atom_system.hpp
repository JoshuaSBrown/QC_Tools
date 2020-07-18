
#pragma once
#ifndef CATNIP_ATOM_SYSTEM_HPP
#define CATNIP_ATOM_SYSTEM_HPP
#include "atom_group_container.hpp"

namespace catnip {

  /**
   * @brief Unlike atom group container, this atom system does not allow to 
   * change group types
   *
   * It can only be created if there are known group types and there is only
   * a single complex, and it's components
   */
  class AtomSystem {

      AtomGroupContainer atm_grp_cont_;    
    public:

      AtomSystem(AtomGroupContainer atm_cont);

      /**
       * @brief assign basis functions to atom group 
       *
       * @param complex_basis_func_count
       */
      void assignBasisFunctions(int index, const std::vector<int>& basis_funcs);

      /**
       * @brief Determine if there is a conflict between the basis functions
       * you are attempting to assign and the currently assigned basis functions
       *
       * This is done by comparing the basis functions on atoms that shared
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
  }

};

#endif // CATNIP_ATOM_SYSTEM_HPP
