#ifndef COMMDEVICEENUM_HPP
#define COMMDEVICEENUM_HPP

#include "systemusrlib.h"

enum CommDeviceEnum {
  COMMDEVICE_CAMERA      = 0x1,
  COMMDEVICE_TRIOPHEAD   = 0x2,
  COMMDEVICE_ALGORITHM   = 0x3,
  COMMDEVICE_BATTERY     = 0x4,
  COMMDEVICE_APP         = 0x5,
  COMMDEVICE_PC          = 0x6,
  COMMDEVICE_BROADCAST   = 0xf,
};


class CommDeviceEnumDesc {
 public:
  const unordered_map<CommDeviceEnum, std::string> &en2str() const {return en2str_;}
  const unordered_map<std::string, CommDeviceEnum> &str2en() const {return str2en_;}
  const unordered_set<CommDeviceEnum> &set() const {return set_;}

  static shared_ptr<CommDeviceEnumDesc> Get();
 private:
  unordered_map<CommDeviceEnum, std::string> en2str_;
  unordered_map<std::string, CommDeviceEnum> str2en_;
  unordered_set<CommDeviceEnum> set_;

  CommDeviceEnumDesc();
};

std::string CommDeviceEnum_getstr(const CommDeviceEnum);
CommDeviceEnum CommDeviceEnum_getstr(const std::string);
uint64_t CommDeviceEnum_getuint(const CommDeviceEnum);
CommDeviceEnum CommDeviceEnum_getuint(uint64_t);
bool CommDeviceEnum_isvalid(const CommDeviceEnum);
bool CommDeviceEnum_isvalid(const uint64_t);



#endif  // COMMDEVICEENUM_HPP

