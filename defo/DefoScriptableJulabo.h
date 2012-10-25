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

  void setPID( double p, int tv, int td );
  void enableCirculator();
  void disableCirculator();
  void setWorkingTemperature( double temperature );

  QScriptValue circulator();
  QScriptValue workingTemperature();
  QScriptValue bath();
  QScriptValue safety();

signals:
  void changeP( double p );
  void changeTv( int tv );
  void changeTd( int td );
  void switchCirculator( bool );
  void changeWorkingTemperature( double temperature );

protected:
  DefoJulaboModel* julaboModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEJULABO_H
