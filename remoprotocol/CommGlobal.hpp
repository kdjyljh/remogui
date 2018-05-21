#ifndef COMMGLOBAL_HPP
#define COMMGLOBAL_HPP

#include "systemusrlib.h"
#include "EndpointEnum.hpp"
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include "TimedTask.hpp"
#include "CommDeviceEnum.hpp"

class IPPortCfg {
 public:
  inline string COMM_CAMERA_IP() {return COMM_CAMERA_IP_;}
  inline string COMM_CAMERA_PORT() {return COMM_CAMERA_PORT_;}
  inline uint16_t COMM_SELF_PORT() {return COMM_SELF_PORT_;}
  inline CommDeviceEnum SelfCommDevice() {return SelfCommDevice_;}

  static shared_ptr<IPPortCfg> Get();
 private:
  string COMM_CAMERA_IP_;
  string COMM_CAMERA_PORT_;
  uint16_t COMM_SELF_PORT_;
  CommDeviceEnum SelfCommDevice_;
  IPPortCfg();
};

typedef boost::asio::ip::udp::endpoint UDPEndpoint;
typedef boost::shared_ptr<boost::asio::ip::udp::endpoint> UDPEndpointPtr;
typedef boost::shared_ptr<boost::asio::ip::udp::socket> UDPSocketPtr;

typedef boost::asio::ip::tcp::endpoint TCPEndpoint;
typedef boost::shared_ptr<boost::asio::ip::tcp::endpoint> TCPEndpointPtr;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> TCPSocketPtr;

class UDPEndpoints : public boost::noncopyable {
 public:
  UDPEndpointPtr fetch(EndpointEnum endp);

  static boost::shared_ptr<UDPEndpoints> Get();
 private:
  boost::asio::io_service io_service_;
  boost::unordered_map<EndpointEnum, UDPEndpointPtr> mapEndpEnum2Ptr_;
  UDPEndpoints();
};

class TCPEndpoints : public boost::noncopyable {
 public:
  TCPEndpointPtr fetch(EndpointEnum endp);

  static boost::shared_ptr<TCPEndpoints> Get();
 private:
  boost::asio::io_service io_service_;
  boost::unordered_map<EndpointEnum, TCPEndpointPtr> mapEndpEnum2Ptr_;
  TCPEndpoints();
};

typedef std::vector<char> CommMessage;
typedef boost::shared_ptr<CommMessage> CommMessagePtr;

struct CommInfo4Sender {
  EndpointEnum     endp; // 发送给谁?
  CommMessagePtr   msg;  // 发送什么信息?
  bool operator< (const CommInfo4Sender &other) const;
};

#endif //COMMGLOBAL_HPP
