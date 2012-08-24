#ifndef DEFOPOINTRECOGNITIONMODEL_H
#define DEFOPOINTRECOGNITIONMODEL_H

#include <QObject>
#include "DefoConfigReader.h"

class DefoPointRecognitionModel : public QObject
{
    Q_OBJECT
public:
  enum Threshold {
      THRESHOLD_1 = 0 ///< Identifier for threshold 1
    , THRESHOLD_2 = 1 ///< Identifier for threshold 2
    , THRESHOLD_3 = 2 ///< Identifier for threshold 3
  };

  explicit DefoPointRecognitionModel(QObject *parent = 0);

  // Methods for value, range and logical lower limit
  static const int THRESHOLD_MINIMUM;
  static const int THRESHOLD_MAXIMUM;
  int getThresholdValue(const Threshold& threshold) const;
  int getThresholdDynamicMininum(const Threshold& threshold) const;

public slots:
  void setThresholdValue(const Threshold& threshold, int value);

protected:
  std::vector<int> thresholds_;

signals:
  void thresholdValueChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void dynamicMinimumChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );

};

#endif // DEFOPOINTRECOGNITIONMODEL_H
