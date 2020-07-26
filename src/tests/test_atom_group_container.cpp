
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

  AtomGroup atom_grp("CH2");
  
  Atom atom1(Element::H, 0.0, 0.0, 0.0);
  Atom atom2(Element::H, 2.0, 0.0, 0.0);
  Atom atom3(Element::C, 1.0, 1.0, 0.0);

  auto atom1_ptr = std::make_shared<Atom>(atom1);
  auto atom2_ptr = std::make_shared<Atom>(atom2);
  auto atom3_ptr = std::make_shared<Atom>(atom3);

  atom_grp.add(atom1_ptr);
  atom_grp.add(atom2_ptr);
  atom_grp.add(atom3_ptr);

  AtomGroup atom_grp2("O2");

  Atom atom4(Element::O, 3.3, 4.3);
  Atom atom5(Element::O, 3.0, 3.9);

  auto atom4_ptr = std::make_shared<Atom>(atom4);
  auto atom5_ptr = std::make_shared<Atom>(atom5);

  atom_grp2.add(atom4_ptr);  
  atom_grp2.add(atom5_ptr);  

  AtomGroupContainer atom_grp_cont;

  atom_grp_cont.add(atom_grp1);
  atom_grp_cont.add(atom_grp2);

  REQUIRE(atom_grp_cont.size() == 2);

  atom_grp_cont.assignGroupTypes();
}
