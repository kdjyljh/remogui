#include "CommDeviceEnum.hpp"
#include "Mix.hpp"

CommDeviceEnumDesc::CommDeviceEnumDesc() {
  en2str_[COMMDEVICE_CAMERA] = "COMMDEVICE_CAMERA";
  str2en_["COMMDEVICE_CAMERA"] = COMMDEVICE_CAMERA;
  set_.insert(COMMDEVICE_CAMERA);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMDEVICE_GIMBAL] = "COMMDEVICE_GIMBAL";
  str2en_["COMMDEVICE_GIMBAL"] = COMMDEVICE_GIMBAL;
  set_.insert(COMMDEVICE_GIMBAL);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMDEVICE_ALGORITHM] = "COMMDEVICE_ALGORITHM";
  str2en_["COMMDEVICE_ALGORITHM"] = COMMDEVICE_ALGORITHM;
  set_.insert(COMMDEVICE_ALGORITHM);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMDEVICE_BATTERY] = "COMMDEVICE_BATTERY";
  str2en_["COMMDEVICE_BATTERY"] = COMMDEVICE_BATTERY;
  set_.insert(COMMDEVICE_BATTERY);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMDEVICE_APP] = "COMMDEVICE_APP";
  str2en_["COMMDEVICE_APP"] = COMMDEVICE_APP;
  set_.insert(COMMDEVICE_APP);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMDEVICE_PC] = "COMMDEVICE_PC";
  str2en_["COMMDEVICE_PC"] = COMMDEVICE_PC;
  set_.insert(COMMDEVICE_PC);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[COMMDEVICE_BROADCAST] = "COMMDEVICE_BROADCAST";
  str2en_["COMMDEVICE_BROADCAST"] = COMMDEVICE_BROADCAST;
  set_.insert(COMMDEVICE_BROADCAST);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
};
GENERATE_CLASS_SINGLETON_GET(CommDeviceEnumDesc)

bool CommDeviceEnum_isvalid(const CommDeviceEnum en) {
  auto &&enumSet = CommDeviceEnumDesc::Get()->set();
  auto it = enumSet.find(en);
  return it != enumSet.end();
}

bool CommDeviceEnum_isvalid(const uint64_t item) {
  auto &&enumSet = CommDeviceEnumDesc::Get()->set();
  auto it = enumSet.find(static_cast<CommDeviceEnum>(item));
  return it != enumSet.end();
}

std::string CommDeviceEnum_getstr(const CommDeviceEnum en) {
  auto &&en2str = CommDeviceEnumDesc::Get()->en2str();
  auto it = en2str.find(en);
  CHECK(it != en2str.end());
  return it->second;
}

CommDeviceEnum CommDeviceEnum_getstr(const std::string desc) {
  auto &&str2en = CommDeviceEnumDesc::Get()->str2en();
  auto it = str2en.find(desc);
  CHECK(it != str2en.end()) << desc;
  return it->second;
}

uint64_t CommDeviceEnum_getuint(const CommDeviceEnum en) {
  CHECK(CommDeviceEnum_isvalid(en));
  return static_cast<uint64_t>(en);
}

CommDeviceEnum CommDeviceEnum_getuint(uint64_t item) {
  CHECK(CommDeviceEnum_isvalid(item));
  return static_cast<CommDeviceEnum>(item);
}

