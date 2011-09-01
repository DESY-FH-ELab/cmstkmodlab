#include <stdio.h>

#include <iostream>

#include <QTextStream>
#include <QString>
#include <QDir>

#include "labcontrolpreferences.h"

LabControlPreferences::LabControlPreferences()
{
    dataDirectory_ = "~";

    for (unsigned int i=0;i<10;i++) {
        calibration_[i] = std::make_pair<float,float>(1.0, 0.0);
    }
}

void LabControlPreferences::read()
{
    QString filename = QDir::homePath();
    filename += "/.labcontrolrc";

    FILE * ifile = fopen(filename.toStdString().c_str(), "r");
    if (!ifile) return;

    QTextStream istream(ifile, QIODevice::ReadOnly);
    QString line;

    std::cout << " [LabControl::Preferences] -- \n"
            << "  reading " << filename.toStdString() << std::endl;

    while (!istream.atEnd()) {
        line = istream.readLine();

        if (line.startsWith("<DATADIR>")) {
            line.remove("<DATADIR>");
            line.replace("~", QDir::homePath());
            line = line.simplified();
            dataDirectory_ = line.toStdString();
            std::cout << " [LabControl::Preferences] -- \n"
                    << "  setting data directory to " << dataDirectory_ << std::endl;
        }

        if (line.startsWith("<CAL>")) {
            line.remove("<CAL>").simplified();
            int Channel;
            float Slope, Offset;
            QTextStream ts(&line, QIODevice::ReadOnly);
            ts >> Channel >> Slope >> Offset;
            if (Channel<0 || Channel>9) continue;

            calibration_[Channel].first = Slope;
            calibration_[Channel].second = Offset;

            std::cout << " [LabControl::Preferences] -- \n"
                    << "  setting calibration for channel "
                    << Channel << " to "
                    << calibration_[Channel].first  << " "
                    << calibration_[Channel].second << std::endl;
        }
    }
}

const std::pair<float,float> LabControlPreferences::calibration(unsigned int channel) const
{
    if (channel>9) return std::make_pair<float,float>(1.0, 0.0);
    return calibration_[channel];
}
