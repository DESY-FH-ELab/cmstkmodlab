#ifndef MICROSCRIPTMODEL_H
#define MICROSCRIPTMODEL_H

#include <QObject>
#include <QTimer>
#include <QTextDocument>

#include "MicroDAQModel.h"
#include "IotaModel.h"
#include "ArduinoPresModel.h"

#include "MicroScriptThread.h"

class MicroScriptModel : public QObject
{
    Q_OBJECT
public:

    explicit MicroScriptModel(MicroDAQModel* daqModel,
                              IotaModel* iotaModel,
                              ArduinoPresModel* arduinoPresModel,
                              QObject *parent = 0);

    QTextDocument* scriptDocument() { return script_; }
    const QString& currentScriptFilename() const { return currentScriptFilename_; }

    void openScript(const QString filename);
    void saveScript(const QString filename);
    void executeScript();
    void abortScript();
    int executionTime() const { return executionTime_; }

    void startMeasurement();
    void stopMeasurement();
    void message(int value);
    void message(uint value);
    void message(double value);
    void message(const QString & text);
    void log(const QString & text);

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
    MicroDAQModel* daqModel_;
    IotaModel* iotaModel_;
    ArduinoPresModel* arduinoPresModel_;
    MicroScriptThread* scriptThread_;

signals:
  void prepareNewMeasurement();
  void scriptLoaded();
  void setControlsEnabled(bool enabled);
  void clearMessageText();
  void appendMessageText(const QString & text);
};

#endif // MICROSCRIPTMODEL_H
