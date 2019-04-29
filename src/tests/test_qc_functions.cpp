#include "../libcatnip/qc_functions.hpp"
#include "../libcatnip/matrix.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
using namespace catnip;
using namespace std;

int main(void) {

  cout << "Testing: unscramble_Coef" << endl;
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

    // Contains the x y and z position of atoms in monomer A
    Eigen::MatrixXd coordA(2, 3);
    // Atom 1 at (1.0,1.0,1.0)
    // Atom 2 at (0.0,0.0,0.0)
    coordA << 1.0, 1.0, 1.0,
              0.0, 0.0, 0.0;

    // Contains the x y and z position of atoms in monomer B
    Eigen::MatrixXd coordB(3, 3);
    // Atom 3 at (1.0,0.0,0.0)
    // Atom 4 at (2.0,0.0,0.0)
    // Atom 5 at (3.0,0.0,0.0)
    coordB << 1.0, 0.0, 0.0,
              2.0, 0.0, 0.0,
              3.0, 0.0, 0.0;

    Eigen::MatrixXd coordDimer(5, 3);
    // Arrange atoms in the dimer so they do not appear in the
    // same order as the monomers

    // Atom 3   1.0, 0.0, 0.0
    // Atom 1   1.0, 1.0, 1.0
    // Atom 5   3.0, 0.0, 0.0
    // Atom 4   2.0, 0.0, 0.0
    // Atom 2   0.0, 0.0, 0.0

    coordDimer << 1.0, 0.0, 0.0,
                  1.0, 1.0, 1.0,
                  3.0, 0.0, 0.0,
                  2.0, 0.0, 0.0,
                  0.0, 0.0, 0.0;

    vector<int> matchA = matchRow(coordA,coordDimer, 2);
    vector<int> matchB = matchRow(coordB,coordDimer, 2);

    assert(matchA.at(0) == 1);
    assert(matchA.at(1) == 4);

    assert(matchB.at(0) == 0);
    assert(matchB.at(1) == 4);
    assert(matchB.at(2) == 2);

    vector<int> basisFuncDimer;
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

    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);

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

    cerr << "Before passing in" << endl;
    for (auto it : matchA) cerr << it << endl;
    for (auto it : matchB) cerr << it << endl;
    for (auto it : basisFuncDimer) cerr << it << endl;
    // If we correct the dimer coefficient matrix to line up with the
    // coefficients of the monomers it should look like this
    //
    //        Col 0   Col 1   Col 2   Col 3   Col 4   Col 5   Col 6   Col 7
    // Row 0   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 1   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 2   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 3   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 4   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 5   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 6   1.3     9.4     3.4     82.4    0       0.2     0       -3
    // Row 7   1.3     9.4     3.4     82.4    0       0.2     0       -3

    cerr << "Calling unscramble" << endl;
    Eigen::MatrixXd NewCoef = unscramble_Coef(matchB, matchA, basisFuncDimer, dimerCoef);

    cerr << NewCoef << endl;

    for (int i = 0; i < 8; ++i) {
      assert(static_cast<int>(NewCoef(i, 0) * 10) == 13);
      assert(static_cast<int>(NewCoef(i, 1) * 10) == 94);
      assert(static_cast<int>(NewCoef(i, 2) * 10) == 34);
      assert(static_cast<int>(NewCoef(i, 3) * 10) == 824);
      assert(static_cast<int>(NewCoef(i, 4)) == 0);
      assert(static_cast<int>(NewCoef(i, 5) * 10) == 2);
      assert(static_cast<int>(NewCoef(i, 6)) == 0);
      assert(static_cast<int>(NewCoef(i, 7)) == -3);
    }
  }

  cout << "Testing: unscramble_S_Coef" << endl;
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
    Eigen::MatrixXd coordA(2, 3);
    // Atom 1 at (1.0,1.0,1.0)
    // Atom 2 at (0.0,0.0,0.0)
    coordA << 1.0, 1.0, 1.0,
              0.0, 0.0, 0.0;

    // Contains the x y and z position of atoms in monomer B
    Eigen::MatrixXd coordB(3, 3);
    // Atom 3 at (1.0,0.0,0.0)
    // Atom 4 at (2.0,0.0,0.0)
    // Atom 5 at (3.0,0.0,0.0)
    coordB << 1.0, 0.0, 0.0,
              2.0, 0.0, 0.0,
              3.0, 0.0, 0.0;

    Eigen::MatrixXd coordDimer = Eigen::MatrixXd::Zero(5, 3);
    // Arrange atoms in the dimer so they do not appear in the
    // same order as the monomers

    // row1 Atom 3
    coordDimer(0, 0) = 1.0;
    // row2 Atom 1
    coordDimer(1, 0) = 1.0;
    coordDimer(1, 1) = 1.0;
    coordDimer(1, 2) = 1.0;
    // row3 Atom 5
    coordDimer(2, 0) = 3.0;
    // row4 Atom 4
    coordDimer(3, 0) = 2.0;
    // row5 Atom 2 (0.0,0.0,0.0)

    vector<int> matchA = matchRow(coordA,coordDimer, 2);
    vector<int> matchB = matchRow(coordB,coordDimer, 2);

    assert(matchA.at(0) == 1);
    assert(matchA.at(1) == 4);

    assert(matchB.at(0) == 0);
    assert(matchB.at(1) == 3);
    assert(matchB.at(2) == 2);

    vector<int> basisFuncDimer;
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

    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);

    cerr << "Before passing in" << endl;
    for (auto it : matchA) cerr << it << endl;
    for (auto it : matchB) cerr << it << endl;
    for (auto it : basisFuncDimer) cerr << it << endl;
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
    cerr << "Calling unscramble" << endl;
    Eigen::MatrixXd NewCoef = unscramble_S(matchB, matchA, basisFuncDimer, SCoef);

    cerr << NewCoef << endl;

    for (int i = 0; i < 8; ++i) {
      if (i != 3) {
        assert(static_cast<int>(NewCoef(i, 4) * 10) == 13);
        assert(static_cast<int>(NewCoef(i, 5) * 10) == 13);

        assert(static_cast<int>(NewCoef(4, i) * 10) == 13);
        assert(static_cast<int>(NewCoef(5, i) * 10) == 13);
      }
    }

    for (int i = 0; i < 8; ++i) {
      if (i == 4 || i == 5) {
        assert(static_cast<int>(NewCoef(i, 3)) == 6);
        assert(static_cast<int>(NewCoef(3, i)) == 6);
      } else {
        assert(static_cast<int>(NewCoef(3, i)) == 4);
        assert(static_cast<int>(NewCoef(i, 3)) == 4);
      }
    }
  }
  return 0;
}
