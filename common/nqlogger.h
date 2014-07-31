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

#ifndef NQLOGGER_H
#define NQLOGGER_H

#include <vector>
#include <set>
#include <string>

#include <QObject>
#include <QIODevice>
#include <QTextStream>

class NQLog
{
public:

    enum LogLevel {
        Spam        = 0,
        Message     = 1,
        Warning     = 2,
        Critical    = 3,
        Fatal       = 4
    };

    NQLog(const QString& module, LogLevel level = Message);
    ~NQLog();

    inline NQLog &operator<<(QChar t) { stream_ << '\'' << t << '\''; return *this;}
    inline NQLog &operator<<(char t) { stream_ << t; return *this; }

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    inline NQLog &operator<<(QBool t) { stream_ << (bool(t != 0) ? "true" : "false"); return *this; }
#endif
    inline NQLog &operator<<(bool t) { stream_ << (t ? "true" : "false"); return *this; }

    inline NQLog &operator<<(signed short t) { stream_ << t; return *this; }
    inline NQLog &operator<<(unsigned short t) { stream_ << t; return *this; }

    inline NQLog &operator<<(signed int t) { stream_ << t; return *this; }
    inline NQLog &operator<<(unsigned int t) { stream_ << t; return *this; }

    inline NQLog &operator<<(signed long t) { stream_ << t; return *this; }
    inline NQLog &operator<<(unsigned long t) { stream_ << t; return *this; }

    inline NQLog &operator<<(qint64 t) { stream_ << QString::number(t); return *this; }
    inline NQLog &operator<<(quint64 t) { stream_ << QString::number(t); return *this; }

    inline NQLog &operator<<(float t) { stream_ << t; return *this; }
    inline NQLog &operator<<(double t) { stream_ << t; return *this; }

    inline NQLog &operator<<(const char* t) { stream_ << QString::fromLatin1(t); return *this; }
    inline NQLog &operator<<(const std::string& t) { stream_ << QString::fromStdString(t); return *this; }
    inline NQLog &operator<<(const QString& t) { stream_ << '\"' << t << '\"'; return *this; }
    inline NQLog &operator<<(const QStringRef & t) { return operator<<(t.toString()); }
    inline NQLog &operator<<(const QLatin1String &t) { stream_ << '\"'  << t.latin1() << '\"'; return *this; }
    inline NQLog &operator<<(const QByteArray & t) { stream_  << '\"' << t << '\"'; return *this; }

    inline NQLog &operator<<(const void * t) { stream_ << t; return *this; }

    inline NQLog &operator<<(QTextStreamFunction f) { stream_ << f; return *this; }
    inline NQLog &operator<<(QTextStreamManipulator m) { stream_ << m; return *this; }

protected:

    QString module_;
    LogLevel level_;
    QString buffer_;
    QTextStream stream_;
};

class NQLogger : public QObject
{
    Q_OBJECT
public:

    static NQLogger* instance(QObject *parent = 0);

    void write(const QString& module, NQLog::LogLevel level, const QString&buffer);

    void addActiveModule(const QString& module);

    void addDestiniation(QIODevice * device, NQLog::LogLevel level = NQLog::Message);
    void addDestiniation(FILE * fileHandle, NQLog::LogLevel level = NQLog::Message);

protected:

    explicit NQLogger(QObject *parent = 0);
    static NQLogger* instance_;

    std::set<std::pair<QString,bool> > activeModules_;
    std::vector<std::pair<NQLog::LogLevel,QTextStream*> > destinations_;
};

#endif // NQLOGGER_H
