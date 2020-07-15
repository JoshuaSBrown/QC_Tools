
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../libcatnip/io/file_readers/punreader.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <eigen3/Eigen/Dense>

using namespace catnip;
using namespace std;

TEST_CASE("Pun Reader","[unit]") {

  cerr << "Testing: PunReader Constructor" << endl;
  { PunReader pr("file.pun"); }

  cerr << "Testing: PunReader read" << endl;
  {
    PunReader pr("../../../GAUSSIANFILES/90_unordered/90_pair.pun");
    pr.read();

    Eigen::MatrixXd m = pr.getCoefsMatrix("Alpha");
    assert(m.rows() == 92);
    assert(pr.restrictedShell());
  }
}
