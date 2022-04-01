/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QtCore>
#include <QtNetwork>
#include <QHostInfo>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "Controller.h"

Controller::Controller(const QString& command, const QStringList& parameters)
 : command_(command),
   parameters_(parameters)
{
  ipAddress_ = ApplicationConfig::instance()->getValue<std::string>("main", "CommServerIP").c_str();
  if (ipAddress_.isEmpty()) {
    QHostInfo hostinfo = QHostInfo::fromName(ApplicationConfig::instance()->getValue<std::string>("main", "CommServerHostname").c_str());
    if (!hostinfo.addresses().isEmpty()) {
      QHostAddress address = hostinfo.addresses().first();
      // use the first IP address

      ipAddress_ = address.toString();
    } else {
      ipAddress_ = QHostAddress(QHostAddress::LocalHost).toString();
    }
  }

  port_ = ApplicationConfig::instance()->getValue<unsigned int>("main", "CommServerPort");

  NQLogDebug("Controller") << ipAddress_ << ":" << port_;

  socket_ = new QTcpSocket(this);

  connect(socket_, SIGNAL(connected()), this, SLOT(sendCommand()));
  connect(socket_, SIGNAL(readyRead()), this, SLOT(readResponse()));
}

void Controller::connectToServer()
{
  NQLogDebug("Controller") << "void Controller::connectToServer()";

  socket_->abort();
  socket_->connectToHost(ipAddress_, port_);

  if (!socket_->waitForConnected(500)) {
    NQLogDebug("Controller") << "The following error occurred: " << socket_->errorString().toStdString();

    std::cout << "ERR" << std::endl;

    socket_->close();
    socket_->deleteLater();

    QCoreApplication::quit();
  }
}

void Controller::sendCommand()
{
  NQLogDebug("Controller") << "void Controller::sendCommand()";

  QString buffer = "COM ";
  buffer += command_;
  buffer += " ";
  for (int i=0; i<parameters_.size(); ++i) {
    if (i>0) buffer += " ";
    buffer += parameters_.at(i);
  }
  
  NQLogDebug("Controller") << "command (" << buffer.length() << ") = |" << buffer.toStdString() << "|";

  QTextStream os(socket_);
  os.setAutoDetectUnicode(true);

  os << buffer << "\r\n";
  
  socket_->flush();
}

void Controller::readResponse()
{
  NQLogDebug("Controller") << "void Controller::readResponse()";
  
  QString response;
  if (socket_->canReadLine()) {
    response = socket_->readLine();
  }

  std::cout << response.toStdString();

  socket_->close();

  QCoreApplication::quit();
}
