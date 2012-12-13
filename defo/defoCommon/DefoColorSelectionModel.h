#ifndef DEFOCOLORSELECTIONMODEL_H
#define DEFOCOLORSELECTIONMODEL_H

#include <QColor>
#include <QDir>

class DefoColorSelectionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoColorSelectionModel(QObject *parent = 0);

  float getDistance(float hue, float saturation);
  void setColor(float hue, float saturation);

  const QColor& getColor() const { return color_; }

  void write(const QDir& path);
  void read(const QString& filename);

protected:
  QColor color_;

signals:
  void colorChanged(float hue, float saturation);
};

#endif // DEFOALIGNMENTMODEL_H
