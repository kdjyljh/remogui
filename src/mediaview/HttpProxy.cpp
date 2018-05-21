//
// Created by jianghualuo on 18-3-13.
//

#include "HttpProxy.h"

#include <boost/lexical_cast.hpp>
#include<boost/algorithm/string.hpp>
#include <glog/logging.h>

using boost::asio::ip::tcp;

HttpProxy::HttpProxy() {

}

HttpProxy::~HttpProxy() {

}

bool HttpProxy::getRequest(std::string url, std::vector<uint8_t> &buff,
                           std::string connection, std::string accept,
                           std::string protocolVersion) {
    LOG(INFO) << "HttpProxy::getRequest start url:" << url;
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    std::string host, path;
    std::string errorMsg;
    if (!parseUrl(url, host, path)) {
        errorMsg = "parseUrl error";
        goto error;
    }
    else {
        try {
            tcp::resolver::query query(host, "http");
            tcp::resolver::iterator edpIt = resolver.resolve(query);
            tcp::socket socket(io_service);
            boost::asio::connect(socket, edpIt);

            boost::asio::streambuf reqBuff;
            std::ostream reqStream(&reqBuff);
            reqStream << "GET " << path << " " << protocolVersion << "\r\n"
                      << "Host: " << host << "\r\n"
                      << "Accept: " << accept << "\r\n"
                      << "Connection: " << connection << "\r\n"
                      << "\r\n";
            boost::asio::write(socket, reqBuff);

            boost::asio::streambuf respBuff;
            std::istream respStream(&respBuff);

            std::string protoVersion, statusMsg;
            unsigned int statusCode;
            boost::asio::read_until(socket, respBuff, "\r\n\r\n");
            respStream >> protoVersion >> statusCode;
            std::getline(respStream, statusMsg);
            if (protoVersion.substr(0, 5) != "HTTP/") {
                errorMsg = "HttpProxy::getRequest response remoprotocol error";
                goto error;
            }
            if (statusCode != 200) {
                errorMsg = std::string("HttpProxy::getRequest error code:") +
                        boost::lexical_cast<std::string>(statusCode) + " message: " + statusMsg;
                goto error;
            }

            int contentLength = 0;
            boost::asio::read_until(socket, respBuff, "\r\n\r\n");
            std::string header;
            while (std::getline(respStream, header) && header != "\r") {
                LOG(INFO) << header << std::endl;
                if (header.find("Content-Length") != std::string::npos) {
                    int pos = header.find(':') + 1;
                    std::string strLength = header.substr(pos, header.size() - pos);
                    boost::trim(strLength);
                    contentLength = boost::lexical_cast<int>(strLength);
                }
            }
            LOG(INFO) << std::endl;

            int remainSize = contentLength - respBuff.size();

            boost::system::error_code error;
            if (remainSize > 0) {
                boost::asio::read(socket, respBuff,
                                         boost::asio::transfer_at_least(remainSize), error);
                if (error != boost::asio::error::eof && error.value() != boost::system::errc::success) {
                    LOG(INFO) << error.message();
                    throw boost::system::system_error(error);
                }
                else {
                    buff.resize(contentLength);
                    boost::asio::buffer_copy(boost::asio::buffer(buff), respBuff.data());
                }
            }
            else {
                boost::asio::read(socket, respBuff, error);
                if (error == boost::asio::error::eof) {
                    buff.resize(respBuff.size());
                    boost::asio::buffer_copy(boost::asio::buffer(buff), respBuff.data());
                }
                else {
                    LOG(INFO) << error.message();
                    throw boost::system::system_error(error);
                }
            }
        } catch (std::exception e) {
            errorMsg = std::string("HttpProxy::getRequest ") + e.what();
            goto error;
        }
    }

    return true;
error:
    LOG(INFO) << "got an error to get resource from url:" << url;
    LOG(INFO) << errorMsg;
    return false;
}

bool HttpProxy::parseUrl(const std::string &url, std::string &host, std::string &path) {
    std::string::size_type pos1 = url.find("http://") + strlen("http://");
    if (pos1 != 7)
        return false;
    std::string::size_type pos2 = url.find("/", pos1);
    host = url.substr(pos1, pos2 - pos1);
    path = url.substr(pos2, url.size() - pos2);
    return true;
}
