#include "CommGlobal.hpp"
#include "Mix.hpp"
#include "CfgVar.hpp"

IPPortCfg::IPPortCfg() {
  CfgVar cfgVar("remo_gui.json");
  COMM_CAMERA_IP_ = cfgVar.get_check_convert<string>({"COMM_CAMERA_IP"});
  COMM_CAMERA_PORT_ = cfgVar.get_check_convert<string>({"COMM_CAMERA_PORT"});
  COMM_SELF_PORT_ = cfgVar.get_check_convert<uint16_t>({"COMM_SELF_PORT"});
  SelfCommDevice_ = CommDeviceEnum_getstr(cfgVar.get_check_convert<string>({"SelfCommDevice"}));

  LOG(INFO) << "IPPortCfg ctor:\n"
            << "COMM_CAMERA = " << COMM_CAMERA_IP_ << ":" << COMM_CAMERA_PORT_ << "\n"
            << "COMM_SELF_PORT = " << COMM_SELF_PORT_ << "\n"
            << "SelfCommDevice = " << CommDeviceEnum_getstr(SelfCommDevice_);
}
GENERATE_CLASS_SINGLETON_GET(IPPortCfg)

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

UDPEndpointPtr UDPEndpoints::fetch(EndpointEnum endp) {
  auto it = mapEndpEnum2Ptr_.find(endp);
  CHECK(it != mapEndpEnum2Ptr_.end());
  return it->second;
}

UDPEndpoints::UDPEndpoints() {
  udp::resolver resolver(io_service_);
  boost::shared_ptr<udp::resolver::query> query;

#define ADD_DEVICE_ENDPOINT(DEVICENAME) \
query = boost::make_shared<udp::resolver::query>(udp::v4(), IPPortCfg::Get()->COMM_##DEVICENAME##_IP(), IPPortCfg::Get()->COMM_##DEVICENAME##_PORT());\
mapEndpEnum2Ptr_[ENDPOINT_##DEVICENAME] = boost::make_shared<udp::endpoint>(*resolver.resolve(*query))

  ADD_DEVICE_ENDPOINT(CAMERA);

#undef ADD_DEVICE_ENDPOINT

  stringstream ss; ss << "\n\n\nUDPEndpoints ctor";
  for (auto &&pair : mapEndpEnum2Ptr_) {
    ss << "\n"
       << EndpointEnum_getstr(pair.first) << " <--> "
       << pair.second->address().to_string() << ":" << pair.second->port();
  }
  ss << "\n\n\n";
  LOG(INFO) << ss.str();
}
GENERATE_CLASS_SINGLETON_GET(UDPEndpoints)

TCPEndpointPtr TCPEndpoints::fetch(EndpointEnum endp) {
  auto it = mapEndpEnum2Ptr_.find(endp);
  CHECK(it != mapEndpEnum2Ptr_.end());
  return it->second;
}

TCPEndpoints::TCPEndpoints() {
  tcp::resolver resolver(io_service_);
  boost::shared_ptr<tcp::resolver::query> query;

#define ADD_DEVICE_ENDPOINT(DEVICENAME) \
query = boost::make_shared<tcp::resolver::query>(tcp::v4(), IPPortCfg::Get()->COMM_##DEVICENAME##_IP(), IPPortCfg::Get()->COMM_##DEVICENAME##_PORT());\
mapEndpEnum2Ptr_[ENDPOINT_##DEVICENAME] = boost::make_shared<tcp::endpoint>(*resolver.resolve(*query))

    ADD_DEVICE_ENDPOINT(CAMERA);

#undef ADD_DEVICE_ENDPOINT

    stringstream ss; ss << "\n\n\nTCPEndpoints ctor";
    for (auto &&pair : mapEndpEnum2Ptr_) {
      ss << "\n"
         << EndpointEnum_getstr(pair.first) << " <--> "
         << pair.second->address().to_string() << ":" << pair.second->port();
    }
    ss << "\n\n\n";
    LOG(INFO) << ss.str();
}
GENERATE_CLASS_SINGLETON_GET(TCPEndpoints)

bool CommInfo4Sender::operator<(const CommInfo4Sender &other) const {
  if (endp < other.endp)
    return true;
  if (endp == other.endp && msg < other.msg)
    return true;
  return false;
}
