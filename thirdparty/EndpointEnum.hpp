#ifndef ENDPOINTENUM_HPP
#define ENDPOINTENUM_HPP

#include "systemusrlib.h"

enum EndpointEnum {
  ENDPOINT_UNKNOWN     = 0,
  ENDPOINT_QTGUI       = 1,
  ENDPOINT_CAMERA      = 2,
};


class EndpointEnumDesc {
 public:
  const unordered_map<EndpointEnum, std::string> &en2str() const {return en2str_;}
  const unordered_map<std::string, EndpointEnum> &str2en() const {return str2en_;}
  const unordered_set<EndpointEnum> &set() const {return set_;}

  static shared_ptr<EndpointEnumDesc> Get();
 private:
  unordered_map<EndpointEnum, std::string> en2str_;
  unordered_map<std::string, EndpointEnum> str2en_;
  unordered_set<EndpointEnum> set_;

  EndpointEnumDesc();
};

std::string EndpointEnum_getstr(const EndpointEnum);
EndpointEnum EndpointEnum_getstr(const std::string);
uint64_t EndpointEnum_getuint(const EndpointEnum);
EndpointEnum EndpointEnum_getuint(uint64_t);
bool EndpointEnum_isvalid(const EndpointEnum);
bool EndpointEnum_isvalid(const uint64_t);



#endif  // ENDPOINTENUM_HPP

