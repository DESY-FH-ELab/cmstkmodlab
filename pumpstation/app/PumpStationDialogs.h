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

#ifndef PUMPSTATIONDIALOGS_H
#define PUMPSTATIONDIALOGS_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>

class PumpStationDialog : public QDialog
{
  Q_OBJECT
public:

  PumpStationDialog(const QString & message, QWidget* parent = 0);
};

class PumpStationPINDialog : public QDialog
{
  Q_OBJECT
public:

  PumpStationPINDialog(const QString & message, QWidget* parent = 0);

  const QString getPIN();

protected:

  QLineEdit * pinLabel_;

protected slots:

  void addNumber(int);
  void removeNumber();
};

#endif // DEFOGEOMETRYWIDGET_H
