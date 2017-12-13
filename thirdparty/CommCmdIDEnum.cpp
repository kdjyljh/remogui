#include "CommCmdIDEnum.hpp"
#include "Mix.hpp"

CommCmdIDEnumDesc::CommCmdIDEnumDesc() {
  en2str_[COMMCMDID_PING] = "COMMCMDID_PING";
  str2en_["COMMCMDID_PING"] = COMMCMDID_PING;
  set_.insert(COMMCMDID_PING);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDID_DEVICE_VERSION] = "COMMCMDID_DEVICE_VERSION";
  str2en_["COMMCMDID_DEVICE_VERSION"] = COMMCMDID_DEVICE_VERSION;
  set_.insert(COMMCMDID_DEVICE_VERSION);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDID_DEVICE_STATUS] = "COMMCMDID_DEVICE_STATUS";
  str2en_["COMMCMDID_DEVICE_STATUS"] = COMMCMDID_DEVICE_STATUS;
  set_.insert(COMMCMDID_DEVICE_STATUS);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDID_MAX_DEVICE] = "COMMCMDID_MAX_DEVICE";
  str2en_["COMMCMDID_MAX_DEVICE"] = COMMCMDID_MAX_DEVICE;
  set_.insert(COMMCMDID_MAX_DEVICE);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
};
GENERATE_CLASS_SINGLETON_GET(CommCmdIDEnumDesc)

bool CommCmdIDEnum_isvalid(const CommCmdIDEnum en) {
  auto &&enumSet = CommCmdIDEnumDesc::Get()->set();
  auto it = enumSet.find(en);
  return it != enumSet.end();
}

bool CommCmdIDEnum_isvalid(const uint64_t item) {
  auto &&enumSet = CommCmdIDEnumDesc::Get()->set();
  auto it = enumSet.find(static_cast<CommCmdIDEnum>(item));
  return it != enumSet.end();
}

std::string CommCmdIDEnum_getstr(const CommCmdIDEnum en) {
  auto &&en2str = CommCmdIDEnumDesc::Get()->en2str();
  auto it = en2str.find(en);
  CHECK(it != en2str.end());
  return it->second;
}

CommCmdIDEnum CommCmdIDEnum_getstr(const std::string desc) {
  auto &&str2en = CommCmdIDEnumDesc::Get()->str2en();
  auto it = str2en.find(desc);
  CHECK(it != str2en.end()) << desc;
  return it->second;
}

uint64_t CommCmdIDEnum_getuint(const CommCmdIDEnum en) {
  CHECK(CommCmdIDEnum_isvalid(en));
  return static_cast<uint64_t>(en);
}

CommCmdIDEnum CommCmdIDEnum_getuint(uint64_t item) {
  CHECK(CommCmdIDEnum_isvalid(item));
  return static_cast<CommCmdIDEnum>(item);
}

