
#include "atom_group_container.hpp"

#include "atom.hpp"
#include "atom_group.hpp"

#include <exception>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>

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
        if ( grp1.at(ind1)->equal(*grp2.at(ind2)) ){
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


  void AtomGroupContainer::add( AtomGroup atom_group ) {
    if( isUniqueGroup(atom_group) ){
      atom_groups_.push_back(atom_group);
    }
    group_types_uptodate_ = false;
    index_of_complex_ = -1;
  }

  void AtomGroupContainer::assignAtomGroupTypes(){
    if (group_types_uptodate_ ) return;
    std::vector<size_t> number_atoms;        
    for( AtomGroup group : atom_groups_){
      number_atoms.push_back(group.numberOfAtoms());
    }
    auto max_elem_it = max_element(number_atoms.begin(),number_atoms.end()); 
    size_t max_elem = *max_elem_it; 
    for( AtomGroup group : atom_groups_){
      group.setType(GroupType::Component);
    } 
    size_t number_atoms_excluding_max = std::accumulate(number_atoms.begin(),number_atoms.end(),-1*max_elem);
    if ( std::count(number_atoms.begin(),number_atoms.end(), max_elem) == 1 &&
        number_atoms_excluding_max == max_elem ){
      for( AtomGroup group : atom_groups_){
        if( group.numberOfAtoms() == max_elem ){
          group.setType(GroupType::Complex);
          index_of_complex_ = std::distance(number_atoms.begin(),max_elem_it);
        } 
      }
    }
    group_types_uptodate_ = true;
  }

  int AtomGroupContainer::getTotalBasisFunctionCount(const GroupType & type) const {
    int total_num_basis = 0;
    std::vector<int> group_indices = getGroups(type);     
    for ( int ind : group_indices ){
      AtomGroup group = getGroup(ind);
      for( const std::shared_ptr<Atom> & atom_ptr : group ) {
        total_num_basis += atom_ptr->getBasisFuncCount();
      }
    } 
    return total_num_basis;
  }

  int AtomGroupContainer::getMaxBasisFunctionCount(const GroupType & type) const {
    int max_num_basis = 0;
    std::vector<int> group_indices = getGroups(type);     
    for ( int ind : group_indices ){
      AtomGroup group = getGroup(ind);
      for( const std::shared_ptr<Atom> & atom_ptr : group ) {
        if( atom_ptr->getBasisFuncCount() > max_num_basis){
          max_num_basis = atom_ptr->getBasisFuncCount();
        }
      }
    } 
    return max_num_basis;
  }

  bool AtomGroupContainer::complexExists() const {
    // First check that all the group types have been assigned
    if ( group_types_uptodate_ == false ){
      throw std::runtime_error("Cannot determine if complex exists group types are not up to date");
    }
    return index_of_complex_ > -1;
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
  }

  void AtomGroupContainer::assignBasisFunctionCount(
      const std::vector<int>& complex_basis_func_count) {

    // 1. Check that up to date
    if ( group_types_uptodate_ == false ){
      throw std::runtime_error("Cannot assign basis functions to atoms as group types are not up to date!");
    }
    // 2. Check that the number of atoms in the complex is consistent with the number of values provided
    if ( complex_basis_func_count.size() != atom_groups_.at(index_of_complex_).numberOfAtoms() ){
      throw std::runtime_error("Cannot assign basis function count to atoms as the basis function count vector is not equal to the number of atoms!");
    }

    assignBasisFuncCountToComplex_(complex_basis_func_count);
    assignBasisFuncCountToComponents_();
    
  }

   
}  // namespace catnip
