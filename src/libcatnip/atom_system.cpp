
// Local private includes 
#include "atom_system.hpp"

#include "atom.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace catnip {

  /****************************************************************************
   * Local private structs
   ****************************************************************************/
/*  struct AtomMatch {
    int index_component;
    int index_component_atom;
    int index_complex_atom;
  };*/

  /****************************************************************************
   * Local private functions
   ****************************************************************************/
  
  static void assignBasisFuncs_(
      int grp_ind,
      const std::vector<int>& basis_func_count,
      AtomGroupContainer & atm_grp_cont) {
    // Cycle through the atoms in the complex and assign the correct number of
    // basis functions
    int index = 0; 
    for( std::shared_ptr<Atom> & atom_ptr : atm_grp_cont.at(grp_ind) ){
      if( atom_ptr->getBasisFuncCount() == -1){
        atom_ptr->setBasisFuncCount(basis_func_count.at(index));
      } else if(atom_ptr->getBasisFuncCount() != basis_func_count.at(index) ){
        // Make sure the new assignment is consistent with the old one
        // if not throw an error, because you have a conflicting number of basis
        // functions between groups
        throw std::runtime_error("Inconsistency detected in the number of "
            "basis functions detected between atom groups. Ensure that the "
            "basis function count vector is in a consistent order with the "
            "atoms in the atom group");
      }
      index++;
    }
  }
/*
  static std::vector<AtomMatch> matchComponentAtomsToComplex_(){

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
  }*/

  static void checkValidSystemCriteria_(AtomGroupContainer & atm_grp_cont){
    std::vector<int> complex_indices = 
      atm_grp_cont.getGroups(GroupType::Complex);

    if( complex_indices.size() == 0 ){
      throw std::runtime_error("Cannot create atom system, no complexes were "
          "found in the atom group container.");
    } else if( complex_indices.size() > 1 ){
      throw std::runtime_error("Cannot create atom system, several complexes "
          "were found in the atom group. The atom system can only contain a "
          "single complex.");
    }

    std::vector<int> component_indices = 
      atm_grp_cont.getGroups(GroupType::Component);

    if ( component_indices.size() < 2){
      throw std::runtime_error("Cannot create atom system, must contain at "
          "least two components"); 
    }
  }

  static void synchroniseAtoms_(AtomGroupContainer & atm_grp_cont){

    // Overwrite any of the pointers in the component that have the same atom
    // with the complex, with the atom pointer from the complex
    int complex_index = 
      atm_grp_cont.getGroups(GroupType::Complex).at(0);

    std::vector<int> component_indices = 
      atm_grp_cont.getGroups(GroupType::Component);

    AtomGroup & atm_complex = atm_grp_cont.at(complex_index);
    for ( const int component_ind : component_indices){
      AtomGroup & atm_grp = atm_grp_cont.at(component_ind);
      // Cycle atoms in atm_grp 
      for ( std::shared_ptr<Atom> & atm : atm_grp){
        std::vector<int> complex_atm_ind = atm_complex.find(atm);
        // This vector should be 0 or 1
        if( complex_atm_ind.size() == 1){
          // Change the pointer
          atm = atm_complex.at(complex_atm_ind.at(0)); 
        }
      }
    }
  }
  /****************************************************************************
   * Public Methods
   ****************************************************************************/

  /**
   * @brief Creates an atom system if possible
   *
   * The criteria to create an atom system is that the AtomGroupContainer must
   * contain only a single complex and the components that belong to that
   * complex
   *
   * @param atm_grp_cont
   */
  AtomSystem::AtomSystem(AtomGroupContainer atm_grp_cont) {
    atm_grp_cont.assignGroupTypes();

    checkValidSystemCriteria_(atm_grp_cont);

    // Make atoms consistent between components and the complex, this is done
    // by making atoms that share the same location point to the same place
    // in memory
    synchroniseAtoms_(atm_grp_cont);

    atm_grp_cont_ = atm_grp_cont;
  }

  int AtomSystem::getTotalBasisFunctions(const GroupType & type) const {
    int total_num_basis = 0;
    std::vector<int> group_indices = atm_grp_cont_.getGroups(type);     
    for ( int ind : group_indices ){
      const AtomGroup & group = atm_grp_cont_.at(ind);
      for( const std::shared_ptr<Atom> & atom_ptr : group ) {
        total_num_basis += atom_ptr->getBasisFuncCount();
      }
    } 
    return total_num_basis;
  }

  int AtomSystem::getMaxBasisFunctions(const GroupType & type) const {
    int max_num_basis = 0;
    std::vector<int> group_indices = atm_grp_cont_.getGroups(type);     
    for ( int ind : group_indices ){
      const AtomGroup & group = atm_grp_cont_.at(ind);
      for( const std::shared_ptr<Atom> & atom_ptr : group ) {
        if( atom_ptr->getBasisFuncCount() > max_num_basis){
          max_num_basis = atom_ptr->getBasisFuncCount();
        }
      }
    } 
    return max_num_basis;
  }


  void AtomSystem::assignBasisFunctions(int index, 
      const std::vector<int>& basis_func_count) {

    // 2. Check that the number of atoms in the grp is consistent with 
    // the number of values provided
    if ( basis_func_count.size() != atm_grp_cont_.at(index).size() ){
      throw std::runtime_error("Cannot assign basis function count to atoms "
          "as the basis function count vector is not equal to the number of "
          "atoms in the group!");
    }

    assignBasisFuncs_(index, basis_func_count, atm_grp_cont_);
  }

  bool AtomSystem::systemComplete() const noexcept {
    for ( const AtomGroup & grp : atm_grp_cont_){
      for( const std::shared_ptr<Atom> & atm : grp){
        if(atm->getBasisFuncCount() == -1) return false;
      }
    }
    return true;
  }

  const AtomGroup & AtomSystem::at(int ind) const {
    return atm_grp_cont_.at(ind); 
  }
}
