/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2DAQSTREAMER_H
#define THERMO2DAQSTREAMER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDir>

#include <Thermo2DAQModel.h>

class Thermo2DAQStreamer : public QObject
{
  Q_OBJECT
public:
  explicit Thermo2DAQStreamer(Thermo2DAQModel* model, QObject* parent=0);

  const QString& getFilename() const { return ofilename_; }

protected slots:

  void handleDAQMessage(const QString& message);
  void daqStateChanged(bool state);

protected:

  Thermo2DAQModel* model_;
  bool isStreaming_;
  QString ofilename_;
  QFile* ofile_;
  QTextStream* stream_;
  QDir currentDir_;
};

#endif // THERMO2DAQSTREAMER_H
