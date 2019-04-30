

#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../libcatnip/io/file_readers/logreader.hpp"
#include "../libcatnip/io/io.hpp"

using namespace catnip;
using namespace std;

int main(void) {

  cout << "Testing: LogReader Constructor" << endl;
  { LogReader lr("file.log"); }

  cout << "Testing: LogReader read energies" << endl;
  {

    string test_file = "testfile_energies.log";
    {
      ofstream fid;
      fid.open(test_file);
      fid << "Test log file contains sections that the log reader";
      fid << " will search and read from." << endl;
      fid << " Alpha  occ. eigenvalues --  -10.19871 -10.19791  -0.76206  -0.58448  -0.47269" << endl; 
      fid << " Alpha  occ. eigenvalues --   -0.42268  -0.36322  -0.27597" << endl; 
      fid << " Alpha virt. eigenvalues --   -0.00988   0.02500   0.03257   0.04671   0.06946" << endl;
      fid << " Alpha virt. eigenvalues --    0.08668   0.08746   0.12753   0.15670   0.17639" << endl; 
      fid.close();
    }

    LogReader lr(test_file);
    lr.read();

    Eigen::VectorXd orbital_energies = lr.getOE("Alpha");
    
    set<double> correct_orbital_energies = { 
    -10.19871, -10.19791,  -0.76206, -0.58448, -0.47269, 
     -0.42268,  -0.36322,  -0.27597, 
     -0.00988,   0.02500,   0.03257,   0.04671,  0.06946,
      0.08668,   0.08746,   0.12753,   0.15670,  0.17639};

    vector<bool> found_energy(correct_orbital_energies.size(),false); 
    for( int index = 0; index < orbital_energies.size();++index){
      if( correct_orbital_energies.count(orbital_energies(index))){
        found_energy.at(index) = true;
      }
      cout << "Found Energy " << orbital_energies(index);
      cout << " " << found_energy.at(index) << endl;
    }

    for ( const bool & found : found_energy){
      assert(found);
    }
  }

  cout << "Testing: LogReader read coordinates" << endl;
  {
    string test_file = "testfile_coords.log";
    {
      ofstream fid;
      fid.open(test_file);
      fid << "Center     Atomic      Atomic" << endl;
      fid << " Number     Number      Type              X           Y           Z" << endl;
      fid << " ---------------------------------------------------------------------" << endl;
      fid << "    1          6             0        0.672749    0.000000    5.000000" << endl;
      fid << "    2          6             0       -0.672749    0.000000    5.000000" << endl;
      fid << "    3          1             0        1.242623    0.162327    5.920604" << endl;
      fid << "    4          1             0        1.242623   -0.162327    4.079396" << endl;
      fid << "    5          1             0       -1.242623    0.162327    5.920604" << endl;
      fid << "    6          1             0       -1.242623   -0.162327    4.079396" << endl;
      fid << "-----------------------------------";
      fid << "----------------------------------" << endl;
      fid.close();
    }

    LogReader lr(test_file);
    lr.read();

    vector<vector<double>> atomic_coords = lr.getCoords();
 
    cout << atomic_coords.at(0).at(0) << " ";
    cout << atomic_coords.at(1).at(0) << " ";
    cout << atomic_coords.at(2).at(0) << " ";
    assert(atomic_coords.at(0).at(0) == 0.672749); 
    assert(atomic_coords.at(1).at(0) == 0.000000); 
    assert(atomic_coords.at(2).at(0) == 5.000000);
    cout << "Atom 1 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(1) << " ";
    cout << atomic_coords.at(1).at(1) << " ";
    cout << atomic_coords.at(2).at(1) << " ";
    assert(atomic_coords.at(0).at(1) == -0.672749); 
    assert(atomic_coords.at(1).at(1) == 0.0000000); 
    assert(atomic_coords.at(2).at(1) == 5.0000000); 
    cout << "Atom 2 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(2) << " ";
    cout << atomic_coords.at(1).at(2) << " ";
    cout << atomic_coords.at(2).at(2) << " ";
    assert(atomic_coords.at(0).at(2) == 1.242623); 
    assert(atomic_coords.at(1).at(2) == 0.162327); 
    assert(atomic_coords.at(2).at(2) == 5.920604); 
    cout << "Atom 3 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(3) << " ";
    cout << atomic_coords.at(1).at(3) << " ";
    cout << atomic_coords.at(2).at(3) << " ";
    assert(atomic_coords.at(0).at(3) == 1.242623); 
    assert(atomic_coords.at(1).at(3) == -0.162327); 
    assert(atomic_coords.at(2).at(3) == 4.079396); 
    cout << "Atom 4 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(4) << " ";
    cout << atomic_coords.at(1).at(4) << " ";
    cout << atomic_coords.at(2).at(4) << " ";
    assert(atomic_coords.at(0).at(4) == -1.242623); 
    assert(atomic_coords.at(1).at(4) == 0.162327); 
    assert(atomic_coords.at(2).at(4) == 5.920604); 
    cout << "Atom 5 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(5) << " ";
    cout << atomic_coords.at(1).at(5) << " ";
    cout << atomic_coords.at(2).at(5) << " ";
    assert(atomic_coords.at(0).at(5) == -1.242623); 
    assert(atomic_coords.at(1).at(5) == -0.162327); 
    assert(atomic_coords.at(2).at(5) == 4.079396); 
    cout << "Atom 6 coordinates correct" << endl; 
 
  }
  cout << "Testing: LogReader read" << endl;
  {

    LogReader lr("../../../GAUSSIANFILES/90_unordered/90_pair.log");
    lr.read();
    auto orb_info = lr.getOrbitalInfo();

    auto Soverlap = lr.getOverlapMatrix();
    cout << Soverlap.rows() << endl;
    auto Alpha = lr.getOE("Alpha");

    auto basisFuncCount = lr.getBasisFuncCount();
    for (auto c : basisFuncCount) {
      cout << c << endl;
    }
    auto xyz = lr.getCoords();
    auto x = xyz.at(0);
    auto y = xyz.at(1);
    auto z = xyz.at(2);
    for (size_t ind = 0; ind < x.size(); ++ind) {
      cout << x.at(ind) << " " << y.at(ind) << " " << z.at(ind) << endl;
    }
  }
  return 0;
}
