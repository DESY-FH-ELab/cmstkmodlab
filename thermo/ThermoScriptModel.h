#ifndef THERMOSCRIPTMODEL_H
#define THERMOSCRIPTMODEL_H

#include <QObject>
#include <QTimer>
#include <QTextDocument>

#include "JulaboModel.h"
#include "KeithleyModel.h"
#include "HamegModel.h"
#include "PfeifferModel.h"

#include "ThermoScriptThread.h"

class ThermoScriptModel : public QObject
{
    Q_OBJECT
public:

  explicit ThermoScriptModel(JulaboModel* julaboModel,
                             KeithleyModel* keithleyModel,
                             HamegModel* hamegModel,
                             PfeifferModel* pfeifferModel,
                             QObject *parent = 0);

  QTextDocument* scriptDocument() { return script_; }
  const QString& currentScriptFilename() const { return currentScriptFilename_; }

  void openScript(const QString filename);
  void saveScript(const QString filename);
  void executeScript();
  void abortScript();
  int executionTime() const { return executionTime_; }

  void message(int value);
  void message(uint value);
  void message(double value);
  void message(const QString & text);

public slots:
  void executionStarted();
  void executionFinished();
  void doClearMessageText();
  void doAppendMessageText(const QString & text);

protected slots:
  void executionHeartBeat();

protected:

  QTextDocument* script_;
  QString currentScriptFilename_;

  QTimer executionTimer_;
  int executionTime_;
  JulaboModel* julaboModel_;
  KeithleyModel* keithleyModel_;
  HamegModel* hamegModel_;
  PfeifferModel* pfeifferModel_;
  ThermoScriptThread* scriptThread_;

signals:
  void prepareNewMeasurement();
  void scriptLoaded();
  void setControlsEnabled(bool enabled);
  void clearMessageText();
  void appendMessageText(const QString & text);
};

#endif // THERMOSCRIPTMODEL_H
