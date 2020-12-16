// Local private includes
#include "atom_group_container.hpp"

#include "atom.hpp"
#include "atom_group.hpp"

// Third party includes
#include <eigen3/Eigen/Dense>

// Standard includes
#include <exception>
#include <iterator>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_map>

namespace catnip {

  /****************************************************************************
   * Local private structs
   ****************************************************************************/
  struct GroupAtomIndex {
    int atm_ind;
    int grp_ind; 
  };

  /****************************************************************************
   * Local private functions
   ****************************************************************************/
  static std::unordered_map<Atom,std::vector<GroupAtomIndex>> mapAtomsToGroups_(
      const std::vector<std::unique_ptr<AtomGroup>> & atom_groups_ ){

    std::unordered_map<Atom,std::vector<GroupAtomIndex>> atm_map;
    int grp_index = 0;
    for ( const std::unique_ptr<AtomGroup> & grp : atom_groups_ ){
      int atom_index = 0;
      for ( auto & atom_ptr : *grp ){
        atm_map[*atom_ptr].push_back(GroupAtomIndex{atom_index,grp_index});
        ++atom_index;
      }
      ++grp_index;
    }
    return atm_map;
  }

  static const std::map<int,std::vector<int>> calculateConnectedGroups_(
      std::unordered_map<Atom,std::vector<GroupAtomIndex>> atm_map,
      const std::vector<std::unique_ptr<AtomGroup>> & atom_groups_ 
      ){

    std::map<int,std::vector<int>> grp_indices;
    int grp_index = 0;
    for ( const std::unique_ptr<AtomGroup> & grp : atom_groups_ ){
      // Initialize
      grp_indices[grp_index];
      for ( const auto & atom_ptr : *grp ){
        for ( const GroupAtomIndex & grp_atm_ind : atm_map[*atom_ptr] ){
          if (grp_atm_ind.grp_ind != grp_index ){
            // Check that group has not already been added
            if( grp_indices[grp_index].size() > 0 ){
              if( grp_indices[grp_index].back() != grp_atm_ind.grp_ind ) {
                grp_indices[grp_index].push_back(grp_atm_ind.grp_ind);
              }
            }else {
              grp_indices[grp_index].push_back(grp_atm_ind.grp_ind);
            }
          }
        } 
      }
      ++grp_index;

    }
    return grp_indices;
  }

  static void identifyAtomGroupsOfIslandType_(
      const std::map<int,std::vector<int>> & grp_indices,
      std::vector<std::unique_ptr<AtomGroup>> & atom_groups_ 
      ) {
    for ( std::pair<int,std::vector<int>> grp_groups : grp_indices){
      if(grp_groups.second.size() == 0) {
        std::cout << "Empty makeing unit, does not share atoms" << std::endl;
        atom_groups_.at(grp_groups.first)->setType(GroupType::Island);
      }
    }
  }

  static void identifyAtomGroupsOfComponentComplexAndUnknownTypes_(
      const std::map<int,std::vector<int>> & grp_indices,
      std::vector<std::unique_ptr<AtomGroup>> & atom_groups_ 
      ){
    std::vector<int> potential_complexes;
    for ( std::pair<int,std::vector<int>> grp_groups : grp_indices){
      if( grp_groups.second.size() > 1 ){
        potential_complexes.push_back(grp_groups.first);
      } 
    }

    for( const int & potential_complex : potential_complexes){
      std::vector<int> candidate_components = grp_indices.at(potential_complex); 
      bool candidate_components_valid = true;
      for ( const int & candidate_component : candidate_components){
        std::cout << "Candiate component " << candidate_component << std::endl;
        std::cout << "number of other groups sharing atoms " << grp_indices.at(candidate_component).size() << std::endl;
        if ( grp_indices.at(candidate_component).size() != 1){
          candidate_components_valid = false;
          break; // these are not components, and potential complex is not a complex
        } 
      }
      if ( candidate_components_valid){
        atom_groups_.at(potential_complex)->setType(GroupType::Complex);
        for ( const int & candidate_component : candidate_components){
          atom_groups_.at(candidate_component)->setType(GroupType::Component);
        }
      }else{
        atom_groups_.at(potential_complex)->setType(GroupType::Unknown);
        for ( const int & candidate_component : candidate_components){
          atom_groups_.at(candidate_component)->setType(GroupType::Unknown);
        }
      }
    }
  }

  /****************************************************************************
   * Private Methods
   ****************************************************************************/
  bool AtomGroupContainer::isUniqueGroup_( const std::unique_ptr<AtomGroup> & atom_group ) const {
    // Step 1 check the size of the atom group 
    if(atom_groups_.size() == 0) return true;

    for ( const std::unique_ptr<AtomGroup> & grp : atom_groups_ ){
      if ( grp->size() == atom_group->size() ){
        std::vector<bool> matchs(grp->size(),false);
        // Determine if a match is found
        for ( const auto & atm : *atom_group ) {
          size_t index = 0;
          for ( bool matched : matchs ){
            if ( not matched ){
              // Compare atoms 
              if ( *atm == *(grp->at(index)) ){
                matchs.at(index) = true;
                break;
              }
            } 
            ++index;
          }
          // If an atom was not matched it means that there is at least one 
          // difference, and so we do not need to cycle through the other
          // atoms in this particular group 
          if(index == grp->size()) break;
        } 
        // Count the number of matches in the vector if all of the elements match
        // then the atom group is not unique
        for ( bool match : matchs ) {
          if ( match == false ) break;
        } 
        return false;
      }
    }
    return true;
  }

  /****************************************************************************
   * Public Methods
   ****************************************************************************/

  std::vector<int> AtomGroupContainer::getGroups(const GroupType & type) const noexcept{
    std::vector<int> groups;
    int ind1 = 0;
    for( const std::unique_ptr<AtomGroup> & group : atom_groups_ ){
      if( group->getType() == type){
        groups.push_back(ind1);
      }
      ++ind1;
    }
    return groups;
  }

  // As soon as we add a new atom group it should invalidate the labels on 
  // all the other groups
  void AtomGroupContainer::add( std::unique_ptr<AtomGroup> atom_group ) {
    std::cout << "Adding group " << atom_group->getName() << std::endl; 
    if( isUniqueGroup_(atom_group) ){
      std::cout << "Is unique" << std::endl;
      atom_groups_.push_back(std::move(atom_group));
    } else {
      return;
    }

    group_types_uptodate_ = false;
  }

// Create a map with an atom as the key and a vector of indices to indicate 
// where there is overlap, the group and index of where the overlap occurs 
  void AtomGroupContainer::assignGroupTypes(){

    // If already uptodate do nothing
    if( group_types_uptodate_ ) return;
    std::unordered_map<Atom,std::vector<GroupAtomIndex>> atm_map = mapAtomsToGroups_( atom_groups_ );

    // For each atom in a group find out how many other groups share the atoms
    // First index is the group index the vector stores the other groups that
    // share atoms
    const std::map<int,std::vector<int>> grp_indices = calculateConnectedGroups_(atm_map, atom_groups_ );

    // Calculate which groups do not share atoms with other groups these are units
    identifyAtomGroupsOfIslandType_(grp_indices, atom_groups_);

    identifyAtomGroupsOfComponentComplexAndUnknownTypes_(grp_indices,atom_groups_ );

    group_types_uptodate_ = true;
  }

  bool AtomGroupContainer::exists(GroupType type) const {
    // First check that all the group types have been assigned
    for ( const std::unique_ptr<AtomGroup> & grp : atom_groups_ ){
      if( grp->getType() == type ) return true;
    }
    return false; 
  }

  std::vector<GroupType> AtomGroupContainer::getType(const std::string & group_name) const { 
    std::vector<GroupType> group_types;
    for ( const std::unique_ptr<AtomGroup> & grp : atom_groups_ ){
      if ( group_name == grp->getName() ) {
        group_types.push_back(grp->getType());
      }
    } 
    return group_types;
  }

}  // namespace catnip
