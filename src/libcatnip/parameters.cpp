
#include "parameters.hpp"
#include "string_support.hpp"
#include <stdexcept>
#include <string>

using namespace catnip;
using namespace std;

void Parameters::setLog1(const string& log1) {
  string ext = lastN(log1, 4);
  if (ext.compare(".log"))
    throw invalid_argument("log file has wrong extension");
  log1_ = log1;
}
void Parameters::setLog2(const string& log2) {
  string ext = lastN(log2, 4);
  if (ext.compare(".log"))
    throw invalid_argument("log file has wrong extension");
  log2_ = log2;
}
void Parameters::setLogP(const string& logP) {
  string ext = lastN(logP, 4);
  if (ext.compare(".log"))
    throw invalid_argument("log file has wrong extension");
  logP_ = logP;
}
void Parameters::setPun1(const string& pun1) {
  string ext = lastN(pun1, 4);
  if (ext.compare(".pun"))
    throw invalid_argument("pun file has wrong extension");
  pun1_ = pun1;
}
void Parameters::setPun2(const string& pun2) {
  string ext = lastN(pun2, 4);
  if (ext.compare(".pun"))
    throw invalid_argument("pun file has wrong extension");
  pun2_ = pun2;
}
void Parameters::setPunP(const string& punP) {
  string ext = lastN(punP, 4);
  if (ext.compare(".pun"))
    throw invalid_argument("pun file has wrong extension");
  punP_ = punP;
}
