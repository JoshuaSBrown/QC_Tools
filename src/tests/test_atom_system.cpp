
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Local private includes
#include "atom_system.hpp"

#include "atom.hpp"
#include "atom_group.hpp"
#include "atom_group_container.hpp"
#include "elements.hpp"

// Standard includes
#include <memory>

using namespace catnip;

TEST_CASE("Atom System","[unit]") {

  Atom atom1(Element::H, 0.0, 0.0, 0.0);
  Atom atom2(Element::H, 2.0, 0.0, 0.0);
  Atom atom3(Element::C, 1.0, 1.0, 0.0);
  Atom atom4(Element::O, 3.3, 4.3, 0.0);
  Atom atom5(Element::O, 3.0, 3.9, 0.0);
  // Same position as atom5 but different element
  Atom atom6(Element::C, 3.0, 3.9, 0.0); 

  auto atom1_ptr = std::make_shared<Atom>(atom1);
  auto atom2_ptr = std::make_shared<Atom>(atom2);
  auto atom3_ptr = std::make_shared<Atom>(atom3);
  auto atom4_ptr = std::make_shared<Atom>(atom4);
  auto atom5_ptr = std::make_shared<Atom>(atom5);
  auto atom6_ptr = std::make_shared<Atom>(atom6);

  SECTION("Building component without atom 5") {
    // Extra atoms are allowed in the complex but not in the componets
    AtomGroup component1("CH2");
    component1.add(atom1_ptr);
    component1.add(atom2_ptr);
    component1.add(atom3_ptr);

    AtomGroup component2("O2");
    component2.add(atom4_ptr);  

    AtomGroup complex1("O2CH2");
    complex1.add(atom1_ptr);
    complex1.add(atom2_ptr);
    complex1.add(atom3_ptr);
    complex1.add(atom4_ptr);
    complex1.add(atom5_ptr);

    AtomGroupContainer atom_grp_cont;
    atom_grp_cont.add(component1);
    atom_grp_cont.add(component2);
    atom_grp_cont.add(complex1);

    REQUIRE_NOTHROW(AtomSystem(atom_grp_cont) );
  }

  SECTION("Building complex without atom 5") {
    // Extra atoms are allowed in the complex but not in the componets
    AtomGroup component1("CH2");
    component1.add(atom1_ptr);
    component1.add(atom2_ptr);
    component1.add(atom3_ptr);

    AtomGroup component2("O2");
    component2.add(atom4_ptr);  
    component2.add(atom5_ptr);  

    AtomGroup complex1("O2CH2");
    complex1.add(atom1_ptr);
    complex1.add(atom2_ptr);
    complex1.add(atom3_ptr);
    complex1.add(atom4_ptr);

    AtomGroupContainer atom_grp_cont;
    atom_grp_cont.add(component1);
    atom_grp_cont.add(component2);
    atom_grp_cont.add(complex1);

    REQUIRE_THROWS(AtomSystem(atom_grp_cont) );
  }

  SECTION("Building with atom 5") {
    AtomGroup component1("CH2");
    component1.add(atom1_ptr); // H
    component1.add(atom2_ptr); // H 
    component1.add(atom3_ptr); // C

    AtomGroup component2("O2");
    component2.add(atom4_ptr); // O
    component2.add(atom5_ptr); // O 

    AtomGroup complex1("O2CH2");
    complex1.add(atom1_ptr); // H
    complex1.add(atom2_ptr); // H
    complex1.add(atom3_ptr); // C
    complex1.add(atom4_ptr); // O
    complex1.add(atom5_ptr); // O

    AtomGroupContainer atom_grp_cont;
    atom_grp_cont.add(component1);
    atom_grp_cont.add(component2);
    atom_grp_cont.add(complex1);

    AtomSystem atm_sys(atom_grp_cont);
    REQUIRE(atm_sys.getComplex().getType() == GroupType::Complex );
    REQUIRE(atm_sys.getComplex().getName() == "O2CH2" );

    std::vector<int> component_indices = atm_sys.getComponentIndices();
    REQUIRE(component_indices.size() == 2);
    REQUIRE(component_indices.at(0) == 0);
    REQUIRE(component_indices.at(1) == 1);

    std::vector<AtomSystem::Link> linked_atms = atm_sys.getLinkedAtomsWithDifferentElements();
    REQUIRE(linked_atms.size() == 0); 

    // Basis functions
    std::vector<int> basis_comp1{ 1, 1, 4};
    std::vector<int> basis_comp2{ 5, 5};
    // should be 5, 5 at the end
    std::vector<int> wrong_basis_complex{ 1, 1, 4, 3, 3}; 

    atm_sys.assignBasisFunctions( 0, basis_comp1 );
    atm_sys.assignBasisFunctions( 1, basis_comp2 );
   
    // Should be complete at this point
    REQUIRE( atm_sys.systemComplete() );

    // By using the basis functions for the complex we ensure that basis
    // functions between the components and the complex are consistent
    REQUIRE_THROWS( atm_sys.assignBasisFunctions( 2, wrong_basis_complex ));

    std::vector<int> basis_complex{ 1, 1, 4, 5, 5}; 
    REQUIRE_NOTHROW( atm_sys.assignBasisFunctions( 2, basis_complex ));

    REQUIRE( atm_sys.at(0).size() == 3);
    REQUIRE( atm_sys.at(0).at(0)->getBasisFuncCount() == 1); // H 
    REQUIRE( atm_sys.at(0).at(1)->getBasisFuncCount() == 1); // H
    REQUIRE( atm_sys.at(0).at(2)->getBasisFuncCount() == 4); // C
    
    REQUIRE( atm_sys.at(1).size() == 2);
    REQUIRE( atm_sys.at(1).at(0)->getBasisFuncCount() == 5); // O
    REQUIRE( atm_sys.at(1).at(1)->getBasisFuncCount() == 5); // O

    REQUIRE( atm_sys.at(2).size() == 5);
    REQUIRE( atm_sys.at(2).at(0)->getBasisFuncCount() == 1); // H 
    REQUIRE( atm_sys.at(2).at(1)->getBasisFuncCount() == 1); // H
    REQUIRE( atm_sys.at(2).at(2)->getBasisFuncCount() == 4); // C
    REQUIRE( atm_sys.at(2).at(3)->getBasisFuncCount() == 5); // O
    REQUIRE( atm_sys.at(2).at(4)->getBasisFuncCount() == 5); // O
  }

  SECTION("Building complex with atom 6") {
    AtomGroup component1("CH2");
    component1.add(atom1_ptr);
    component1.add(atom2_ptr);
    component1.add(atom3_ptr);

    AtomGroup component2("O2");
    component2.add(atom4_ptr);  
    component2.add(atom5_ptr);  

    AtomGroup complex1("OC2H2");
    complex1.add(atom1_ptr);
    complex1.add(atom2_ptr);
    complex1.add(atom3_ptr);
    complex1.add(atom4_ptr);
    complex1.add(atom6_ptr); // Carbon instead of Oxygen

    AtomGroupContainer atom_grp_cont;
    atom_grp_cont.add(component1);
    atom_grp_cont.add(component2);
    atom_grp_cont.add(complex1);

    AtomSystem atm_sys(atom_grp_cont);
    REQUIRE(atm_sys.getComplex().getType() == GroupType::Complex );
    REQUIRE(atm_sys.getComplex().getName() == "OC2H2" );

    std::vector<int> component_indices = atm_sys.getComponentIndices();
    REQUIRE(component_indices.size() == 2);
    REQUIRE(component_indices.at(0) == 0);
    REQUIRE(component_indices.at(1) == 1);

    std::vector<AtomSystem::Link> linked_atms = atm_sys.getLinkedAtomsWithDifferentElements();
    REQUIRE(linked_atms.size() == 1);
    AtomSystem::Link link = linked_atms.at(0);
    // atm5 from component 2 should be linked with atom 5 of complex
    REQUIRE(link.component_name == "O2");
    REQUIRE(link.component_ind == 1);
    REQUIRE(link.component_atm_ind == 1);
    REQUIRE(link.complex_name == "OC2H2");
    REQUIRE(link.complex_ind == 2);
    REQUIRE(link.complex_atm_ind == 4);

    // Basis functions
    std::vector<int> basis_comp1{ 1, 1, 4};
    std::vector<int> basis_comp2{ 5, 5};
    // should be 5, 5 at the end
    std::vector<int> wrong_basis_complex{ 1, 1, 4, 3, 3}; 

    atm_sys.assignBasisFunctions( 0, basis_comp1 );
    atm_sys.assignBasisFunctions( 1, basis_comp2 );
   
    // Should NOT be complete at this point, as atom 5 and 6 though positionally
    // linked are not in the same memory space
    REQUIRE( atm_sys.systemComplete() == false );

    // By using the basis functions for the complex we ensure that basis
    // functions between the components and the complex are consistent
    REQUIRE_THROWS( atm_sys.assignBasisFunctions( 2, wrong_basis_complex ));
 
    std::vector<int> basis_complex{ 1, 1, 4, 5, 5}; 
    REQUIRE_NOTHROW( atm_sys.assignBasisFunctions( 2, basis_complex ));
    REQUIRE( atm_sys.systemComplete() );

    REQUIRE( atm_sys.at(0).size() == 3);
    REQUIRE( atm_sys.at(0).at(0)->getBasisFuncCount() == 1); // H 
    REQUIRE( atm_sys.at(0).at(1)->getBasisFuncCount() == 1); // H
    REQUIRE( atm_sys.at(0).at(2)->getBasisFuncCount() == 4); // C

    REQUIRE( atm_sys.at(1).size() == 2);
    REQUIRE( atm_sys.at(1).at(0)->getBasisFuncCount() == 5); // O
    REQUIRE( atm_sys.at(1).at(1)->getBasisFuncCount() == 5); // O

    REQUIRE( atm_sys.at(2).size() == 5);
    REQUIRE( atm_sys.at(2).at(0)->getBasisFuncCount() == 1); // H 
    REQUIRE( atm_sys.at(2).at(1)->getBasisFuncCount() == 1); // H
    REQUIRE( atm_sys.at(2).at(2)->getBasisFuncCount() == 4); // C
    REQUIRE( atm_sys.at(2).at(3)->getBasisFuncCount() == 5); // O
    REQUIRE( atm_sys.at(2).at(4)->getBasisFuncCount() == 5); // O
  }
}
