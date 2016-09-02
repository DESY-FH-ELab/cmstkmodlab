#ifndef MICRODAQSTREAMER_H
#define MICRODAQSTREAMER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDir>

#include <MicroDAQModel.h>

class MicroDAQStreamer : public QObject
{
    Q_OBJECT
public:
    explicit MicroDAQStreamer(MicroDAQModel* model, QObject* parent=0);

    const QString& getFilename() const { return ofilename_; }

protected slots:
    void handleDAQMessage(const QString& message);
    void daqStateChanged(bool state);

protected:
  MicroDAQModel* model_;
  bool isStreaming_;
  QString ofilename_;
  QFile* ofile_;
  QTextStream* stream_;
  QDir currentDir_;
};

#endif // MICRODAQSTREAMER_H
