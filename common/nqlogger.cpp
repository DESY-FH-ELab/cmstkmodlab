/****************************************************************************
 **
 **  Copyright (C) 2013 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <iostream>

#include <QDateTime>

#include "nqlogger.h"

NQLog::NQLog(const QString& module, LogLevel level)
    : module_(module),
      level_(level),
      stream_(&buffer_)
{

}

NQLog::~NQLog()
{
    NQLogger::instance()->write(module_, level_, buffer_);
}

NQLogger* NQLogger::instance_ = NULL;

NQLogger::NQLogger(QObject *parent) :
    QObject(parent)
{

}

NQLogger* NQLogger::instance(QObject *parent)
{
    if (instance_==NULL) {
        instance_ = new NQLogger(parent);
    }

    return instance_;
}

void NQLogger::write(const QString& module, NQLog::LogLevel level, const QString& buffer)
{
    if (activeModules_.empty()) return;

    bool filtered = true;
    for (std::set<std::pair<QString,bool> >::iterator it = activeModules_.begin();
         it!=activeModules_.end();
         ++it) {
        if (it->second==false) {
            if (module==it->first) {
                filtered = false;
                break;
            }
        } else {
            if (it->first.length()==0 || module.startsWith(it->first)) {
                filtered = false;
                break;
            }
        }
    }
    if (filtered) return;

    QString dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString message = dateString;

    switch (level) {
    case NQLog::Spam: message += " S"; break;
    case NQLog::Message: message += " M"; break;
    case NQLog::Warning: message += " W"; break;
    case NQLog::Critical: message += " C"; break;
    case NQLog::Fatal: message += " F"; break;
    }

    message += " [";
    message += module;
    message += "] ";
    message += buffer;
    message += "\n";

    for (std::vector<std::pair<NQLog::LogLevel,QTextStream*> >::iterator it = destinations_.begin();
         it!=destinations_.end();
         ++it) {
        if (level>=it->first) {
        QTextStream* stream = it->second;
        stream->operator <<(message);
        stream->flush();
        }
    }
}

void NQLogger::addActiveModule(const QString& module)
{
    if (module.endsWith("*")) {
        QString temp = module;
        temp.remove(temp.indexOf("*"), temp.length()-temp.indexOf("*"));
        activeModules_.insert(std::pair<QString,bool>(temp,true));
    }
    activeModules_.insert(std::pair<QString,bool>(module,false));
}

void NQLogger::addDestiniation(QIODevice * device, NQLog::LogLevel level)
{
    QTextStream* stream = new QTextStream(device);
    destinations_.push_back(std::pair<NQLog::LogLevel,QTextStream*>(level,stream));
}

void NQLogger::addDestiniation(FILE * fileHandle, NQLog::LogLevel level)
{
    QTextStream* stream = new QTextStream(fileHandle);
    destinations_.push_back(std::pair<NQLog::LogLevel,QTextStream*>(level,stream));
}
