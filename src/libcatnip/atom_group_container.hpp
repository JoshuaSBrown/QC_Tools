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

  /**
   * @brief Allows auto detection of atom groups
   *
   * What this means is that if you pass in atom groups and thier state is not
   * known, this class can automatically detect if they are related, if they
   * are an island a complex or a component.
   *
   * Accurate assignment of a group type can only be achieved after all of the 
   * groups are added. If a new group is added after group types have been 
   * assigned it will invalidate the previously assigned group types. 
   */
  class AtomGroupContainer {
      std::vector<std::unique_ptr<AtomGroup>> atom_groups_;
     
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
      bool isUniqueGroup_( const std::unique_ptr<AtomGroup> & atom_group ) const;


    public: 

      // Invalidates groups if non const because cannot ensure that groups are
      // not changed
      std::vector<std::unique_ptr<AtomGroup>>::iterator 
        begin() { group_types_uptodate_ = false; return atom_groups_.begin(); }

      std::vector<std::unique_ptr<AtomGroup>>::const_iterator 
        begin() const { return atom_groups_.begin(); }

      // Invalidates groups if non const because cannot ensure that groups are
      // not changed
      std::vector<std::unique_ptr<AtomGroup>>::iterator 
        end() { group_types_uptodate_ = false; return atom_groups_.end(); }
 
      std::vector<std::unique_ptr<AtomGroup>>::const_iterator 
        end() const { return atom_groups_.end(); }

      void add( std::unique_ptr<AtomGroup> atom_group );

      size_t size() const noexcept { return  atom_groups_.size(); }

      /**
       * @brief This algorithm assignes a group type to each each of the groups 
       */
      void assignGroupTypes();

      bool isUpToDate() const noexcept { return group_types_uptodate_; }

      /**
       * @brief Determine if a specified group type exists within the atom groups
       *
       * @return 
       */
      bool exists(GroupType type) const;

      const GroupType & getType(int index) const { return atom_groups_.at(index)->getType(); }

      std::vector<GroupType> getType(const std::string & group_name) const; 

      /**
       * @brief Get the indices of all the groups of the specified group type
       *
       * @param type
       *
       * @return 
       */
      std::vector<int> getGroups(const GroupType & type) const noexcept;     

      std::unique_ptr<AtomGroup> & at(size_t ind) { 
        group_types_uptodate_ = false; 
        return atom_groups_.at(ind);
      }

      const std::unique_ptr<AtomGroup> & at(size_t ind) const { return   atom_groups_.at(ind);}
  };

  

}  // namespace catnip
#endif  // CATNIP_ATOM_GROUP_CONTAINER_HPP
