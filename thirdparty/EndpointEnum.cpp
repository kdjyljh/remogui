#include "EndpointEnum.hpp"
#include "Mix.hpp"

EndpointEnumDesc::EndpointEnumDesc() {
  en2str_[ENDPOINT_UNKNOWN] = "ENDPOINT_UNKNOWN";
  str2en_["ENDPOINT_UNKNOWN"] = ENDPOINT_UNKNOWN;
  set_.insert(ENDPOINT_UNKNOWN);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[ENDPOINT_QTGUI] = "ENDPOINT_QTGUI";
  str2en_["ENDPOINT_QTGUI"] = ENDPOINT_QTGUI;
  set_.insert(ENDPOINT_QTGUI);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
  en2str_[ENDPOINT_CAMERA] = "ENDPOINT_CAMERA";
  str2en_["ENDPOINT_CAMERA"] = ENDPOINT_CAMERA;
  set_.insert(ENDPOINT_CAMERA);
  CHECK(en2str_.size() == set_.size() && str2en_.size() == set_.size());
    
};
GENERATE_CLASS_SINGLETON_GET(EndpointEnumDesc)

bool EndpointEnum_isvalid(const EndpointEnum en) {
  auto &&enumSet = EndpointEnumDesc::Get()->set();
  auto it = enumSet.find(en);
  return it != enumSet.end();
}

bool EndpointEnum_isvalid(const uint64_t item) {
  auto &&enumSet = EndpointEnumDesc::Get()->set();
  auto it = enumSet.find(static_cast<EndpointEnum>(item));
  return it != enumSet.end();
}

std::string EndpointEnum_getstr(const EndpointEnum en) {
  auto &&en2str = EndpointEnumDesc::Get()->en2str();
  auto it = en2str.find(en);
  CHECK(it != en2str.end());
  return it->second;
}

EndpointEnum EndpointEnum_getstr(const std::string desc) {
  auto &&str2en = EndpointEnumDesc::Get()->str2en();
  auto it = str2en.find(desc);
  CHECK(it != str2en.end()) << desc;
  return it->second;
}

uint64_t EndpointEnum_getuint(const EndpointEnum en) {
  CHECK(EndpointEnum_isvalid(en));
  return static_cast<uint64_t>(en);
}

EndpointEnum EndpointEnum_getuint(uint64_t item) {
  CHECK(EndpointEnum_isvalid(item));
  return static_cast<EndpointEnum>(item);
}

