

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../libcatnip/io/file_readers/logreader.hpp"
#include "../libcatnip/io/io.hpp"

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: LogReader Constructor" << endl;
  { LogReader lr("file.log"); }

  cerr << "Testing: LogReader read" << endl;
  {

    LogReader lr("../../../GAUSSIANFILES/90_unordered/90_pair.log");
    lr.read();
    auto orb_info = lr.getOrbitalInfo();

    auto Soverlap = lr.getOverlapMatrix();
    cout << Soverlap->get_rows() << endl;
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
