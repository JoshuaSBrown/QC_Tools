
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Local private includes
#include "basis_map.hpp"

#include "atom.hpp"
#include "atom_group_container.hpp"
#include "atom_system.hpp"
#include "elements.hpp"

using namespace catnip;

TEST_CASE("Basis Map Unscrambled","[integration]") {

  Atom atom1(Element::H, 0.0, 0.0, 0.0);
  Atom atom2(Element::H, 2.0, 0.0, 0.0);
  Atom atom3(Element::C, 1.0, 1.0, 0.0);
  Atom atom4(Element::O, 3.3, 4.3, 0.0);
  Atom atom5(Element::O, 3.0, 3.9, 0.0);

  auto atom1_ptr = std::make_shared<Atom>(atom1);
  auto atom2_ptr = std::make_shared<Atom>(atom2);
  auto atom3_ptr = std::make_shared<Atom>(atom3);
  auto atom4_ptr = std::make_shared<Atom>(atom4);
  auto atom5_ptr = std::make_shared<Atom>(atom5);

  // Extra atoms are allowed in the complex but not in the componets
  auto component1 = std::make_unique<AtomGroup>("CH2");
  component1->add(atom1_ptr);
  component1->add(atom2_ptr);
  component1->add(atom3_ptr);

  auto component2 = std::make_unique<AtomGroup>("O2");
  component2->add(atom4_ptr);  
  component2->add(atom5_ptr);  

  auto complex1 = std::make_unique<AtomGroup>("O2CH2");
  complex1->add(atom1_ptr);
  complex1->add(atom2_ptr);
  complex1->add(atom3_ptr);
  complex1->add(atom4_ptr);
  complex1->add(atom5_ptr);

  auto atom_grp_cont = std::unique_ptr<AtomGroupContainer>(new AtomGroupContainer);
  atom_grp_cont->add(std::move(component1));
  atom_grp_cont->add(std::move(component2));
  atom_grp_cont->add(std::move(complex1));

  auto atm_sys = AtomSystem(std::move(atom_grp_cont));

  std::vector<int> basis_complex{ 1, 1, 4, 5, 5}; 

  int index_complex = 2;
  atm_sys.assignBasisFunctions( index_complex, basis_complex);
  BasisMap basis_map(std::move(atm_sys));

  // Equal to the total number of basis in the group
  REQUIRE( basis_map.row_col_current[0].size() == 6);
  // comp1 atm 1 should map to location 0 in the complex at least currentially
  REQUIRE( basis_map.row_col_current[0].at(0) == 0);
  // comp1 atm 2 should map to location 1 in the complex at least currentially
  REQUIRE( basis_map.row_col_current[0].at(1) == 1);
  // comp1 atm 3 should map to location 2-5 in the complex at least currentially
  REQUIRE( basis_map.row_col_current[0].at(2) == 2);
  REQUIRE( basis_map.row_col_current[0].at(3) == 3);
  REQUIRE( basis_map.row_col_current[0].at(4) == 4);
  REQUIRE( basis_map.row_col_current[0].at(5) == 5);
  
  REQUIRE( basis_map.row_col_current[1].size() == 10);
  // comp2 atm 1 should map to location 4 in the complex at least currentially
  REQUIRE( basis_map.row_col_current[1].at(0) == 6);
  // comp2 atm 2 should map to location 5 in the complex at least currentially
  REQUIRE( basis_map.row_col_current[1].at(1) == 7);
  REQUIRE( basis_map.row_col_current[1].at(2) == 8);
  REQUIRE( basis_map.row_col_current[1].at(3) == 9);
  REQUIRE( basis_map.row_col_current[1].at(4) == 10);
  REQUIRE( basis_map.row_col_current[1].at(5) == 11);
  REQUIRE( basis_map.row_col_current[1].at(6) == 12);
  REQUIRE( basis_map.row_col_current[1].at(7) == 13);
  REQUIRE( basis_map.row_col_current[1].at(8) == 14);
  REQUIRE( basis_map.row_col_current[1].at(9) == 15);

  // In this case the final and currential version should be the same
  // Equal to the total number of basis in the group
  REQUIRE( basis_map.row_col_final[0].size() == 6);
  // comp1 atm 1 should map to location 0 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[0].at(0) == 0);
  // comp1 atm 2 should map to location 1 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[0].at(1) == 1);
  // comp1 atm 3 should map to location 2-5 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[0].at(2) == 2);
  REQUIRE( basis_map.row_col_final[0].at(3) == 3);
  REQUIRE( basis_map.row_col_final[0].at(4) == 4);
  REQUIRE( basis_map.row_col_final[0].at(5) == 5);
  
  REQUIRE( basis_map.row_col_final[1].size() == 10);
  // comp2 atm 1 should map to location 4 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[1].at(0) == 6);
  // comp2 atm 2 should map to location 5 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[1].at(1) == 7);
  REQUIRE( basis_map.row_col_final[1].at(2) == 8);
  REQUIRE( basis_map.row_col_final[1].at(3) == 9);
  REQUIRE( basis_map.row_col_final[1].at(4) == 10);
  REQUIRE( basis_map.row_col_final[1].at(5) == 11);
  REQUIRE( basis_map.row_col_final[1].at(6) == 12);
  REQUIRE( basis_map.row_col_final[1].at(7) == 13);
  REQUIRE( basis_map.row_col_final[1].at(8) == 14);
  REQUIRE( basis_map.row_col_final[1].at(9) == 15);

  std::pair<int,int> location = basis_map.findLocation(0);
  REQUIRE( location.first == 0 );
  REQUIRE( location.second == 0 );
  location = basis_map.findLocation(1);
  REQUIRE( location.first == 0 );
  REQUIRE( location.second == 1 );
  location = basis_map.findLocation(2);
  REQUIRE( location.first == 0 );
  REQUIRE( location.second == 2 );

  location = basis_map.findLocation(6);
  REQUIRE( location.first == 1 );
  REQUIRE( location.second == 0 );
  location = basis_map.findLocation(7);
  REQUIRE( location.first == 1 );
  REQUIRE( location.second == 1 );
  location = basis_map.findLocation(15);
  REQUIRE( location.first == 1 );
  REQUIRE( location.second == 9 );
}


/**
 * @brief In this test some of the atoms added to the complex will be added
 * in a different order from how they were added to each of the components
 *
 * @param Scrambled"
 * @param "[integration]"
 */
TEST_CASE("Basis Map Scrambled","[integration]") {

  std::vector<int> basis_comp1{ 1, 1, 4};
  std::vector<int> basis_comp2{ 5, 5}; 

  Atom atom1(Element::H, 0.0, 0.0, 0.0);
  Atom atom2(Element::H, 2.0, 0.0, 0.0);
  Atom atom3(Element::C, 1.0, 1.0, 0.0);
  Atom atom4(Element::O, 3.3, 4.3, 0.0);
  Atom atom5(Element::O, 3.0, 3.9, 0.0);

  auto atom1_ptr = std::make_shared<Atom>(atom1);
  auto atom2_ptr = std::make_shared<Atom>(atom2);
  auto atom3_ptr = std::make_shared<Atom>(atom3);
  auto atom4_ptr = std::make_shared<Atom>(atom4);
  auto atom5_ptr = std::make_shared<Atom>(atom5);

  // Extra atoms are allowed in the complex but not in the componets
  auto component1 = std::make_unique<AtomGroup>("CH2");
  component1->add(atom1_ptr);
  component1->add(atom2_ptr);
  component1->add(atom3_ptr);

  auto component2 = std::make_unique<AtomGroup>("O2");
  component2->add(atom4_ptr);  
  component2->add(atom5_ptr);  

  // HERE: the order has been changed
  auto complex1 = std::make_unique<AtomGroup>("O2CH2");
  complex1->add(atom4_ptr); // The basis functions of atom 4 : 0 - 4
  complex1->add(atom1_ptr); // The basis functiosn of atom 1 : 5 - 5 
  complex1->add(atom3_ptr); // The basis functiosn of atom 3 : 6 - 9
  complex1->add(atom5_ptr); // The basis functiosn of atom 5 : 10 - 14
  complex1->add(atom2_ptr); // The basis functions of atom 2 : 15 - 15

  auto atom_grp_cont = std::unique_ptr<AtomGroupContainer>(new AtomGroupContainer);
  atom_grp_cont->add(std::move(component1));
  atom_grp_cont->add(std::move(component2));
  atom_grp_cont->add(std::move(complex1));

  AtomSystem atm_sys(std::move(atom_grp_cont));

  int index_component1 = 0;
  int index_component2 = 1;
  atm_sys.assignBasisFunctions( index_component1, basis_comp1);
  atm_sys.assignBasisFunctions( index_component2, basis_comp2);

  BasisMap basis_map(atm_sys);

  // The row_col_current will now be different but the row_col_final should remail
  // the same

  // The basis functions of atom 4 : 0 - 4
  // The basis functiosn of atom 1 : 5 - 5 
  // The basis functiosn of atom 3 : 6 - 9
  // The basis functiosn of atom 5 : 10 - 14
  // The basis functions of atom 2 : 15 - 15
  
  // Equal to the total number of basis in the group
  REQUIRE( basis_map.row_col_current[0].size() == 6);
  REQUIRE( basis_map.row_col_current[0].at(0) == 5);  // atom 1
  REQUIRE( basis_map.row_col_current[0].at(1) == 15); // atom 2
  REQUIRE( basis_map.row_col_current[0].at(2) == 6);  // atom 3
  REQUIRE( basis_map.row_col_current[0].at(3) == 7);
  REQUIRE( basis_map.row_col_current[0].at(4) == 8);
  REQUIRE( basis_map.row_col_current[0].at(5) == 9);
  
  REQUIRE( basis_map.row_col_current[1].size() == 10);
  REQUIRE( basis_map.row_col_current[1].at(0) == 0); // atom 4
  REQUIRE( basis_map.row_col_current[1].at(1) == 1);
  REQUIRE( basis_map.row_col_current[1].at(2) == 2);
  REQUIRE( basis_map.row_col_current[1].at(3) == 3);
  REQUIRE( basis_map.row_col_current[1].at(4) == 4);
  REQUIRE( basis_map.row_col_current[1].at(5) == 10); // atom 5
  REQUIRE( basis_map.row_col_current[1].at(6) == 11);
  REQUIRE( basis_map.row_col_current[1].at(7) == 12);
  REQUIRE( basis_map.row_col_current[1].at(8) == 13);
  REQUIRE( basis_map.row_col_current[1].at(9) == 14);

  // In this case the final and currential version should be the same
  // Equal to the total number of basis in the group
  REQUIRE( basis_map.row_col_final[0].size() == 6);
  // comp1 atm 1 should map to location 0 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[0].at(0) == 0);
  // comp1 atm 2 should map to location 1 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[0].at(1) == 1);
  // comp1 atm 3 should map to location 2-5 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[0].at(2) == 2);
  REQUIRE( basis_map.row_col_final[0].at(3) == 3);
  REQUIRE( basis_map.row_col_final[0].at(4) == 4);
  REQUIRE( basis_map.row_col_final[0].at(5) == 5);
  
  REQUIRE( basis_map.row_col_final[1].size() == 10);
  // comp2 atm 1 should map to location 4 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[1].at(0) == 6);
  // comp2 atm 2 should map to location 5 in the complex at least finalially
  REQUIRE( basis_map.row_col_final[1].at(1) == 7);
  REQUIRE( basis_map.row_col_final[1].at(2) == 8);
  REQUIRE( basis_map.row_col_final[1].at(3) == 9);
  REQUIRE( basis_map.row_col_final[1].at(4) == 10);
  REQUIRE( basis_map.row_col_final[1].at(5) == 11);
  REQUIRE( basis_map.row_col_final[1].at(6) == 12);
  REQUIRE( basis_map.row_col_final[1].at(7) == 13);
  REQUIRE( basis_map.row_col_final[1].at(8) == 14);
  REQUIRE( basis_map.row_col_final[1].at(9) == 15);

  // Look for where in the currential rows the row of the complex can be found
  std::pair<int,int> location = basis_map.findLocation(0);
  REQUIRE( location.first == 1 );
  REQUIRE( location.second == 0 );
  location = basis_map.findLocation(1);
  REQUIRE( location.first == 1 );
  REQUIRE( location.second == 1 );
  location = basis_map.findLocation(2);
  REQUIRE( location.first == 1 );
  REQUIRE( location.second == 2 );

  location = basis_map.findLocation(6);
  REQUIRE( location.first == 0 );
  REQUIRE( location.second == 2 );
  location = basis_map.findLocation(7);
  REQUIRE( location.first == 0 );
  REQUIRE( location.second == 3 );
  location = basis_map.findLocation(15);
  REQUIRE( location.first == 0 );
  REQUIRE( location.second == 1 );
}
