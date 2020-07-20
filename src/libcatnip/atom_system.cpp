
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

  static AtomGroup & assignBasisFunctionsToSynchronisedAtoms_(
      int grp_ind, 
      const std::vector<int>& basis_func_count,
      AtomGroup & grp,
      AtomGroupContainer & atm_grp_cont ){

    int index = 0; 
    for( std::shared_ptr<Atom> & atom_ptr : grp ){
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

  static void checkLinkedAtomsInComplexForConsistentBasisFunctionCounts(
      int grp_ind, 
      const AtomGroup & grp,
      AtomGroupContainer & atm_grp_cont,
      std::map<int,std::vector<std::pair<int,int>>> linked_atoms ){

    for ( auto grp_linked_atms : linked_atoms ){ 
      const AtomGroup & component = atm_grp_cont.at(grp_linked_atms.first);
      for( std::pair<int,int> linked_atoms : grp_linked_atms.second ){
        if ( component.at(linked_atoms.first)->getBasisFuncCount() != -1){
          if ( component.at(linked_atoms.first)->getBasisFuncCount() != grp.at(linked_atoms.second)->getBasisFuncCount()){
            throw std::runtime_error("There are inconsistencies between the "
                "the number of basis functions used in a linked atom. Atoms "
                "in a component must contain the same number of basis "
                "functions as thier linked atoms in the complex");
          }
        }  
      }
    }
  }

  static void checkLinkedAtomsInComponentForConsistentBasisFunctionCounts(
      int grp_ind, 
      const AtomGroup & grp,
      AtomGroupContainer & atm_grp_cont,
      std::map<int,std::vector<std::pair<int,int>>> linked_atoms 
      ) {

    std::vector<int> complex_grps = atm_grp_cont.getGroups(GroupType::Complex);
    assert(complex_grps.size() == 1 && "There must be at least one complex");
    const AtomGroup & complex_grp = atm_grp_cont.at(complex_grps.at(0));
    for( std::pair<int,int> linked_atoms : linked_atoms[grp_ind] ){
      if ( complex_grp.at(linked_atoms.first)->getBasisFuncCount() != -1){
        if ( complex_grp.at(linked_atoms.first)->getBasisFuncCount() != grp.at(linked_atoms.second)->getBasisFuncCount()){
          throw std::runtime_error("There are inconsistencies between the "
              "the number of basis functions used in a linked atom. Atoms "
              "in a component must contain the same number of basis "
              "functions as thier linked atoms in the complex");
        }
      }
    }
  }

  /**
   * @brief Assigns basis function to atoms in group
   *
   * This fucntions should only be called after the atoms have been syncrhonized
   * between atom groups
   *
   * @param grp_ind
   * @param basis_func_count
   * @param atm_grp_cont
   */
  static void assignBasisFuncs_(
      int grp_ind,
      const std::vector<int>& basis_func_count,
      AtomGroupContainer & atm_grp_cont,
      std::map<int,std::vector<std::pair<int,int>>> linked_atoms 
      ) {
    // Cycle through the atoms in the complex and assign the correct number of
    // basis functions
  
    AtomGroup & grp = atm_grp_cont.at(grp_ind);

    assignBasisFunctionsToSynchronisedAtoms_(
      grp_ind, 
      basis_func_count,
      grp,
      atm_grp_cont);
    // Make sure that there are no inconsistencies between the linked atoms
    if ( atm_grp_cont.at(grp_ind).is(GroupType::Complex)){
      // If it is a complex
      checkLinkedAtomsInComplexForConsistentBasisFunctionCounts(
          grp_ind, 
          grp,
          atm_grp_cont,
          linked_atoms);

    }else {
      // If it is a component
      checkLinkedAtomsInComponentForConsistentBasisFunctionCounts(
          grp_ind, 
          grp,
          atm_grp_cont,
          linked_atoms);
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

  // Map 
  // first int - index of the group
  // first int of the pair - index of the atom in the component
  // second int of the pair - index of the linked atom in the complex 
  static std::map<int,std::vector<std::pair<int,int>>> findNonConsistentAtoms_(
      AtomGroupContainer & atm_grp_cont) {

    std::map<int,std::vector<std::pair<int,int>>> linked_atoms;
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
      int ind_atm = 0;
      for ( std::shared_ptr<Atom> & atm : atm_grp){
        std::vector<int> complex_atm_ind = atm_complex.find(atm->getPos());
        // This vector must be of size 1
        if( complex_atm_ind.size() == 1){
          // If the elements dont' match it is non-consistent 
          if( atm_complex.at(complex_atm_ind.at(0))->getElement() != atm_grp.at(ind_atm)->getElement() ){
            linked_atoms[component_ind].push_back(std::pair<int,int>(ind_atm,complex_atm_ind.at(0)));
          }
        }else if (complex_atm_ind.size() == 0) {
          throw std::runtime_error("A component atom did not have a matching "
              "atom in the complex. The component atom does not have to have "
              "the same element as the atom in the complex bust must share "
              "the same position.");
        } else {
          throw std::runtime_error("A component atom matches more than a single"
             " atom in the complex. There appears to be a problem with the "
             "complex, as more than one atom occupies the same location.");
        }
        ++ind_atm;
      }
    }
    return linked_atoms;
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

    // Searches for atoms that exist in the component that share a position with
    // an atom in the complex, but are of a different element
    linked_atoms_ = findNonConsistentAtoms_(atm_grp_cont);

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
