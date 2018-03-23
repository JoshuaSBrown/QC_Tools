
#ifndef _PARAMETERS_HPP_
#define _PARAMETERS_HPP_

#include <string>

class Parameters {
  private:
    std::string log1_;
    std::string log2_;
    std::string logP_;
    std::string pun1_;
    std::string pun2_;
    std::string punP_;
    int MO1_Alpha_;
    int MO2_Alpha_;
    int MOP_Alpha_;
    int MO1_Beta_;
    int MO2_Beta_;
    int MOP_Beta_;
    int HOMO_MO1_Alpha_;
    int HOMO_MO2_Alpha_;
    int HOMO_MOP_Alpha_;
    int HOMO_MO1_Beta_;
    int HOMO_MO2_Beta_;
    int HOMO_MOP_Beta_;


  public:
    Parameters() : log1_(""), log2_(""), logP_(""),pun1_(""), pun2_(""), punP_(""), MO1_Alpha_(-1),MO2_Alpha_(-1),MOP_Alpha_(-1),MO1_Beta_(-1),MO2_Beta_(-1),MOP_Beta_(-1), HOMO_MO1_Alpha_(-1),HOMO_MO2_Alpha_(-1),HOMO_MOP_Alpha_(-1),HOMO_MO1_Beta_(-1),HOMO_MO2_Beta_(-1),HOMO_MOP_Beta_(-1){};
    std::string getLog1() const { return log1_; }
    std::string getLog2() const { return log2_; }
    std::string getLogP() const { return logP_; }
    std::string getPun1() const { return pun1_; }
    std::string getPun2() const { return pun2_; }
    std::string getPunP() const { return punP_; }
    int getMO1_Alpha() const { return MO1_Alpha_; }
    int getMO2_Alpha() const { return MO2_Alpha_; }
    int getMOP_Alpha() const { return MOP_Alpha_; }
    int getMO1_Beta() const { return MO1_Beta_; }
    int getMO2_Beta() const { return MO2_Beta_; }
    int getHOMO_MO1Alpha() const { return HOMO_MO1_Alpha_; }
    int getHOMO_MO2Alpha() const { return HOMO_MO2_Alpha_; }
    int getHOMO_MOPAlpha() const { return HOMO_MOP_Alpha_; }
    int getHOMO_MO1Beta() const { return HOMO_MO1_Beta_; }
    int getHOMO_MO2Beta() const { return HOMO_MO2_Beta_; }
    int getHOMO_MOPBeta() const { return HOMO_MOP_Beta_; }

   int getMOP_Beta_() const { return MOP_Beta_; }

    void setLog1(const std::string log1);
    void setLog2(const std::string log2);
    void setLogP(const std::string logP);
    void setPun1(const std::string pun1);
    void setPun2(const std::string pun2);
    void setPunP(const std::string punP);
    void setMO1Alpha(const int mo) { MO1_Alpha_ = mo; }
    void setMO2Alpha(const int mo) { MO2_Alpha_ = mo; }
    void setMOPAlpha(const int mo) { MOP_Alpha_ = mo; }
    void setMO1Beta(const int mo) { MO1_Beta_ = mo; }
    void setMO2Beta(const int mo) { MO2_Beta_ = mo; }
    void setMOPBeta(const int mo) { MOP_Beta_ = mo; }
    void setHOMO_MO1Alpha(const int mo) { HOMO_MO1_Alpha_ = mo; }
    void setHOMO_MO2Alpha(const int mo) { HOMO_MO2_Alpha_ = mo; }
    void setHOMO_MOPAlpha(const int mo) { HOMO_MOP_Alpha_ = mo; }
    void setHOMO_MO1Beta(const int mo) { HOMO_MO1_Beta_ = mo; }
    void setHOMO_MO2Beta(const int mo) { HOMO_MO2_Beta_ = mo; }
    void setHOMO_MOPBeta(const int mo) { HOMO_MOP_Beta_ = mo; }
};

#endif
