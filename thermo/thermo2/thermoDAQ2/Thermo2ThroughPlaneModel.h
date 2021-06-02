/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2THROUGHPLANEMODEL_H
#define THERMO2THROUGHPLANEMODEL_H

#include <array>

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "HuberUnistat525wModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"

class Thermo2ThroughPlaneModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2ThroughPlaneModel(HuberUnistat525wModel* huberModel,
		  RohdeSchwarzNGE103BModel* nge103BModel,
		  KeithleyDAQ6510Model* keithleyModel,
		  QObject *parent = 0);

public slots:

protected slots:

  void huberInfoChanged();
  void nge103BInfoChanged();
  void keithleyInfoChanged();

protected:

  HuberUnistat525wModel* huberModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;

  QMutex mutex_;

  QDateTime currentTime_;

  template <typename T> bool updateIfChanged(T &variable, T newValue) {
    if (variable==newValue) return false;
    variable = newValue;
    return true;
  }

signals:

  void informationChanged();
};

#endif // THERMO2THROUGHPLANEMODEL_H
