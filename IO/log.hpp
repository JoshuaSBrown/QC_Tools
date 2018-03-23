
#include <vector>
#include <string>
#include <map>
// Gaussian log file reader

class logReader{
  public:
    logReader(std::string fileName) : fileName_(fileName); 
    std::map<std::string,map<std::string,std::vector<double>>> getOrb(void);
    
  private:
    std::string fileName_;
    // Contains the information as so:
    // each element in the map refers to an atom type C, H, N etc
    // The first element of the second map contains the orbital type
    // 1S, 2S, 3PY etc...
    // The vector contains the Total Alpha Beta and Spin if there are no
    // Beta's the vector will be one item shorter 
    std::map<std::string,map<std::string,std::vector<double>>> orb_;

};
