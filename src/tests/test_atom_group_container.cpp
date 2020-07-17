
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Local private includes
#include "atom_group_container.hpp"

#include "atom.hpp"
#include "atom_group.hpp"
#include "elements.hpp"

// Standard includes
#include <memory>

using namespace catnip;

TEST_CASE("Atom Group Container","[unit]") {

  Atom atom1(Element::H, 0.0, 0.0, 0.0);
  Atom atom2(Element::H, 2.0, 0.0, 0.0);
  Atom atom3(Element::C, 1.0, 1.0, 0.0);

  auto atom1_ptr = std::make_shared<Atom>(atom1);
  auto atom2_ptr = std::make_shared<Atom>(atom2);
  auto atom3_ptr = std::make_shared<Atom>(atom3);

  AtomGroup atom_grp1("CH2");
  atom_grp1.add(atom1_ptr);
  atom_grp1.add(atom2_ptr);
  atom_grp1.add(atom3_ptr);

  Atom atom4(Element::O, 3.3, 4.3, 0.0);
  Atom atom5(Element::O, 3.0, 3.9, 0.0);

  auto atom4_ptr = std::make_shared<Atom>(atom4);
  auto atom5_ptr = std::make_shared<Atom>(atom5);

  AtomGroup atom_grp2("O2");
  atom_grp2.add(atom4_ptr);  
  atom_grp2.add(atom5_ptr);  

  AtomGroupContainer atom_grp_cont;

  atom_grp_cont.add(atom_grp1);
  atom_grp_cont.add(atom_grp2);

  REQUIRE(atom_grp_cont.size() == 2);

  atom_grp_cont.assignGroupTypes();

  // Because it made a copy these should still be unassigned
  REQUIRE(atom_grp1.getType() == GroupType::Unassigned);
  REQUIRE(atom_grp2.getType() == GroupType::Unassigned);

  // These should be changed
  REQUIRE(atom_grp_cont.getType(0) == GroupType::Unit);
  REQUIRE(atom_grp_cont.getType(1) == GroupType::Unit);
  REQUIRE(atom_grp_cont.exists(GroupType::Unit));

  // Now we make a third group that has all the atoms of the first two groups
  AtomGroup atom_grp3("O2CH2");
  atom_grp3.add(atom1_ptr);
  atom_grp3.add(atom2_ptr);
  atom_grp3.add(atom3_ptr);
  atom_grp3.add(atom4_ptr);
  atom_grp3.add(atom5_ptr);

  atom_grp_cont.add(atom_grp3);

  REQUIRE(atom_grp_cont.getType(0) == GroupType::Unassigned);
  REQUIRE(atom_grp_cont.getType(1) == GroupType::Unassigned);
  REQUIRE(atom_grp_cont.getType(2) == GroupType::Unassigned);
  REQUIRE(atom_grp_cont.exists(GroupType::Unassigned));

  atom_grp_cont.assignGroupTypes();
  REQUIRE(atom_grp_cont.getType(0) == GroupType::Component);
  REQUIRE(atom_grp_cont.getType(1) == GroupType::Component);
  REQUIRE(atom_grp_cont.getType(2) == GroupType::Complex);

  REQUIRE(atom_grp_cont.exists(GroupType::Complex));
  REQUIRE(atom_grp_cont.exists(GroupType::Component));

}
