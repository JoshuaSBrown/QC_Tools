
#ifndef _CATNIP_PARAMETERS_HPP_
#define _CATNIP_PARAMETERS_HPP_

#include <string>

namespace catnip {

class Parameters {
 private:
  std::string log1_;
  std::string log2_;
  std::string logP_;
  std::string pun1_;
  std::string pun2_;
  std::string punP_;

  std::string spinP;
  std::string spin1;
  std::string spin2;

  std::string orb_typeP;
  std::string orb_type1;
  std::string orb_type2;

  int orb_numP;
  int orb_num1;
  int orb_num2;

  bool citation;
  bool counterPoise;

 public:
  Parameters()
      : log1_(""),
        log2_(""),
        logP_(""),
        pun1_(""),
        pun2_(""),
        punP_(""),
        spinP(""),
        spin1(""),
        spin2(""),
        orb_typeP(""),
        orb_type1(""),
        orb_type2(""),
        orb_numP(0),
        orb_num1(0),
        orb_num2(0),
        counterPoise(false){};

  std::string getLog1() const { return log1_; }
  std::string getLog2() const { return log2_; }
  std::string getLogP() const { return logP_; }
  std::string getPun1() const { return pun1_; }
  std::string getPun2() const { return pun2_; }
  std::string getPunP() const { return punP_; }

  std::string getSpinP() const { return spinP; }
  std::string getSpin1() const { return spin1; }
  std::string getSpin2() const { return spin2; }

  std::string getOrbTypeP() const { return orb_typeP; }
  std::string getOrbType1() const { return orb_type1; }
  std::string getOrbType2() const { return orb_type2; }

  int getOrbNumP() const { return orb_numP; }
  int getOrbNum1() const { return orb_num1; }
  int getOrbNum2() const { return orb_num2; }
  bool getCounterPoise() const { return counterPoise; }
  bool getCitation() const { return citation; }

  void setLog1(const std::string& log1);
  void setLog2(const std::string& log2);
  void setLogP(const std::string& logP);
  void setPun1(const std::string& pun1);
  void setPun2(const std::string& pun2);
  void setPunP(const std::string& punP);

  void setSpinP(const std::string& spin) { spinP = spin; }
  void setSpin1(const std::string& spin) { spin1 = spin; }
  void setSpin2(const std::string& spin) { spin2 = spin; }

  void setOrbTypeP(const std::string& orb) { orb_typeP = orb; }
  void setOrbType1(const std::string& orb) { orb_type1 = orb; }
  void setOrbType2(const std::string& orb) { orb_type2 = orb; }

  void setOrbNumP(const int& num) { orb_numP = num; }
  void setOrbNum1(const int& num) { orb_num1 = num; }
  void setOrbNum2(const int& num) { orb_num2 = num; }
  void setCounterPoise(bool cp) { counterPoise = cp; }
  void setCitation(bool cite) { citation = cite; }
};

}  // namespace catnip
#endif  // _CATNIP_PARAMETERS_HPP_
