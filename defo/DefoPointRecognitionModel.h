#ifndef DEFOPOINTRECOGNITIONMODEL_H
#define DEFOPOINTRECOGNITIONMODEL_H

#include <QObject>
#include "DefoConfigReader.h"

class DefoPointRecognitionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoPointRecognitionModel(QObject *parent = 0);
  int getThresholdValue(int threshold) const;

public slots:
  void setThresholdValue(int threshold, int value);

protected:
  std::vector<int> thresholds_;

signals:
  void thresholdValueChanged(int threshold, int value);

};

#endif // DEFOPOINTRECOGNITIONMODEL_H
