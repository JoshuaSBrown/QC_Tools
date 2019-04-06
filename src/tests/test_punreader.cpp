
#include "../libcatnip/io/file_readers/punreader.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: PunReader Constructor" << endl;
  { PunReader pr("file.pun"); }

  cerr << "Testing: PunReader read" << endl;
  {
    PunReader pr("../../../GAUSSIANFILES/90_unordered/90_pair.pun");
    pr.read();

    auto m = pr.getCoefsMatrix("Alpha");
    assert(m->get_rows() == 92);
    assert(pr.restrictedShell());
  }
  return 0;
}
