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

  void AtomGroupContainer::assignBasisFuncCountToComplex_(const std::vector<int>& complex_basis_func_count) {
    // Cycle through the atoms in the complex and assign the correct number of
    // basis functions
    int index = 0; 
    for( std::shared_ptr<Atom> & atom_ptr : atom_groups_.at(index_of_complex_) ){
      atom_ptr->setBasisFuncCount(complex_basis_func_count.at(index));
      index++;
    }
  }

  // Assumes the complex atoms already have been assigned the basis functioncount
  void AtomGroupContainer::assignBasisFuncCountToComponents_() {
    std::vector<AtomMatch> paired_atoms = matchComponentAtomsToComplex_();
    for ( AtomMatch atom_match : paired_atoms ) {
      int basis_func_count = atom_groups_.at(index_of_complex_).at(atom_match.index_complex_atom)->getBasisFuncCount();
      atom_groups_.at(atom_match.index_component).at(atom_match.index_component_atom)->setBasisFuncCount(basis_func_count);
    } 
  }

  std::vector<int> AtomGroupContainer::getGroups(const GroupType & type) const {
    std::vector<int> groups;
    int ind1 = 0;
    for( const AtomGroup & group : atom_groups_ ){
      if( group.getType() == type){
        groups.push_back(ind1);
      }
      ++ind1;
    }
    return groups;
  }
  std::unordered_map<int,int> AtomGroupContainer::matchAtoms_( 
      const AtomGroup & grp1, const AtomGroup & grp2) const {
  
    std::unordered_map<int,int> paired_atoms;  
    for ( size_t ind1 = 0; ind1 < grp1.size(); ++ind1){
      for (size_t ind2 = 0; ind2 < grp2.size(); ++ind2){
        if ( *(grp1.at(ind1)) == *(grp2.at(ind2)) ){
          if ( paired_atoms.count(ind1) ) {
            throw std::runtime_error("Atom in complex has already been paired with an atom in the component, you have two or more atoms in a component that are redundant with an atom in the complex");
          }
          paired_atoms[ind1] = ind2;
          break;
        } 
      }
    }
    return paired_atoms;
  }


  bool AtomGroupContainer::isUniqueGroup_( const AtomGroup & atom_group ) const {
    // Step 1 check the size of the atom group 
    if(atom_groups_.size() == 0) return true;

    for ( const AtomGroup & grp : atom_groups_ ){
      if ( grp.size() == atom_group.size() ){
        std::vector<bool> matchs(grp.size(),false);
        // Determine if a match is found
        for ( const auto & atm : atom_group ) {
          size_t index = 0;
          for ( bool matched : matchs ){
            if ( not matched ){
              // Compare atoms 
              if ( *atm == *(grp.at(index)) ){
                matchs.at(index) = true;
                break;
              }
            } 
            ++index;
          }
          // If an atom was not matched it means that there is at least one 
          // difference, and so we do not need to cycle through the other
          // atoms in this particular group 
          if(index == grp.size()) break;
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
  // As soon as we add a new atom group it should invalidate the labels on 
  // all the other groups
  void AtomGroupContainer::add( AtomGroup atom_group ) {
    std::cout << "Adding group " << atom_group.getName() << std::endl; 
    if( isUniqueGroup_(atom_group) ){
      std::cout << "Is unique" << std::endl;
      atom_groups_.push_back(atom_group);
    } else {
      return;
    }

    for ( AtomGroup & grp : atom_groups_ ){
      grp.setType(GroupType::Unassigned);
    }
    group_types_uptodate_ = false;
    index_of_complex_ = -1;
  }

  struct GroupAtomIndex {
    int atm_ind;
    int grp_ind; 
  };

  static std::unordered_map<Atom,std::vector<GroupAtomIndex>> mapAtomsToGroups(
      const std::vector<AtomGroup> & atom_groups_ ){

    std::unordered_map<Atom,std::vector<GroupAtomIndex>> atm_map;
    int grp_index = 0;
    for ( const AtomGroup & grp : atom_groups_ ){
      int atom_index = 0;
      for ( auto & atom_ptr : grp ){
        atm_map[*atom_ptr].push_back(GroupAtomIndex{atom_index,grp_index});
        ++atom_index;
      }
      ++grp_index;
    }
    return atm_map;
  }

  static const std::map<int,std::vector<int>> calculateConnectedGroups(
      std::unordered_map<Atom,std::vector<GroupAtomIndex>> atm_map,
      const std::vector<AtomGroup> & atom_groups_ 
      ){

    std::map<int,std::vector<int>> grp_indices;
    int grp_index = 0;
    for ( const AtomGroup & grp : atom_groups_ ){
      // Initialize
      grp_indices[grp_index];
      for ( const auto & atom_ptr : grp ){
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

  static void identifyAtomGroupsOfUnitType(
      const std::map<int,std::vector<int>> & grp_indices,
      std::vector<AtomGroup> & atom_groups_ 
      ) {
    for ( std::pair<int,std::vector<int>> grp_groups : grp_indices){
      if(grp_groups.second.size() == 0) {
        std::cout << "Empty makeing unit, does not share atoms" << std::endl;
        atom_groups_.at(grp_groups.first).setType(GroupType::Unit);
      }
    }
  }

  static void identifyAtomGroupsOfComponentComplexAndUnknownTypes(
      const std::map<int,std::vector<int>> & grp_indices,
      std::vector<AtomGroup> & atom_groups_ 
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
        atom_groups_.at(potential_complex).setType(GroupType::Complex);
        for ( const int & candidate_component : candidate_components){
          atom_groups_.at(candidate_component).setType(GroupType::Component);
        }
      }else{
        atom_groups_.at(potential_complex).setType(GroupType::Unknown);
        for ( const int & candidate_component : candidate_components){
          atom_groups_.at(candidate_component).setType(GroupType::Unknown);
        }
      }
    }
  }
// Create a map with an atom as the key and a vector of indices to indicate 
// where there is overlap, the group and index of where the overlap occurs 
  void AtomGroupContainer::assignGroupTypes(){

    std::cout << "Assigning group types" << std::endl;
    std::unordered_map<Atom,std::vector<GroupAtomIndex>> atm_map = mapAtomsToGroups( atom_groups_ );

    std::cout << "Size of atom map " << atm_map.size() << std::endl;
    // For each atom in a group find out how many other groups share the atoms
    // First index is the group index the vector stores the other groups that
    // share atoms
    const std::map<int,std::vector<int>> grp_indices = calculateConnectedGroups(atm_map, atom_groups_ );

    for ( const auto & pr : grp_indices ) {
      std::cout << "group " << pr.first << std::endl;
      for ( auto ind : pr.second ){
        std::cout << "Atoms shared with group " << ind << std::endl;
      }
    }
    // Calculate which groups do not share atoms with other groups these are units
    identifyAtomGroupsOfUnitType(grp_indices, atom_groups_);

    for ( const auto & pr : grp_indices ) {
      std::cout << "group " << pr.first << std::endl;
      for ( auto ind : pr.second ){
        std::cout << "Atoms shared with group " << ind << std::endl;
      }
    }
    identifyAtomGroupsOfComponentComplexAndUnknownTypes(grp_indices,atom_groups_ );
  }

  int AtomGroupContainer::getTotalBasisFunctions(const GroupType & type) const {
    int total_num_basis = 0;
    std::vector<int> group_indices = getGroups(type);     
    for ( int ind : group_indices ){
      AtomGroup group = atom_groups_.at(ind);
      for( const std::shared_ptr<Atom> & atom_ptr : group ) {
        total_num_basis += atom_ptr->getBasisFuncCount();
      }
    } 
    return total_num_basis;
  }

  int AtomGroupContainer::getMaxBasisFunctions(const GroupType & type) const {
    int max_num_basis = 0;
    std::vector<int> group_indices = getGroups(type);     
    for ( int ind : group_indices ){
      AtomGroup group = atom_groups_.at(ind);
      for( const std::shared_ptr<Atom> & atom_ptr : group ) {
        if( atom_ptr->getBasisFuncCount() > max_num_basis){
          max_num_basis = atom_ptr->getBasisFuncCount();
        }
      }
    } 
    return max_num_basis;
  }

  bool AtomGroupContainer::exists(GroupType type) const {
    // First check that all the group types have been assigned
    for ( const AtomGroup & grp : atom_groups_ ){
      if( grp.getType() == type ) return true;
    }
    return false; 
  }

  std::vector<AtomGroupContainer::AtomMatch> AtomGroupContainer::matchComponentAtomsToComplex_(){

    AtomGroup complex_group = atom_groups_.at(index_of_complex_);
    std::vector<int> component_indices = getGroups(GroupType::Component);
    std::vector<AtomMatch> all_paired_atoms;
    for ( const int & component_index : component_indices ) {
      std::unordered_map<int,int> paired_atoms = matchAtoms_(complex_group,atom_groups_.at(component_index));
      // Pass to vector
      for ( auto pr : paired_atoms ){
        AtomMatch atom_match;
        atom_match.index_component = component_index;
        atom_match.index_component_atom = pr.second;
        atom_match.index_complex_atom = pr.first;
        all_paired_atoms.push_back(atom_match);
      }
    }
    return all_paired_atoms;
  }

  void AtomGroupContainer::assignBasisFunctions(
      const std::vector<int>& complex_basis_func_count) {

    // 1. Check that up to date
    if ( group_types_uptodate_ == false ){
      throw std::runtime_error("Cannot assign basis functions to atoms as group types are not up to date!");
    }
    // 2. Check that the number of atoms in the complex is consistent with the number of values provided
    if ( complex_basis_func_count.size() != atom_groups_.at(index_of_complex_).size() ){
      throw std::runtime_error("Cannot assign basis function count to atoms as the basis function count vector is not equal to the number of atoms!");
    }

    assignBasisFuncCountToComplex_(complex_basis_func_count);
    assignBasisFuncCountToComponents_();
    
  }

   
}  // namespace catnip
