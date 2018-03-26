//
// Created by jianghualuo on 18-3-13.
//

#ifndef REMO_GUI_HTTPPROXY_H
#define REMO_GUI_HTTPPROXY_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

class HttpProxy : private boost::noncopyable {
public:
    ~HttpProxy();
    static bool getRequest(std::string url, std::vector<uint8_t> &buff,
                           std::string connection = "close", std::string accept = "*/*",
                           std::string protocolVersion = "HTTP/1.1");
    static bool getRequest(std::string url)  {
        std::vector<uint8_t> buff;
        return getRequest(url, buff);
    }
    static bool parseUrl(const std::string &url, std::string &host, std::string &path);

private:
    HttpProxy();

private:
    struct Request {
        std::string url;
        std::vector<uint8_t> &buff;
        std::string connection;
        std::string accept;
        std::string protocolVersion;
    };

};


#endif //REMO_GUI_HTTPPROXY_H
