#include "DefoPointSaver.h"

const QString PointSaver::LINE_FORMAT = "%1\t%2\t%3\t%4\t%5\n";

PointSaver::PointSaver(const QString &filename, QObject* parent) :
  QFile(filename, parent)
{
  open( QIODevice::WriteOnly | QIODevice::Truncate );
}

PointSaver::~PointSaver() {
  close();
}

void PointSaver::writePoint(const DefoPoint& point)
{
  double x = point.getX();
  double y = point.getY();
  QColor color = point.getColor();
  int hue = color.hsvHue();
  int saturation = color.hsvSaturation();
  int value = color.value();

  QString line = LINE_FORMAT
      .arg(x, 0, 'e', 5)
      .arg(y, 0, 'e', 5)
      .arg(hue)
      .arg(saturation)
      .arg(value);
  write(line.toAscii());
}
