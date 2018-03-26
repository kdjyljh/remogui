#ifndef NETIMAGE_HPP
#define NETIMAGE_HPP

#include <memory>
#include <deque>
#include <QWidget>
#include <QThread>
#include <QtNetwork/QtNetwork>

class NetImage : public QObject {
 Q_OBJECT
 public:
  explicit NetImage(QObject *parent = 0);

 public slots:
  void accept_connection();
  void read_assemble();
  void disconn_discard();
  void err_discard(QAbstractSocket::SocketError socketError);

 private:
  QTcpServer server;
  QTcpSocket *client;
  std::deque<char> recvBuf;
  std::unique_ptr<char []> singleImageFrame;
  QPixmap image;

 signals:
  void sender_image(QPixmap); // 发送图片
};

#endif // NETIMAGE_HPP
