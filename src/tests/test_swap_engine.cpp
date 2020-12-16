
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Local private includes
#include "swap_engine.hpp"

#include "atom.hpp"
#include "atom_group_container.hpp"
#include "atom_system.hpp"
#include "basis_map.hpp"
#include "elements.hpp"

// Third party includes
#include <eigen3/Eigen/Dense>

// Standard includes
#include <iostream>
#include <vector>

using namespace catnip;

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
  complex1->add(atom1_ptr); // The basis functions of atom 1 : 5 - 5 
  complex1->add(atom3_ptr); // The basis functions of atom 3 : 6 - 9
  complex1->add(atom5_ptr); // The basis functions of atom 5 : 10 - 14
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

}

TEST_CASE("QC Functions","[integration]") {

  std::cout << "Testing: unscramble_Coef" << std::endl;
  {
    Eigen::MatrixXd dimerCoef = Eigen::MatrixXd::Zero(8, 8);
    for (int i = 0; i < 8; ++i) {
      dimerCoef(i, 0) = 1.3;
      dimerCoef(i, 2) = 0.2;
      dimerCoef(i, 3) = 82.4;
      dimerCoef(i, 4) = 9.4;
      dimerCoef(i, 5) = 3.4;
      dimerCoef(i, 7) = -3.0;
    }

    // Our coefficient matrix should look like this
    //
    //         col 0  col 1  col 2  col 3  col 4  col 5  col 6  col 7
    //        _______________________________________________________
    // row 0 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 1 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 2 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 3 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 4 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 5 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 6 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 7 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    //         atm3   atm1   atm1   atm5   atm4   atm4   atm2   atm2

    Atom atom1(Element::H, 1.0, 1.0, 1.0);
    Atom atom2(Element::H, 0.0, 0.0, 0.0);

    auto atom1_ptr = std::make_shared<Atom>(atom1);
    auto atom2_ptr = std::make_shared<Atom>(atom2);

    auto componentA = std::make_unique<AtomGroup>("A");
    componentA->add(atom1_ptr);
    componentA->add(atom2_ptr);

    Atom atom3(Element::C, 1.0, 0.0, 0.0);
    Atom atom4(Element::O, 2.0, 0.0, 0.0);
    Atom atom5(Element::O, 3.0, 0.0, 0.0);

    auto atom3_ptr = std::make_shared<Atom>(atom3);
    auto atom4_ptr = std::make_shared<Atom>(atom4);
    auto atom5_ptr = std::make_shared<Atom>(atom5);

    auto componentB = std::make_unique<AtomGroup>("B");
    componentB->add(atom3_ptr);
    componentB->add(atom4_ptr);
    componentB->add(atom5_ptr);

    // Arrange atoms in the dimer so they do not appear in the
    // same order as the monomers

    // Atom 3   1->0, 0->0, 0->0
    // Atom 1   1->0, 1->0, 1->0
    // Atom 5   3->0, 0->0, 0->0
    // Atom 4   2->0, 0->0, 0->0
    // Atom 2   0->0, 0->0, 0->0

    auto complexAB = std::make_unique<AtomGroup>("AB");
    complexAB->add(atom3_ptr);
    complexAB->add(atom1_ptr);
    complexAB->add(atom5_ptr);
    complexAB->add(atom4_ptr);
    complexAB->add(atom2_ptr);

    // Combine atom groups
    auto atom_grp_cont = std::unique_ptr<AtomGroupContainer>(new AtomGroupContainer);
    atom_grp_cont->add(std::move(componentA));
    atom_grp_cont->add(std::move(componentB));
    atom_grp_cont->add(std::move(complexAB));

    // Basis functions per atom
    //
    // Atom 1 2
    // Atom 2 2
    // Atom 3 1
    // Atom 4 2
    // Atom 5 1

    // According to the dimer coordinates they will appear in the following
    // order
    // Atom 3 1
    // Atom 1 2
    // Atom 5 1
    // Atom 4 2
    // Atom 2 2

    std::vector<int> basisFuncDimer;
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);

    AtomSystem atm_sys(std::move(atom_grp_cont));

    // Assign the basis functions to the atoms that are in the complex
    int index_complex = 2;
    atm_sys.assignBasisFunctions( index_complex, basisFuncDimer);
    BasisMap basis_map(atm_sys);

    // According to the basis functions and the atom positions the current
    // coef table for monomer A should look like this
    //         col 0  col 1  col 2  col 3
    //        ____________________________
    // row 0 |  0.0    0.2    0.0   -3.0
    // row 1 |  0.0    0.2    0.0   -3.0
    // row 2 |  0.0    0.2    0.0   -3.0
    // row 3 |  0.0    0.2    0.0   -3.0
    // row 4 |  0.0    0.2    0.0   -3.0
    // row 5 |  0.0    0.2    0.0   -3.0
    // row 6 |  0.0    0.2    0.0   -3.0
    // row 7 |  0.0    0.2    0.0   -3.0
    //         atm1   atm1   atm2   atm2

    auto swap_eng = SwapEngine(basis_map);

    std::cout << "Before arranging dimer coef" << std::endl;
    std::cout << dimerCoef << std::endl;

    swap_eng.arrangeCols(dimerCoef);

    std::cout << "Expected output" << std::endl;
    std::cout << "       Col 1   Col 1   Col 2   Col 3   Col 4   Col 5   Col 6   Col 7 " << std::endl;
    std::cout << "Row 0   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl; 
    std::cout << "Row 1   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;
    std::cout << "Row 2   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;
    std::cout << "Row 3   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;
    std::cout << "Row 4   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;
    std::cout << "Row 5   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;
    std::cout << "Row 6   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;
    std::cout << "Row 7   0.0     0.2     0.0     -3.0    1.3     9.4     3.4     82.4" << std::endl;

    std::cout << "After arranging dimer coef" << std::endl;
    std::cout << dimerCoef << std::endl;

    for (int i = 0; i < 8; ++i) {
      REQUIRE(dimerCoef(i, 0) == Approx(0.0));
      REQUIRE(dimerCoef(i, 1) == Approx(0.2));
      REQUIRE(dimerCoef(i, 2) == Approx(0.0));
      REQUIRE(dimerCoef(i, 3) == Approx(-3.0));
      REQUIRE(dimerCoef(i, 4) == Approx(1.3));
      REQUIRE(dimerCoef(i, 5) == Approx(9.4));
      REQUIRE(dimerCoef(i, 6) == Approx(3.4));
      REQUIRE(dimerCoef(i, 7) == Approx(82.4));
    }
  }

  std::cout << "Testing: unscramble SCoef" << std::endl;
  {
    Eigen::MatrixXd SCoef = Eigen::MatrixXd::Zero(8, 8);
    for (int i = 0; i < 8; ++i) {
      SCoef(i, 1) = 1.3;
      SCoef(i, 2) = 1.3;
      SCoef(i, 3) = 4.0;
    }

    for (int i = 0; i < 8; ++i) {
      SCoef(1, i) = 1.3;
      SCoef(2, i) = 1.3;
      SCoef(3, i) = 4.0;
    }

    SCoef(1, 3) = 6.0;
    SCoef(2, 3) = 6.0;
    SCoef(3, 1) = 6.0;
    SCoef(3, 2) = 6.0;

    // Our coefficient matrix should look like this
    //
    //               col 0  col 1  col 2  col 3  col 4  col 5  col 6  col 7
    //              _______________________________________________________
    // atm 3 row 0 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 1 row 1 |  1.3    1.3    1.3    6.0    1.3    1.3    1.3    1.3
    // atm 1 row 2 |  1.3    1.3    1.3    6.0    1.3    1.3    1.3    1.3
    // atm 5 row 3 |  4.0    6.0    6.0    4.0    4.0    4.0    4.0    4.0
    // atm 4 row 4 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 4 row 5 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 2 row 6 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 2 row 7 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    //                atm3   atm1   atm1   atm5   atm4   atm4   atm2   atm2

    // Contains the x y and z position of atoms in monomer A
    Atom atom1(Element::H, 1.0, 1.0, 1.0);
    Atom atom2(Element::H, 0.0, 0.0, 0.0);

    auto atom1_ptr = std::make_shared<Atom>(atom1);
    auto atom2_ptr = std::make_shared<Atom>(atom2);

    auto componentB = std::make_unique<AtomGroup>("B");
    componentB->add(atom1_ptr);
    componentB->add(atom2_ptr);

    // Contains the x y and z position of atoms in monomer B
    Atom atom3(Element::C, 1.0, 0.0, 0.0);
    Atom atom4(Element::O, 2.0, 0.0, 0.0);
    Atom atom5(Element::O, 3.0, 0.0, 0.0);

    auto atom3_ptr = std::make_shared<Atom>(atom3);
    auto atom4_ptr = std::make_shared<Atom>(atom4);
    auto atom5_ptr = std::make_shared<Atom>(atom5);

    auto componentA = std::make_unique<AtomGroup>("A");
    componentA->add(atom3_ptr);
    componentA->add(atom4_ptr);
    componentA->add(atom5_ptr);

    // Positions of atoms in the complex
    auto complexAB = std::make_unique<AtomGroup>("AB");
    complexAB->add(atom3_ptr);
    complexAB->add(atom1_ptr);
    complexAB->add(atom5_ptr);
    complexAB->add(atom4_ptr);
    complexAB->add(atom2_ptr);

    // Combine atom groups
    auto atom_grp_cont = std::unique_ptr<AtomGroupContainer>(new AtomGroupContainer);
    atom_grp_cont->add(std::move(componentA));
    atom_grp_cont->add(std::move(componentB));
    atom_grp_cont->add(std::move(complexAB));

    AtomSystem atm_sys(std::move(atom_grp_cont));
    
    // Basis functions per atom
    //
    // Atom 1 2
    // Atom 2 2
    // Atom 3 1
    // Atom 4 2
    // Atom 5 1

    // According to the dimer coordinates they will appear in the following
    // order
    // Atom 3 1
    // Atom 1 2
    // Atom 5 1
    // Atom 4 2
    // Atom 2 2

    std::vector<int> basisFuncDimer;
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);

    // Assign the basis functions to the atoms that are in the complex
    int index_complex = 2;
    atm_sys.assignBasisFunctions( index_complex, basisFuncDimer);
    BasisMap basis_map(atm_sys);

    // If we correct the dimer coefficient matrix to line up with the
    // coefficients of the monomers it should look like this
    //
    //        Col 0   Col 1   Col 2   Col 3   Col 4   Col 5   Col 6   Col 7
    // Row 0   0       0       0       4       1.3     1.3     0       0
    // Row 1   0       0       0       4       1.3     1.3     0       0
    // Row 2   0       0       0       4       1.3     1.3     0       0
    // Row 3   4       4       4       4       6       6       4       4
    // Row 4   1.3     1.3     1.3     6       1.3     1.3     1.3     1.3
    // Row 5   1.3     1.3     1.3     6       1.3     1.3     1.3     1.3
    // Row 6   0       0       0       4       1.3     1.3     0       0
    // Row 7   0       0       0       4       1.3     1.3     0       0
    //        atm3    atm4    atm3    atm5    atm1    atm1    atm2    atm2

    auto swap_eng = SwapEngine(basis_map);

    std::cout << "Before arranging S" << std::endl;
    std::cout << SCoef << std::endl;

    swap_eng.arrange(SCoef);

    std::cout << "Expected output" << std::endl;
    std::cout << "       Col 1   Col 1   Col 2   Col 3   Col 4   Col 5   Col 6   Col 7 " << std::endl;
    std::cout << "Row 0    0       0       0       4       1.3     1.3     0       0  " << std::endl; 
    std::cout << "Row 1    0       0       0       4       1.3     1.3     0       0  " << std::endl;
    std::cout << "Row 2    0       0       0       4       1.3     1.3     0       0  " << std::endl;
    std::cout << "Row 3    4       4       4       4       6       6       4       4  " << std::endl;
    std::cout << "Row 4    1.3     1.3     1.3     6       1.3     1.3     1.3     1.3" << std::endl;
    std::cout << "Row 5    1.3     1.3     1.3     6       1.3     1.3     1.3     1.3" << std::endl;
    std::cout << "Row 6    0       0       0       4       1.3     1.3     0       0  " << std::endl;
    std::cout << "Row 7    0       0       0       4       1.3     1.3     0       0  " << std::endl;

    std::cout << "After arranging dimer coef" << std::endl;
    std::cout << SCoef << std::endl;

    for (int i = 0; i < 8; ++i) {
      if (i != 3) {
        REQUIRE(SCoef(i, 4) == Approx(1.3));
        REQUIRE(SCoef(i, 5) == Approx(1.3));

        REQUIRE(SCoef(4, i) == Approx(1.3));
        REQUIRE(SCoef(5, i) == Approx(1.3));
      }
    }

    for (int i = 0; i < 8; ++i) {
      if (i == 4 || i == 5) {
        REQUIRE(SCoef(i, 3) == Approx(6));
        REQUIRE(SCoef(3, i) == Approx(6));
      } else {
        REQUIRE(SCoef(3, i) == Approx(4));
        REQUIRE(SCoef(i, 3) == Approx(4));
      }
    }
  }
}
