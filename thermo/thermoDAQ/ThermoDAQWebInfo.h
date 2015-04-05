#ifndef THERMODAQWEBINFO_H
#define THERMODAQWEBINFO_H

#include <QObject>
#include <QTimer>

#include <ThermoDAQModel.h>

class ThermoDAQWebInfo : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQWebInfo(ThermoDAQModel* model, QObject* parent=0);

protected slots:

    void updateWebInfo();
    void daqStateChanged(bool state);

protected:
  ThermoDAQModel* model_;
  QTimer executionTimer_;
  QString htmlSource_;
  QString lastUpdate_;
  QString htmlFile_;
};

#endif // THERMODAQWEBINFO_H
