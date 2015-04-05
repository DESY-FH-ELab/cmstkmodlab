#ifndef THERMODAQSTREAMER_H
#define THERMODAQSTREAMER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDir>

#include <ThermoDAQModel.h>

class ThermoDAQStreamer : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQStreamer(ThermoDAQModel* model, QObject* parent=0);

    const QString& getFilename() const { return ofilename_; }
    
protected slots:
    void handleDAQMessage(const QString& message);
    void daqStateChanged(bool state);

protected:
  ThermoDAQModel* model_;
  bool isStreaming_;
  QString ofilename_;
  QFile* ofile_;
  QTextStream* stream_;
  QDir currentDir_;
};

#endif // THERMODAQSTREAMER_H
