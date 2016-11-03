#ifndef PLASMASTEPDOUBLESPINBOX_H
#define PLASMASTEPDOUBLESPINBOX_H

#include <QDoubleSpinBox>

class PlasmaStepDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
public:

  explicit PlasmaStepDoubleSpinBox(QWidget *parent = 0);

  QString textFromValue(double value) const;
  double valueFromText(const QString & text) const;
};

#endif // PLASMASTEPDOUBLESPINBOX_H
