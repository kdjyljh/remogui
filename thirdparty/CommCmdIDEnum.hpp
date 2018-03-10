#ifndef COMMCMDIDENUM_HPP
#define COMMCMDIDENUM_HPP

#include "systemusrlib.h"

enum CommCmdIDEnum {
  COMMCMDID_PING             = 0x00,
  COMMCMDID_DEVICE_VERSION   = 0x10,
  COMMCMDID_DEVICE_STATUS    = 0x11,
  COMMCMDID_MAX_DEVICE       = 0x12,
};


class CommCmdIDEnumDesc {
 public:
  const unordered_map<CommCmdIDEnum, std::string> &en2str() const {return en2str_;}
  const unordered_map<std::string, CommCmdIDEnum> &str2en() const {return str2en_;}
  const unordered_set<CommCmdIDEnum> &set() const {return set_;}

  static shared_ptr<CommCmdIDEnumDesc> Get();
 private:
  unordered_map<CommCmdIDEnum, std::string> en2str_;
  unordered_map<std::string, CommCmdIDEnum> str2en_;
  unordered_set<CommCmdIDEnum> set_;

  CommCmdIDEnumDesc();
};

std::string CommCmdIDEnum_getstr(const CommCmdIDEnum);
CommCmdIDEnum CommCmdIDEnum_getstr(const std::string);
uint64_t CommCmdIDEnum_getuint(const CommCmdIDEnum);
CommCmdIDEnum CommCmdIDEnum_getuint(uint64_t);
bool CommCmdIDEnum_isvalid(const CommCmdIDEnum);
bool CommCmdIDEnum_isvalid(const uint64_t);



#endif  // COMMCMDIDENUM_HPP

