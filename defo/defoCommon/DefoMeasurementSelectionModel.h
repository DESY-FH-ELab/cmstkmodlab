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

#ifndef DEFOMEASUREMENTSELECTIONMODEL_H
#define DEFOMEASUREMENTSELECTIONMODEL_H

#include <QObject>
#include "DefoMeasurement.h"

class DefoMeasurementSelectionModel : public QObject
{
  Q_OBJECT
public:

  explicit DefoMeasurementSelectionModel(QObject *parent = 0);
  DefoMeasurement* getSelection();
  DefoMeasurement* getSelection() const;

public slots:

  void setSelection(DefoMeasurement* selection);

protected:

  DefoMeasurement* selection_;

signals:

  void selectionChanged(DefoMeasurement* selection);
};

class DefoMeasurementPairSelectionModel : public QObject
{
    Q_OBJECT
public:

  explicit DefoMeasurementPairSelectionModel(QObject *parent = 0);
  DefoMeasurementPair* getSelection() const;

public slots:

  void setSelection(DefoMeasurementPair* selection);

protected:

  DefoMeasurementPair* selection_;

signals:

  void selectionChanged(DefoMeasurementPair* selection);

};

#endif // DEFOMEASUREMENTSELECTIONMODEL_H
