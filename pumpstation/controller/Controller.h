#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStringList>
#include <QObject>
#include <QTcpSocket>

class Controller : public QObject
{
  Q_OBJECT

public:

  Controller(QStringList& arguments);

private slots:

  void connectToServer();
  void sendCommand();
  void readResponse();
  void reportError(QAbstractSocket::SocketError socketError);

private:

  QStringList arguments_;
  QTcpSocket *socket_;
  QString ipAddress;
};

#endif
