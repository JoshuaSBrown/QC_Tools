#pragma once
#ifndef CATNIP_ATOM_GROUP_CONTAINER_HPP
#define CATNIP_ATOM_GROUP_CONTAINER_HPP

// Local private includes
#include "atom_group.hpp"

// Third party includes
#include <eigen3/Eigen/Dense>

// Standard includes
#include <exception>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace catnip {

  class Atom;

  /// What we need is a function that will actually create the atom group
  /// container so that the state is known, basically you need to pass in the
  /// a vector of the atom groups
  class AtomGroupContainer {
      std::vector<AtomGroup> atom_groups_;
     
      bool group_types_uptodate_ = false; 

      /**
       * @brief Checks to makes sure the group is unique
       *
       * Does not compare atom addresses just the atoms in the group the size
       * Ignores the atom group name in the comparison
       * @param atom_group
       *
       * @return 
       */
      bool isUniqueGroup_( const AtomGroup & atom_group ) const;

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

      size_t size() const noexcept { return  atom_groups_.size(); }

      /**
       * @brief This algorithm assignes a group type to each each of the groups 
       */
      void assignGroupTypes();

      /**
       * @brief Determine if a specified group type exists within the atom groups
       *
       * @return 
       */
      bool exists(GroupType type) const;

      GroupType getType(int index) const { return atom_groups_.at(index).getType(); }
      /**
       * @brief If a complex exists and we know the number of basis functions
       * associated with each atom in the complex we can assign the basis function
       * number to each of the atoms
       *
       * @param complex_basis_func_count
       */
      void assignBasisFunctions(const std::vector<int>& complex_basis_func_count);

      int getTotalBasisFunctions(const GroupType & type) const;

      int getMaxBasisFunctions(const GroupType & type) const;
      /**
       * @brief Get the indices of all the groups of the specified group type
       *
       * @param type
       *
       * @return 
       */
      std::vector<int> getGroups(const GroupType & type) const;     

      AtomGroup & at(size_t ind) { return  atom_groups_.at(ind);}
  };

  

}  // namespace catnip
#endif  // CATNIP_ATOM_GROUP_CONTAINER_HPP
