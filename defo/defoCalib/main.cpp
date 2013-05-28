#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QString>
#include <QDir>

#include <DefoPoint.h>
#include <DefoRecoSurface.h>
#include <DefoSurface.h>

class image {
public:
    std::string timestamp;
    float focalLength;
    std::string comment;
    int amplitude;
    bool hasPoints;
    DefoPointCollection points;
};

class measurementSet {
public:
    double amplitude;
    std::string undeformed;
    std::string deformed;
    DefoSurface surface;
};

bool readPoints(std::string& filename, image& i) {

    i.points.clear();

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader stream(&file);

    while (!stream.atEnd()) {
        stream.readNextStartElement();
        if (stream.isStartElement() && stream.name()=="DefoPoint") {
            double x = stream.attributes().value("x").toString().toDouble();
            double y = stream.attributes().value("y").toString().toDouble();
            float H = stream.attributes().value("H").toString().toFloat();
            float S = stream.attributes().value("S").toString().toFloat();
            float V = stream.attributes().value("V").toString().toFloat();
            DefoPoint p(x, y);
            QColor c;
            c.setHsvF(H, S, V);
            p.setColor(c);

            p.setValid(true);

            bool indexed = (stream.attributes().value("indexed").toString().toInt()==1);
            p.unindex();
            if (indexed) {
              int ix = stream.attributes().value("ix").toString().toInt();
              int iy = stream.attributes().value("iy").toString().toInt();
              p.setIndex(ix, iy);
            }

            i.points.push_back(p);
        }
    }

    return true;
}

bool parseImage(std::string& dataPath, image& i) {

    std::string filename = dataPath + "/";
    filename += i.timestamp;
    filename += ".xml";

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader stream(&file);

    while (!stream.atEnd()) {
        stream.readNextStartElement();
        if (stream.isStartElement() && stream.name()=="DefoMeasurement") {
            std::string timestamp = stream.attributes().value("timestamp").toString().toStdString();
        }

        if (stream.isStartElement() && stream.name()=="Comment") {
            i.comment = stream.readElementText().toStdString();
        }

        if (stream.isStartElement() && stream.name()=="Calibration") {
            i.amplitude = stream.attributes().value("amplitude").toString().toInt();
        }

        if (stream.isStartElement() && stream.name()=="FocalLength") {
            i.focalLength = stream.attributes().value("value").toString().toFloat();
        }
    }

    QDir dir(dataPath.c_str());
    filename = "offlinePoints_";
    filename += i.timestamp;
    filename += ".xml";
    if (dir.exists(filename.c_str())) {
        i.hasPoints = true;
    } else {
        i.hasPoints = false;
    }

    if (i.hasPoints) {
        filename = dataPath + "/";
        filename += "offlinePoints_";
        filename += i.timestamp;
        filename += ".xml";
        readPoints(filename, i);
    }

    return true;
}

bool parseODMX(std::string& dataPath, std::map<std::string,image>& images) {

    std::string filename = dataPath + "/measurements.odmx";
    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader stream(&file);

    while (!stream.atEnd()) {
        stream.readNextStartElement();
        if (stream.isStartElement() && stream.name()=="DefoMeasurement") {
            std::string timestamp = stream.attributes().value("timestamp").toString().toStdString();
            image i;
            i.timestamp = timestamp;

            if (!parseImage(dataPath, i)) return false;

            images[timestamp] = i;
        }
    }

    return true;
}

void dump(std::map<std::string,image>& images) {
    for (std::map<std::string,image>::iterator it = images.begin();
         it!=images.end();
         ++it) {
        image& i = it->second;
        if (!i.hasPoints) continue;
        std::cout << "image: " << it->first << std::endl;
        std::cout << "  amplitude:        " << i.amplitude << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if ((argc==1 || argc>3) || (argc==3 && std::string(argv[2])!="dump")) {
        std::cout << "usage: defoCalib steering_file [dump]" << std::endl;
        return -1;
    }

    std::ifstream ifile(argv[1]);
    if (!ifile.is_open() || !ifile.good()) {
        std::cout << "steering file could not be opened" << std::endl;
        return -1;
    }

    std::string line;
    std::string key;
    std::string dataPath;
    std::vector<measurementSet> measurementSets;
    std::map<std::string,image> images;
    std::map<std::string,std::string> uniqueImages;

    while (std::getline(ifile, line)) {
        std::istringstream iss(line);
        iss >> key;

        if (key=="DATA") {
            iss >> dataPath;
        } else if (key=="MEASUREMENT") {
            measurementSet m;
            iss >> m.undeformed;
            iss >> m.deformed;
            measurementSets.push_back(m);
            uniqueImages[m.undeformed] = m.undeformed;
            uniqueImages[m.deformed] = m.deformed;
        }
    }

    if (!parseODMX(dataPath, images)) {
        std::cout << "ODMX file could not be parsed!" << std::endl;
        return -1;
    }

    std::cout << "datapath: " << dataPath << std::endl;

    if (argc==3) {
        dump(images);
        return 0;
    }

    DefoRecoSurface reco;

    reco.setPitchX(22.3/5184.); // mm/pixel
    reco.setPitchY(14.9/3456.); // mm/pixel

    reco.setNominalGridDistance(1.802*1000.);
    reco.setNominalCameraDistance(1.822*1000.);

    std::cout << "datapath: " << dataPath << std::endl;
    std::cout << "measurements:" << std::endl;
    for (std::vector<measurementSet>::iterator it = measurementSets.begin();
         it!=measurementSets.end();
         ++it) {

        image& undeformed = images[it->undeformed];
        image& deformed = images[it->deformed];

        it->amplitude = deformed.amplitude - undeformed.amplitude;

        std::cout << it->amplitude << "um\t" << it->undeformed << "\t" << it->deformed << std::endl;

        reco.setFocalLength(undeformed.focalLength);

        it->surface = reco.reconstruct(undeformed.points, deformed.points);

        std::string filename = "defoDump_";
        filename += it->undeformed;
        filename += "_";
        filename += it->deformed;
        filename += ".txt";

        it->surface.dumpSplineField(filename);

        filename = "defoStats_";
        filename += it->undeformed;
        filename += "_";
        filename += it->deformed;
        filename += ".txt";

        it->surface.dumpStats(filename);
    }

    return 0;
}
