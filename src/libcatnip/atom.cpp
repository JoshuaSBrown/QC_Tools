#include "atom.hpp"

namespace catnip {
  double round(double N, double n) 
  { 
    int h; 
    double  b, d, e, i, j, m, f; 
    b = N; 

    // Counting the no. of digits to the left of decimal point 
    // in the given no. 
    for (i = 0; b >= 1; ++i) 
      b = b / 10; 

    d = n - i; 
    b = N; 
    b = b * std::pow(10, d); 
    e = b + 0.5; 
    if ((float)e == (float)std::ceil(b)) { 
      f = (std::ceil(b)); 
      h = f - 2; 
      if (h % 2 != 0) { 
        e = e - 1; 
      } 
    } 
    j = std::floor(e); 
    m = std::pow(10, d); 
    j = j / m; 
    return j;
  }
}
