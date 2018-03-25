
#ifndef _LogReader_HPP_
#define _LogReader_HPP_

#include <vector>

#include "../MATRIX/matrix.hpp"
#include "filereader.hpp"
// Gaussian log file reader

typedef std::map<std::pair<int,std::string>,std::map<std::string,std::vector<double>>> orb_cont;

class LogReader : public FileReader {
  public:
    LogReader(std::string str);     
    orb_cont getOrbitalInfo() { return orb_; }
    Matrix * getOverlapMatrix() { return S_; }
  private:
    virtual void registerSections_();
    virtual void validFileName_();
    static void AOFunctionSectionReader(void *);
    static void OverlapSectionReader(void *);
    
    // Contains the information as so:
    // each element in the map refers to an atom and its type C, H, N etc
    // The first element of the second map contains the orbital type
    // 1S, 2S, 3PY etc...
    // The vector contains the Total Alpha Beta and Spin if there are no
    // Beta's the vector will be one item shorter 
    orb_cont orb_;
    // Overlap matrix
    Matrix * S_;
};

#endif
