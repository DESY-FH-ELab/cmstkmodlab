#include <iostream>

#include <QDateTime>

#include "ApplicationConfig.h"
#include "DefoDAQStreamer.h"
#include "DefoDAQModel.h"

DefoDAQStreamer::DefoDAQStreamer(DefoDAQModel* model, QObject* parent)
  : QObject(parent),
    model_(model),
    isStreaming_(false),
    ofile_(0),
    stream_(0)
{
  connect(model_, SIGNAL(daqMessage(QString)),
	  this, SLOT(handleDAQMessage(QString)));
}

void DefoDAQStreamer::handleDAQMessage(const QString& message)
{
  QString buffer = message;
  if (buffer.startsWith("\n")) {
    buffer.remove(0, 1);
  }

  if (stream_ && isStreaming_ && buffer.length()>0) {
    *stream_ << buffer << "\n";
    stream_->flush();
  }
}

void DefoDAQStreamer::startDAQ(const QString& ofilename)
{
  if (isStreaming_) {
    stream_->device()->close();
    delete ofile_;
    delete stream_;
  }
  
  ofilename_ = ofilename;
  ofile_ = new QFile(ofilename_);
  if (ofile_->open(QFile::WriteOnly | QFile::Truncate)) {
    stream_ = new QTextStream(ofile_);
    if (stream_) isStreaming_ = true;
  }

  QString statusMessage;
  model_->createDAQStatusMessage(statusMessage);
  handleDAQMessage(statusMessage);
}
