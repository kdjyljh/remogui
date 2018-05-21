#ifndef COMMCMDSETENUM_HPP
#define COMMCMDSETENUM_HPP

#include "systemusrlib.h"

enum CommCmdSetEnum {
  COMMCMDSET_UNIVERSAL     = 0,
  COMMCMDSET_CAMERA        = 1,
  COMMCMDSET_TRIOPHEAD     = 2,
  COMMCMDSET_BATTERY       = 3,
  COMMCMDSET_INTERACTION   = 4,
  COMMCMDSET_FACTORY       = 5,
};


class CommCmdSetEnumDesc {
 public:
  const unordered_map<CommCmdSetEnum, std::string> &en2str() const {return en2str_;}
  const unordered_map<std::string, CommCmdSetEnum> &str2en() const {return str2en_;}
  const unordered_set<CommCmdSetEnum> &set() const {return set_;}

  static shared_ptr<CommCmdSetEnumDesc> Get();
 private:
  unordered_map<CommCmdSetEnum, std::string> en2str_;
  unordered_map<std::string, CommCmdSetEnum> str2en_;
  unordered_set<CommCmdSetEnum> set_;

  CommCmdSetEnumDesc();
};

std::string CommCmdSetEnum_getstr(const CommCmdSetEnum);
CommCmdSetEnum CommCmdSetEnum_getstr(const std::string);
uint64_t CommCmdSetEnum_getuint(const CommCmdSetEnum);
CommCmdSetEnum CommCmdSetEnum_getuint(uint64_t);
bool CommCmdSetEnum_isvalid(const CommCmdSetEnum);
bool CommCmdSetEnum_isvalid(const uint64_t);



#endif  // COMMCMDSETENUM_HPP

