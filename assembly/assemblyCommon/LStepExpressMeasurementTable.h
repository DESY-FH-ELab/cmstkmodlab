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
  void update();

 protected:
  std::vector<std::vector<float> > Columns;
};

#endif // LSTEPEXPRESSMEASUREMENTTABLE_H 
