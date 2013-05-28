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

class image {
public:
    std::string timestamp;
    std::string comment;
    int amplitude;
    bool hasPoints;
};

class measurementSet {
public:
    double amplitude;
    std::string undeformed;
    std::string deformed;
};

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

            if (stream.isStartElement() && stream.name()=="Comment") {
                i.comment = stream.readElementText().toStdString();
            }

            if (stream.isStartElement() && stream.name()=="Calibration") {
                i.amplitude = stream.attributes().value("amplitude").toString().toInt();
            }
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
        std::cout << "image: " << it->first << std::endl;
        image& i = it->second;
        std::cout << "  has points: " << (int)i.hasPoints << std::endl;
        std::cout << "  amplitude:  " << i.amplitude << std::endl;
        std::cout << "  comment:    " << i.comment << std::endl;
    }
}

bool checkImages(std::string& dataPath, std::map<std::string,std::string>& uniqueImages) {

    return true;
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
            iss >> m.amplitude;
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

    std::cout << "datapath: " << dataPath << std::endl;
    std::cout << "measurements:" << std::endl;
    for (std::vector<measurementSet>::iterator it = measurementSets.begin();
         it!=measurementSets.end();
         ++it) {
        std::cout << it->amplitude << " um\t" << it->undeformed << "\t" << it->deformed << std::endl;
    }

    if (!checkImages(dataPath, uniqueImages)) return -1;

    return 0;
}
