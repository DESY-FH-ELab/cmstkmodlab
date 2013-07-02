#ifndef THERMODAQMODEL_H
#define THERMODAQMODEL_H

#include <QObject>

#include "JulaboModel.h"
#include "KeithleyModel.h"
#include "HamegModel.h"
#include "PfeifferModel.h"

class ThermoDAQModel : public QObject
{
    Q_OBJECT
public:
  explicit ThermoDAQModel(JulaboModel* julaboModel,
                              KeithleyModel* keithleyModel,
                              HamegModel* hamegModel,
                              PfeifferModel* pfeifferModel,
                              QObject *parent = 0);

protected slots:

protected:
  JulaboModel* julaboModel_;
  KeithleyModel* keithleyModel_;
  HamegModel* hamegModel_;
  PfeifferModel* pfeifferModel_;
};

#endif // THERMODAQMODEL_H
