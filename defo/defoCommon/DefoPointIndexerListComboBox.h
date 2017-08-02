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

#ifndef DEFOPOINTINDEXERLISTCOMBOBOX_H
#define DEFOPOINTINDEXERLISTCOMBOBOX_H

#include <map>

#include <QComboBox>
#include "DefoPointIndexerModel.h"
#include "DefoVPointIndexer.h"

class DefoPointIndexerListComboBox : public QComboBox
{
    Q_OBJECT
public:
  explicit DefoPointIndexerListComboBox(
      DefoPointIndexerModel* model
    , QWidget *parent = 0
  );

protected:
  static const QString LABEL_FORMAT;

  DefoPointIndexerModel* model_;

  /**
    Mapping of the measurements in the list to the indices in the combo box.
    Mapping should be one to one, i.e. index in the combobox is the same in the
    list model.
    */
  typedef std::map<const DefoVPointIndexer *, int> IndexerMap;
  IndexerMap indexMap_;

protected slots:
  void fillOptions(int count);
  void setSelection(DefoVPointIndexer* indexer);
  void selectionChanged(int index);

signals:

};

#endif // DEFOMEASUREMENTLISTCOMBOBOX_H
