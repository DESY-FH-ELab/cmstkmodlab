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

#ifndef DBLOGGERCONTROLLER_H
#define DBLOGGERCONTROLLER_H

#include <iostream>

#include "nqlogger.h"

#include "AssemblyDBLoggerModel.h"
#include "AssemblyDBLoggerView.h"

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

class AssemblyDBLoggerController : public QObject
{
    Q_OBJECT //Needed to be able to connect slots, etc.

    public:

        AssemblyDBLoggerController(AssemblyDBLoggerModel*, AssemblyDBLoggerView*);
        virtual ~AssemblyDBLoggerController();
        // static AssemblyDBLoggerController* instance(AssemblyDBLoggerModel*, AssemblyDBLoggerView*);
        // static AssemblyDBLoggerController* instance();
        void writeMessage(QString);

    protected:

        // static AssemblyDBLoggerController* instance_;
        AssemblyDBLoggerModel* model_;
        AssemblyDBLoggerView* view_;
        void connect_all();
        const QString get_dblogfilepath();

    public slots:

        void copy_existing_logfile(const QString);
        void copy_current_logfile(const QString);
        void writeMessageToLog(QString);

    signals:

};

#endif // DBLOGGERCONTROLLER_H
