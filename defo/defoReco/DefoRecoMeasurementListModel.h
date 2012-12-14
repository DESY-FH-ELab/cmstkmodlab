#ifndef _DEFORECOMEASUREMENTLISTMODEL_H
#define _DEFORECOMEASUREMENTLISTMODEL_H

#include "DefoMeasurementListModel.h"

class DefoRecoMeasurementListModel : public DefoMeasurementListModel
{
    Q_OBJECT

public:
  explicit DefoRecoMeasurementListModel(QObject *parent = 0);

  void write(const QDir& path);
  void read(const QString& filename);
};

#endif // _DEFORECOMEASUREMENTLISTMODEL_H
