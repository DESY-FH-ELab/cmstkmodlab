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

#ifndef _VSCRIPTABLEDEVICE_H_
#define _VSCRIPTABLEDEVICE_H_

#include <QObject>
#include <QMutex>
#include <QScriptValue>

class VScriptableDevice : public QObject
{
  Q_OBJECT
  
public:
  explicit VScriptableDevice(QObject *parent = 0);

  virtual void abort() = 0;
};

#endif // _VSCRIPTABLEDEVICE_H_
