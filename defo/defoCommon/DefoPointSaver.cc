#include <QXmlStreamWriter>

#include "DefoPointSaver.h"

const QString DefoPointSaver::LINE_FORMAT = "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\n";

DefoPointSaver::DefoPointSaver(const QString &filename, QObject* parent) :
  QFile(filename, parent)
{
  open( QIODevice::WriteOnly | QIODevice::Truncate );
}

DefoPointSaver::~DefoPointSaver() {
  close();
}

void DefoPointSaver::writePoints(const DefoPointCollection &points)
{
    for (DefoPointCollection::const_iterator it = points.begin();
         it != points.end();
         ++it) {
         if ((*it).isValid())
           writePoint(*it);
    }
}

void DefoPointSaver::writePoint(const DefoPoint& point)
{
  double x = point.getX();
  double y = point.getY();
  QColor color = point.getColor();
  float hue = color.hsvHueF();
  float saturation = color.hsvSaturationF();
  float value = color.valueF();

  QString line = LINE_FORMAT
      .arg(x, 0, 'e', 6)
      .arg(y, 0, 'e', 6)
      .arg(hue, 0, 'e', 6)
      .arg(saturation, 0, 'e', 6)
      .arg(value, 0, 'e', 6)
      .arg((int)(point.isIndexed()))
      .arg(point.getIndex().first, 5)
      .arg(point.getIndex().second, 5)
      .arg((int)point.isValid());
  
  write(line.toStdString().c_str());
}

void DefoPointSaver::writeXMLPoints(const DefoPointCollection &points)
{
    QXmlStreamWriter stream(this);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();

    stream.writeStartElement("DefoPoints");
    stream.writeAttribute("count", QString().setNum(points.size()));

    for (DefoPointCollection::const_iterator it = points.begin();
         it != points.end();
         ++it) {
        stream.writeStartElement("DefoPoint");
        stream.writeAttribute("x", QString().setNum(it->getX(), 'e', 6));
        stream.writeAttribute("y", QString().setNum(it->getY(), 'e', 6));
        float h, s ,v;
        QColor c = it->getColor();
        h = c.hsvHueF();
        s = c.hsvSaturationF();
        v = c.valueF();
        stream.writeAttribute("H", QString().setNum(h, 'e', 6));
        stream.writeAttribute("S", QString().setNum(s, 'e', 6));
        stream.writeAttribute("V", QString().setNum(v, 'e', 6));

        stream.writeAttribute("indexed", QString().setNum(it->isIndexed()));
        if (it->isIndexed()) {
            stream.writeAttribute("ix", QString().setNum(it->getIndex().first));
            stream.writeAttribute("iy", QString().setNum(it->getIndex().second));
        }

        stream.writeEndElement();
    }

    stream.writeEndElement();

    stream.writeEndDocument();
}
