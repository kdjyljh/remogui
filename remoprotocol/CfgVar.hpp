#ifndef CFGVAR_HPP
#define CFGVAR_HPP

#include "systemusrlib.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class CfgVar {
 public:
  template<typename T>
  T get_check_convert(vector<string> path) {
    auto strvec = get(path); CHECK(strvec.size() == 1);
    return lexical_cast<T>(strvec[0]);
  }
  vector<string> get(vector<string> path, bool failcheck = true);
  vector<vector<string>> getvec(vector<string> path, bool failcheck = true);
  string print();
  CfgVar(string jsonFileName);
 private:
  const string jsonFileName_;
  boost::property_tree::ptree tree_;
};

#endif //CFGVAR_HPP
