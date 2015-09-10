#ifndef DEFOCAMERAOPTIONCOMBOBOX_H
#define DEFOCAMERAOPTIONCOMBOBOX_H

#include <QComboBox>
#include "DefoCameraModel.h"

class DefoCameraOptionComboBox : public QComboBox
{
    Q_OBJECT
public:
  explicit DefoCameraOptionComboBox(
      DefoCameraModel* model
    , const DefoCameraModel::Option& option
// TODO   , const QString* label
    , QWidget* parent = 0
  );

protected:
  DefoCameraModel* model_;
  const DefoCameraModel::Option option_;

protected slots:
  void selectionChanged(int index);
  void deviceStateChanged(State newState);
  void controlStateChanged(bool);
  void deviceOptionChanged(DefoCameraModel::Option option, int value);

};

#endif // DEFOCAMERAOPTIONCOMBOBOX_H
