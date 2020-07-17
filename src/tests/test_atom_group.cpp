
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Local private includes
#include "atom_group.hpp"

#include "atom.hpp"
#include "elements.hpp"

// Standard includes
#include <memory>

using namespace catnip;

TEST_CASE("Atom Group","[unit]") {

  AtomGroup atom_grp("CH2");
  
  REQUIRE(atom_grp.getType() == GroupType::Unknown );
  REQUIRE(atom_grp.getName() == "CH2");

  Atom atom1(Element::H, 0.0, 0.0, 0.0);
  Atom atom2(Element::H, 2.0, 0.0, 0.0);
  Atom atom3(Element::C, 1.0, 1.0, 0.0);
  // Same atom different memory location
  Atom atom4(Element::H, 0.0, 0.0, 0.0);

  auto atom1_ptr = std::make_shared<Atom>(atom1);
  auto atom2_ptr = std::make_shared<Atom>(atom2);
  auto atom3_ptr = std::make_shared<Atom>(atom3);
  auto atom4_ptr = std::make_shared<Atom>(atom4);

  atom_grp.add(atom1_ptr);
  atom_grp.add(atom2_ptr);
  atom_grp.add(atom3_ptr);

  REQUIRE(atom_grp.size() == 3); 
 
  REQUIRE(atom_grp.find(atom1_ptr).size() == 1);  
  REQUIRE(atom_grp.find(atom2_ptr).size() == 1);  
  REQUIRE(atom_grp.find(atom3_ptr).size() == 1);  

  REQUIRE(atom_grp.find(atom1_ptr).at(0) == 0);  
  REQUIRE(atom_grp.find(atom2_ptr).at(0) == 1);  
  REQUIRE(atom_grp.find(atom3_ptr).at(0) == 2);  

  atom_grp.add(atom4_ptr);
  REQUIRE(atom_grp.find(atom1_ptr).size() == 2);  
  REQUIRE(atom_grp.find(atom1_ptr).at(0) == 0);  
  REQUIRE(atom_grp.find(atom1_ptr).at(1) == 3);  

  REQUIRE(atom_grp.findStrict(atom4_ptr).size() == 1);
  REQUIRE(atom_grp.findStrict(atom4_ptr).at(0) == 3);  

}
