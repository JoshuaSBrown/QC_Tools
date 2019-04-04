
#ifndef _CATNIP_PUNREADER_HPP_
#define _CATNIP_PUNREADER_HPP_

#include <vector>

#include "../../MATRIX/matrix.hpp"
#include "filereader.hpp"
// Gaussian fort.7/.pun file reader
namespace catnip {

class PunReader : public FileReader {
  public:
    PunReader(const std::string & str);     
    Matrix * getCoefsMatrix(const std::string & orb_type); 
    bool restrictedShell() { return coefs.size()==1; }

  private:
    virtual void registerSections_();
    virtual void validFileName_();

    static void OrbitalCoefAlphaSectionReader(void *);
    static void OrbitalCoefBetaSectionReader(void *);

    void ReadCoef(const std::string & orb_type);
    std::vector<double> readGausCoefLine(const std::string & line);
    
    std::map<std::string,Matrix *> coefs;
};

}
#endif //  _CATNIP_PUNREADER_HPP_
