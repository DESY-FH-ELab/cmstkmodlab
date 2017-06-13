#include <iostream>

#include <QtCore>
#include <QXmlStreamWriter>
#include <QFile>

#include "Analyser.h"

Analyser::Analyser(QStringList& arguments)
 : arguments_(arguments)
{
	dataValid_ = false;
	for (int i=0;i<5;++i) switchStateValid_[i] = false;
	for (int i=0;i<3;++i) gaugeValid_[i] = false;
}

void Analyser::analyse()
{
  QStringList::const_iterator constIterator;
  for (constIterator = arguments_.constBegin(); constIterator != arguments_.constEnd();
      ++constIterator) {
      processFile(*constIterator);
  }

  QCoreApplication::quit();
}

void Analyser::processFile(const QString& filename)
{
	std::cerr << "processing file: " << filename.toLocal8Bit().constData() << std::endl;

	QFile file(filename);
	if (file.open(QFile::ReadOnly)) {
		QXmlStreamReader reader(&file);

		while (!reader.atEnd()) {

			QXmlStreamReader::TokenType tokentype = reader.readNext();

			if (tokentype==QXmlStreamReader::EndElement) {

				if (reader.name()=="Status") {
					if (dataValid_) dumpData();
				}

			}

			if (tokentype==QXmlStreamReader::StartElement) {

				QXmlStreamAttributes attributes = reader.attributes();

				if (attributes.hasAttribute("time")) {
					utime_ = QDateTime::fromString(attributes.value("time").toString(), Qt::ISODate);
				}

				if (reader.name()=="ConradSwitch") {

					if (attributes.hasAttribute("id") &&
							attributes.hasAttribute("state")) {
						int id = attributes.value("id").toString().toInt();
						bool state = attributes.value("state").toString().toInt();

						switchState_[id] = state;
						switchStateValid_[id] = true;

						if (attributes.hasAttribute("time")) dumpData();
					}
				}

				if (reader.name()=="LeyboldGraphixThree") {
					if (attributes.hasAttribute("id") &&
							attributes.hasAttribute("status") &&
							attributes.hasAttribute("p")) {
						int id = attributes.value("id").toString().toInt() - 1;
						bool state = attributes.value("status").toString().toInt();
						float p = attributes.value("p").toString().toFloat();

						gaugeState_[id] = state;
						gaugeValid_[id] = true;
						gaugePressure_[id] = p;

						if (attributes.hasAttribute("time")) dumpData();
					}
				}

				if (dataValid_==false) {
					bool valid = true;
					for (int i=0;i<5;++i) {
						valid &= switchStateValid_[i];
						//std::cout << switchStateValid_[i] << " ";
					}
					for (int i=0;i<3;++i) {
						valid &= gaugeValid_[i];
						//std::cout << gaugeValid_[i] << " ";
					}
					//std::cout << std::endl;

					dataValid_ = valid;
				}
			}
		}
	}
}

void Analyser::dumpData()
{
	std::cout << utime_.toTime_t() ;
	for (int i=0;i<5;++i) std::cout << " " << switchState_[i];
	for (int i=0;i<3;++i) std::cout << " " << gaugeState_[i] << " " << std::scientific << gaugePressure_[i];
	std::cout << std::endl;
}
