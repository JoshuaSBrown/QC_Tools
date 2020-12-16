#pragma once
#ifndef _CATNIP_PUNREADER_HPP_
#define _CATNIP_PUNREADER_HPP_

#include <memory>
#include <vector>

#include "filereader.hpp"
#include <eigen3/Eigen/Dense>
// Gaussian fort.7/.pun file reader
namespace catnip {

class PunReader : public FileReader {
 public:
  explicit PunReader(const std::string &str);
  Eigen::MatrixXd getCoefsMatrix(const std::string &orb_type);
  bool restrictedShell() { return coefs.size() == 1; }

 private:
  virtual void registerSections_();
  virtual void validFileName_();

  static void OrbitalCoefAlphaSectionReader(void *);
  static void OrbitalCoefBetaSectionReader(void *);

  void ReadCoef(const std::string &orb_type);
  std::vector<double> readGausCoefLine(const std::string &line);

  std::map<std::string, std::unique_ptr<Eigen::MatrixXd>> coefs;
};

}  // namespace catnip
#endif  //  _CATNIP_PUNREADER_HPP_
