/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef DBLOGGERMODEL_H
#define DBLOGGERMODEL_H

#include <iostream>

#include "nqlogger.h"

#include <QString>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QLocale>
#include <QTimer>
#include <QStringList>

class AssemblyDBLoggerModel : public QObject
{
    Q_OBJECT //Needed to be able to connect slots, etc.

    public:

        AssemblyDBLoggerModel(const QString);
        virtual ~AssemblyDBLoggerModel();
        QString getTimestamp(QString format="hh:mm:ss");
        void writeMessage(QString);
        const QString get_dblogfilepath() {return dblogfilepath_;}
        void setOverwritingMode() {overwrite_ = true;}

    protected:

        QString dblogfilepath_;
        QMutex mutex_;
        bool appendToEnd_;
        QTimer* timer_;
        bool reading_;
        bool overwrite_;
        uint last_read_endline_;
        int verbosity_;

        void writeLogHeader();

    public slots:

        void read_newlines();
        void setAppendMode(const QString&);

    signals:

        void refresh_content_view(const QStringList&);
        void overwrite_content_view();
};

#endif // DBLOGGERMODEL_H
