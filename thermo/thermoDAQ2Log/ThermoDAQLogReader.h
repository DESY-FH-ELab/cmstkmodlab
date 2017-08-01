/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODAQSTREAMREADER_H
#define THERMODAQSTREAMREADER_H

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

class ThermoDAQLogReader : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQLogReader(QStringList arguments, QObject* parent);

public slots:
    void run();

signals:
    void finished();

protected:
    QStringList arguments_;
    void process();
    void processLine(QString& line);

    void processLog(QXmlStreamReader& xml);
};

#endif // THERMODAQSTREAMREADER_H
