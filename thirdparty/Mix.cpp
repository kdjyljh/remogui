#include "Mix.hpp"

//std::string convert_fromBaseType(float  data, int setPrecision) {
//  std::stringstream ss;
//  ss << std::fixed << std::setprecision(setPrecision) << data;
//  return ss.str();
//}
//std::string convert_fromBaseType(double data, int setPrecision) {
//  std::stringstream ss;
//  ss << std::fixed << std::setprecision(setPrecision) << data;
//  return ss.str();
//}

//template<typename Dtype>
//inline std::string convert_fromVectorFLT(vector<Dtype> fltvec) {
//  std::string res;
//  for (auto it = fltvec.begin(); it != fltvec.end(); ++it) {
//    if (it != fltvec.begin())
//      res += ", ";
//    res += convert_fromBaseType(*it, 4);
//  }
//  return "("+res+")";
//}
//std::string convert_fromVector(vector<float> fltvec) {
//  return convert_fromVectorFLT(fltvec);
//}
//std::string convert_fromVector(vector<double> dblvec) {
//  return convert_fromVectorFLT(dblvec);
//}

//std::string convert_fromVector(vector<char> chvec) {
//  std::stringstream ss;
//  for (unsigned idx = 0; idx < chvec.size(); ++idx) {
//    if (idx != 0)
//      ss << ", ";
//    ss << std::hex << std::setw(2) << std::setfill('0') << "0x" << int(uint8_t(chvec[idx]));
//  }
//  return "("+ss.str()+")";
//}
//std::string convert_fromVector(char *buf, int buflen) {
//  return convert_fromVector(vector<char>(buf, buf+buflen));
//}
