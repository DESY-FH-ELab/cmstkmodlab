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
	ApplicationConfig * config = ApplicationConfig::instance();

  pumpChannels_ = config->getValueVector<int>("PumpSwitches");
  valveChannels_ = config->getValueVector<int>("ValveSwitches");

  connect(this, SIGNAL(setSwitchBlocked(int, bool)),
          model_, SLOT(setSwitchBlocked(int, bool)));

  connect(this, SIGNAL(setSwitchEnabled(int, bool)),
          model_, SLOT(setSwitchEnabled(int, bool)));

  connect(this, SIGNAL(setPumpOperatingHours(int, double)),
          model_, SLOT(setPumpOperatingHours(int, double)));
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
void CommunicationServer::incomingConnection(int socketDescriptor)
{
  NQLogDebug("CommunicationServer") << "void CommunicationServer::incomingConnection(int socketDescriptor)";
#else
  void CommunicationServer::incomingConnection(qintptr socketDescriptor)
  {
    NQLogDebug("CommunicationServer") << "void CommunicationServer::incomingConnection(qintptr socketDescriptor)";
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
  NQLogDebug("CommunicationServer") << "void CommunicationServer::handleCommand()";

  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_4_0);

  quint16 blockSize = 0;
  in >> blockSize;

  QString command;
  in >> command;

  NQLogDebug("CommunicationServer") << "command: (" << blockSize << ") |" << command.toStdString() << "|";

  QStringList args = command.split(" ");
  QString cmd = args.at(0);
  args.removeAt(0);
  QString response;

  if (cmd=="setPumpState") {
  	if (args.count()!=2) {
  		response = "ERR";
  	} else {
  		int pump = args.at(0).toInt();
  		int state = args.at(1).toInt();

  		if (pump<1 || pump>2) {
  			response = "ERR";
  		} else {
  			emit setSwitchEnabled(pumpChannels_[pump-1], state);
  			response = "OK";
  		}
  	}
  } else if (cmd=="getPumpState") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int pump = args.at(0).toInt();

      if (pump<1 || pump>2) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        State state = model_->getSwitchState(pumpChannels_[pump-1]);
        response = QString::number((int)state);
      }
    }
  } else if (cmd=="setPumpOperatingHours") {
    if (args.count()!=2) {
      response = "ERR";
    } else {
      int pump = args.at(0).toInt();
      double value = args.at(1).toDouble();

      if (pump<1 || pump>2) {
        response = "ERR";
      } else {
      	emit setPumpOperatingHours(pump, value);
      	response = "OK";
      }
    }
  } else if (cmd=="getPumpOperatingHours") {
    if (args.count()!=0) {
      response = "ERR";
    } else {
      QMutexLocker locker(&mutex_);
      double value1 = model_->getPumpOperatingHours(1);
      double value2 = model_->getPumpOperatingHours(2);
      response = QString("%1;%2").arg(value1, 'f', 6).arg(value2, 'f', 6);
    }
  } else if (cmd=="setValveState") {
  	if (args.count()!=2) {
  		response = "ERR";
  	} else {
  		int valve = args.at(0).toInt();
  		int state = args.at(1).toInt();

  		if (valve<1 || valve>3) {
  			response = "ERR";
  		} else {
  			emit setSwitchEnabled(valveChannels_[valve-1], state);
  			response = "OK";
  		}
  	}
  } else if (cmd=="getValveState") {
    if (args.count()!=1) {
      response = "ERR";
    } else {
      int valve = args.at(0).toInt();

      if (valve<1 || valve>3) {
        response = "ERR";
      } else {
        QMutexLocker locker(&mutex_);
        State state = model_->getSwitchState(valveChannels_[valve-1]);
        response = QString::number((int)state);
      }
    }
  } else if (cmd=="setSwitchState") {
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
        response = QString("%1").arg(pressure, 0, 'E', 3);
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
              .arg(s1).arg(p1, 0, 'E', 3)
              .arg(s2).arg(p2, 0, 'E', 3)
              .arg(s3).arg(p3, 0, 'E', 3);
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
