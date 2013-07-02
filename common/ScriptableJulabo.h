#ifndef SCRIPTABLEJULABO_H
#define SCRIPTABLEJULABO_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <JulaboModel.h>

class ScriptableJulabo : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableJulabo(JulaboModel* julaboModel, QObject *parent = 0);

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
  JulaboModel* julaboModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEJULABO_H
