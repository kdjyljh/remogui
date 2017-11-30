#include "CommCmdSetEnum.hpp"
#include "Mix.hpp"

CommCmdSetEnumDesc::CommCmdSetEnumDesc() {
  en2str_[COMMCMDSET_UNIVERSAL] = "COMMCMDSET_UNIVERSAL";
  str2en_["COMMCMDSET_UNIVERSAL"] = COMMCMDSET_UNIVERSAL;
  set_.insert(COMMCMDSET_UNIVERSAL);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDSET_CAMERA] = "COMMCMDSET_CAMERA";
  str2en_["COMMCMDSET_CAMERA"] = COMMCMDSET_CAMERA;
  set_.insert(COMMCMDSET_CAMERA);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDSET_TRIOPHEAD] = "COMMCMDSET_TRIOPHEAD";
  str2en_["COMMCMDSET_TRIOPHEAD"] = COMMCMDSET_TRIOPHEAD;
  set_.insert(COMMCMDSET_TRIOPHEAD);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDSET_BATTERY] = "COMMCMDSET_BATTERY";
  str2en_["COMMCMDSET_BATTERY"] = COMMCMDSET_BATTERY;
  set_.insert(COMMCMDSET_BATTERY);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDSET_INTERACTION] = "COMMCMDSET_INTERACTION";
  str2en_["COMMCMDSET_INTERACTION"] = COMMCMDSET_INTERACTION;
  set_.insert(COMMCMDSET_INTERACTION);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMCMDSET_FACTORY] = "COMMCMDSET_FACTORY";
  str2en_["COMMCMDSET_FACTORY"] = COMMCMDSET_FACTORY;
  set_.insert(COMMCMDSET_FACTORY);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
};
GENERATE_CLASS_SINGLETON_GET(CommCmdSetEnumDesc)

bool CommCmdSetEnum_isvalid(const CommCmdSetEnum en) {
  auto &&enumSet = CommCmdSetEnumDesc::Get()->set();
  auto it = enumSet.find(en);
  return it != enumSet.end();
}

bool CommCmdSetEnum_isvalid(const uint64_t item) {
  auto &&enumSet = CommCmdSetEnumDesc::Get()->set();
  auto it = enumSet.find(static_cast<CommCmdSetEnum>(item));
  return it != enumSet.end();
}

std::string CommCmdSetEnum_getstr(const CommCmdSetEnum en) {
  auto &&en2str = CommCmdSetEnumDesc::Get()->en2str();
  auto it = en2str.find(en);
  CHECK(it != en2str.end());
  return it->second;
}

CommCmdSetEnum CommCmdSetEnum_getstr(const std::string desc) {
  auto &&str2en = CommCmdSetEnumDesc::Get()->str2en();
  auto it = str2en.find(desc);
  CHECK(it != str2en.end()) << desc;
  return it->second;
}

uint64_t CommCmdSetEnum_getuint(const CommCmdSetEnum en) {
  CHECK(CommCmdSetEnum_isvalid(en));
  return static_cast<uint64_t>(en);
}

CommCmdSetEnum CommCmdSetEnum_getuint(uint64_t item) {
  CHECK(CommCmdSetEnum_isvalid(item));
  return static_cast<CommCmdSetEnum>(item);
}

