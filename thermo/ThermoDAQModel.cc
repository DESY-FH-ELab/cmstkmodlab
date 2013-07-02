#include <QDebug>

#include "ThermoDAQModel.h"

ThermoDAQModel::ThermoDAQModel(JulaboModel* julaboModel,
                               KeithleyModel* keithleyModel,
                               HamegModel* hamegModel,
                               PfeifferModel* pfeifferModel,
                               QObject *parent) :
    julaboModel_(julaboModel),
    keithleyModel_(keithleyModel),
    hamegModel_(hamegModel),
    pfeifferModel_(pfeifferModel)
{

}
