#include <iostream>

#include <QDateTime>

#include "ApplicationConfig.h"
#include "MicroDAQStreamer.h"
#include "MicroDAQModel.h"

MicroDAQStreamer::MicroDAQStreamer(MicroDAQModel* model, QObject* parent) :
    QObject(parent),
    model_(model),
    isStreaming_(false),
    ofile_(0),
    stream_(0)
{
    connect(model_, SIGNAL(daqStateChanged(bool)),
            this, SLOT(daqStateChanged(bool)));
    connect(model_, SIGNAL(daqMessage(QString)),
            this, SLOT(handleDAQMessage(QString)));
}

void MicroDAQStreamer::handleDAQMessage(const QString& message)
{
    QString buffer = message;
    if (buffer.startsWith("\n")) {
        buffer.remove(0, 1);
    }

    if (isStreaming_ && buffer.length()>0) {
        *stream_ << buffer << "\n";
        stream_->flush();
    }
}

void MicroDAQStreamer::daqStateChanged(bool state)
{
    if (state==true) {
        QDateTime dt = QDateTime::currentDateTime().toUTC();

        ApplicationConfig* config = ApplicationConfig::instance();
        QString dataPath(config->getValue<std::string>("DataPath").c_str());

        QString measurementDirPath(dataPath + "/%1");
        currentDir_.setPath(measurementDirPath.arg(dt.toString("yyyyMMdd")));
        if (!currentDir_.exists())
            currentDir_.mkpath(currentDir_.absolutePath());

        QString filename("%1-%2.xml");
        filename = filename.arg(dt.toString("yyyyMMdd"));
        int i = 1;
        while (currentDir_.exists(filename.arg(i))) ++i;
        filename = filename.arg(i);

        ofile_ = new QFile(currentDir_.absoluteFilePath(filename));
        if (ofile_->open(QFile::WriteOnly | QFile::Truncate)) {
            stream_ = new QTextStream(ofile_);
        }
    } else {
        stream_->device()->close();
        delete ofile_;
        delete stream_;
    }

    isStreaming_ = state;
}
