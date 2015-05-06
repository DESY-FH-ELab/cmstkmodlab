#ifndef DEFOSCRIPTABLEJULABO_H
#define DEFOSCRIPTABLEJULABO_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "DefoJulaboModel.h"

class DefoScriptableJulabo :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableJulabo(
      DefoJulaboModel* julaboModel
    , QObject *parent = 0
  );

public slots:

  void setP( double p );
  void setI( int tv );
  void setD( int td );
  void setPID( double p, int tv, int td );
  QScriptValue getP();
  QScriptValue getI();
  QScriptValue getD();

  void enableCirculator();
  void disableCirculator();
  QScriptValue circulatorState();

  void setWorkingTemperature( double temperature );

  QScriptValue workingTemperature();
  QScriptValue bath();
  QScriptValue safety();

  void setPumpPressure(unsigned int pressure);
  QScriptValue pumpPressure();

signals:
  void changeP( double p );
  void changeTv( int tv );
  void changeTd( int td );
  void switchCirculator( bool );
  void changeWorkingTemperature( double temperature );
  void changePumpPressure( unsigned int pressure );

protected:
  DefoJulaboModel* julaboModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEJULABO_H
