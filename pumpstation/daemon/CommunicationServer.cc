#include <iostream>

#include <QMutexLocker>

#include <nqlogger.h>
#include "ApplicationConfig.h"

#include "CommunicationServer.h"

CommunicationServer::CommunicationServer(PumpStationModel* model,
                                         QObject *parent)
 : QTcpServer(parent),
   model_(model)
{
  connect(this, SIGNAL(setSwitchBlocked(int, bool)),
          model_, SLOT(setSwitchBlocked(int, bool)));

  connect(this, SIGNAL(setSwitchEnabled(int, bool)),
          model_, SLOT(setSwitchEnabled(int, bool)));
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
void CommunicationServer::incomingConnection(int socketDescriptor)
{
  NQLogDebug("server") << "void CommunicationServer::incomingConnection(int socketDescriptor)";
#else
  void CommunicationServer::incomingConnection(qintptr socketDescriptor)
  {
    NQLogDebug("server") << "void CommunicationServer::incomingConnection(qintptr socketDescriptor)";
#endif

  socket_ = new QTcpSocket();

  connect(socket_, SIGNAL(readyRead()),
          this, SLOT(handleCommand()));
  connect(socket_, SIGNAL(disconnected()),
          socket_, SLOT(deleteLater()));

  if (!socket_->setSocketDescriptor(socketDescriptor)) {
    // emit error(tcpSocket->error());
    return;
  }
}

void CommunicationServer::handleCommand()
{
  NQLogDebug("server") << "void CommunicationServer::handleCommand()";

  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_4_0);

  quint16 blockSize = 0;
  in >> blockSize;

  QString command;
  in >> command;

  NQLogDebug("server") << "command: (" << blockSize << ") |" << command.toStdString() << "|";

  QStringList args = command.split(" ");
  QString cmd = args.at(0);
  args.removeAt(0);
  QString response;

  if (cmd=="setSwitchState") {
    if (args.count()!=2) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();
      int state = args.at(1).toInt();

      if (channel<0 || channel>4) {
        response = "ERR";
      } else {
        emit setSwitchEnabled(channel, state);
        response = "OK";
      }
    }
  } else if (cmd=="setSwitchBlocked") {
    if (args.count()!=2) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();
      int state = args.at(1).toInt();

      if (channel<0 || channel>4) {
        response = "ERR";
      } else {
        emit setSwitchBlocked(channel, state);
        response = "OK";
      }
    }
  } else if (cmd=="getSwitchState") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();

      if (channel<0 || channel>4) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        State state = model_->getSwitchState(channel);
        response = QString::number((int)state);
      }
    }
  } else if (cmd=="getSwitchBlocked") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int channel = args.at(0).toInt();

      if (channel<0 || channel>4) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        bool state = model_->getSwitchBlocked(channel);
        response = QString::number((int)state);
      }
    }
  } else if (cmd=="getSwitchStatus") {
    if (args.count()!=0) {
      response = "ERR";
    } else {

      QMutexLocker locker(&mutex_);

      int s0 = model_->getSwitchState(0);
      int s1 = model_->getSwitchState(1);
      int s2 = model_->getSwitchState(2);
      int s3 = model_->getSwitchState(3);
      int s4 = model_->getSwitchState(4);

      response = QString("%1;%2;%3;%4;%5")
                .arg(s0)
                .arg(s1)
                .arg(s2)
                .arg(s3)
                .arg(s4);
     }
  } else if (cmd=="getSwitchBlockedStatus") {
    if (args.count()!=0) {
      response = "ERR";
    } else {

      QMutexLocker locker(&mutex_);

      int s0 = model_->getSwitchBlocked(0);
      int s1 = model_->getSwitchBlocked(1);
      int s2 = model_->getSwitchBlocked(2);
      int s3 = model_->getSwitchBlocked(3);
      int s4 = model_->getSwitchBlocked(4);

      response = QString("%1;%2;%3;%4;%5")
                .arg(s0)
                .arg(s1)
                .arg(s2)
                .arg(s3)
                .arg(s4);
     }
  } else if (cmd=="getSensorStatus") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int sensor = args.at(0).toInt();

      if (sensor<1 || sensor>3) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        int status = model_->getSensorStatus(sensor);
        response = QString::number(status);
      }
    }
  } else if (cmd=="getPressure") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int sensor = args.at(0).toInt();

      if (sensor<1 || sensor>3) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        double pressure = model_->getPressure(sensor);
        response = QString("%1").arg(pressure, 0, 'f', 1);
      }
    }
  } else if (cmd=="getVacuumStatus") {
    if (args.count()!=0) {
      response = "ERR";
    } else {

      QMutexLocker locker(&mutex_);

      int s1 = model_->getSensorStatus(1);
      int s2 = model_->getSensorStatus(2);
      int s3 = model_->getSensorStatus(3);

      double p1 = model_->getPressure(1);
      double p2 = model_->getPressure(2);
      double p3 = model_->getPressure(3);

      response = QString("%1;%2;%3;%4;%5;%6")
              .arg(s1).arg(p1, 0, 'f', 1)
              .arg(s2).arg(p2, 0, 'f', 1)
              .arg(s3).arg(p3, 0, 'f', 1);
    }
  } else {
    response = "ERR";
  }

  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << (quint16)response.length();
  out << response;

  socket_->write(block);
  socket_->disconnectFromHost();
  socket_->waitForDisconnected();
}
