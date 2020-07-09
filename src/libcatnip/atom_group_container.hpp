#pragma once
#ifndef _CATNIP_ATOM_GROUP_CONTAINER_HPP
#define _CATNIP_ATOM_GROUP_CONTAINER_HPP

#include "atom_group.hpp"

#include <eigen3/Eigen/Dense>

#include <exception>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace catnip {

  class Atom;

  class AtomGroupContainer {
      std::vector<AtomGroup> atom_groups_;
     
      bool group_types_uptodate_ = false; 
      /**
       * @brief Checks to makes sure the group is unique
       *
       * @param atom_group
       *
       * @return 
       */
      bool isUniqueGroup_( AtomGroup atom_group );


      /**
       * @brief Match the indices of the first atom group with any that share 
       * coordinates and the same element in atom group 2
       *
       * @param grp1
       * @param grp2
       *
       * @return map returns indices in group two that match group 1 
       */
      std::unordered_map<int,int> matchAtoms_( const AtomGroup & grp1, const AtomGroup & grp2) const;

      void assignBasisFuncCountToComplex_(const std::vector<int>& complex_basis_func_count);
      void assignBasisFuncCountToComponents_();

      struct AtomMatch {
        int index_component;
        int index_component_atom;
        int index_complex_atom;
      };

      std::vector<AtomMatch> matchComponentAtomsToComplex_();
      int index_of_complex_ = -1;
    public: 
      void add( AtomGroup atom_group );

      /**
       * @brief This algorithm determines if a complex exists
       *
       * This algorithm does not compare the atoms in the complex with the
       * atoms in the components, so it is not a rigourous algorithm but should
       * be sufficient for our purposes.
       *
       * Only a single atom group can be the complex
       */
      void assignAtomGroupTypes();

      /**
       * @brief Determine if a complex exists within the atom groups
       *
       * Only a single atom group can be a complex
       *
       * @return 
       */
      bool complexExists() const ;

      /**
       * @brief If a complex exists and we know the number of basis functions
       * associated with each atom in the complex we can assign the basis function
       * number to each of the atoms
       *
       * @param complex_basis_func_count
       */
      void assignBasisFunctionCount(const std::vector<int>& complex_basis_func_count);

      int getTotalBasisFunctionCount(const GroupType & type) const;

      int getMaxBasisFunctionCount(const GroupType & type) const;
      /**
       * @brief Get the indices of all the groups of the specified group type
       *
       * @param type
       *
       * @return 
       */
      std::vector<int> getGroups(const GroupType & type) const;     

      AtomGroup getGroup(size_t ind) const { return  atom_groups_.at(ind);}

      
  };

}  // namespace catnip
#endif  // _CATNIP_ATOM_GROUP_CONTAINER_HPP
