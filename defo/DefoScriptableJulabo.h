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

  void setPID( double p, double tv, double td );
  void enableCirculator();
  void disableCirculator();
  void setWorkingTemperature( double temperature );

  QScriptValue circulator() const;
  QScriptValue workingTemperature() const;
  QScriptValue bath() const;
  QScriptValue safety() const;

signals:
  void changeP( double p );
  void changeTv( double tv );
  void changeTd( double td );
  void switchCirculator( bool );
  void changeWorkingTemperature( double temperature );

protected:
  DefoJulaboModel* julaboModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEJULABO_H
