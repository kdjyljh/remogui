#include "NetImage.hpp"
#include <iostream>

NetImage::NetImage(QObject *parent) {
  connect(&server, SIGNAL(newConnection()), this, SLOT(accept_connection()));
  server.setMaxPendingConnections(10); // 实际上只接收1个
  server.listen(QHostAddress::Any, 1111);
  client = nullptr;
}

void NetImage::accept_connection() {
  if (client) {
    std::cerr << "already has someone connected" << std::endl;
    return;
  }

  client = server.nextPendingConnection();
  connect(client, SIGNAL(readyRead()), this, SLOT(read_assemble()));
  connect(client, SIGNAL(disconnected()), this, SLOT(disconn_discard()));
  connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(err_discard(QAbstractSocket::SocketError)));
  std::cout << "a client connected !" << std::endl;
}

void NetImage::disconn_discard() {
  client->close();
  client = nullptr;
}

void NetImage::err_discard(QAbstractSocket::SocketError) {
  client->close();
  client = nullptr;
}

#include <cinttypes>
void NetImage::read_assemble() {
  const int maxFrameLen = 1e6;
  { // 读入数据
    int need2read = client->bytesAvailable();
//        std::cout << "try to read from a client, read " << need2read << " bytes!" << std::endl;
    char buffer[4096] = {0};
    while (need2read > 0) {
      int len = client->read(buffer, sizeof(buffer));
      recvBuf.insert(recvBuf.end(), buffer, buffer + len);
      need2read -= len;
    }
  }

  uint32_t head, frameLen; int logcnt = 0;
  for (;;) { // 解析头部
    if (recvBuf.size() < 8) // 首4+4个字节表示数据帧的帧头和长度
      return; // 不够解析,直接返回
    { // 读入到head和frameLen
      auto it = recvBuf.begin();
      char *tmp;
      tmp = reinterpret_cast<char *>(&head);
      for (int k = 0; k < 4; ++k) *tmp++ = *it++;
      tmp = reinterpret_cast<char *>(&frameLen);
      for (int k = 0; k < 4; ++k) *tmp++ = *it++;
    }

    if (head != 1234567890) { // 数据错误
      if (logcnt++ < 5) { // 打印信息
        char buf[128];
        snprintf(buf, sizeof(buf), " error head code = %" PRIu32 ", hex = %#02" PRIx8, head, (uint8_t)recvBuf.front());
        std::cerr << __FILE__ << ":" << __LINE__ << buf << std::endl;
      }
      recvBuf.pop_front(); // 逐个排查
      continue; // 继续解析
    }
    if (frameLen > maxFrameLen) { // 数据错误
      if (logcnt++ < 5) { // 打印信息
        char buf[128];
        snprintf(buf, sizeof(buf), " error head code = %" PRIu32 ", hex = %#02" PRIx8, head, (uint8_t)recvBuf.front());
        std::cerr << __FILE__ << ":" << __LINE__ << buf << std::endl;
      }
      recvBuf.pop_front(); // 逐个排查
      continue; // 继续解析
    }
    // 符合要求
    break;
  }

  if (logcnt) {
    std::cerr << __FILE__ << ":" << __LINE__ << "recover from head ERROR\n"
              << "now frame length = " << frameLen << ", has recved = " << recvBuf.size() << std::endl;
  }

  if (frameLen <= recvBuf.size()) { // 包含一张完整的图像
    if (! singleImageFrame)
      singleImageFrame.reset(new char [maxFrameLen]);
    for (unsigned k = 0; k < frameLen; ++k)
      singleImageFrame[k] = recvBuf[k];
    recvBuf.erase(recvBuf.begin(), recvBuf.begin() + frameLen);

    image.loadFromData(reinterpret_cast<const uchar *>(singleImageFrame.get() + 8), frameLen - 8, "JPG");
    emit sender_image(image);
//            std::cout << "emit an image" << std::endl;
  }
}
