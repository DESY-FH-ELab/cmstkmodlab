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

#ifndef APPLICATIONCONFIGITEMMODEL_H
#define APPLICATIONCONFIGITEMMODEL_H

#include <string>
#include <vector>

#include <QAbstractTableModel>

#include <ApplicationConfig.h>

class ApplicationConfigItemModel : public QAbstractTableModel
{
  Q_OBJECT
public:

  ApplicationConfigItemModel(bool readOnly = true, QObject *parent = 0);

  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:

  void setReadOnly(bool readOnly) { isReadOnly_ = readOnly; }

protected slots:

  void structureChanged();

signals:

  void keysChanged();

protected:

  bool isReadOnly_;
  std::vector<std::pair<ApplicationConfig::ShortKey,std::size_t> > keys_;
};

#endif // APPLICATIONCONFIGITEMMODEL_H
