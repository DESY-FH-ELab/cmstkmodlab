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

#ifndef THERMODAQWEBINFO_H
#define THERMODAQWEBINFO_H

#include <QObject>
#include <QTimer>

#include <ThermoDAQModel.h>

class ThermoDAQWebInfo : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQWebInfo(ThermoDAQModel* model, QObject* parent=0);

protected slots:

    void updateWebInfo();
    void daqStateChanged(bool state);

protected:
  ThermoDAQModel* model_;
  QTimer executionTimer_;
  QString htmlSource_;
  QString lastUpdate_;
  QString htmlFile_;
};

#endif // THERMODAQWEBINFO_H
