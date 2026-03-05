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

#ifndef LSTEPEXPRESSMEASUREMENTTABLE_H
#define LSTEPEXPRESSMEASUREMENTTABLE_H

#include <vector>
#include <iostream>
#include <sstream>

#include <Qt>
#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidget>
#include <QStringList>
#include <QTableWidgetItem>
#include <QAbstractTableModel>
#include <QTableView>
#include <QModelIndex>


class LStepExpressMeasurementTable : public QAbstractTableModel
{
  Q_OBJECT

 public:
  LStepExpressMeasurementTable();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  void insertData(int column_index, std::vector<float> values);
  void insertData(int column_index, int cell_index, float value);
  void update();

 protected:
  std::vector<std::vector<float> > Columns;
};

#endif // LSTEPEXPRESSMEASUREMENTTABLE_H 
