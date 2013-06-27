#ifndef DEFOPOINTRECOGNITIONMODEL_H
#define DEFOPOINTRECOGNITIONMODEL_H

#include <vector>

#include <QObject>

class DefoPointRecognitionModel : public QObject
{
    Q_OBJECT
public:
  enum Threshold {
      THRESHOLD_1 = 0 ///< Identifier for threshold 1
    , THRESHOLD_2 = 1 ///< Identifier for threshold 2
    , THRESHOLD_3 = 2 ///< Identifier for threshold 3
    , MAXTHRESHOLDS
  };

  explicit DefoPointRecognitionModel(QObject *parent = 0);

  // Methods for value, range and logical lower limit
  static const int THRESHOLD_MINIMUM;
  static const int THRESHOLD_MAXIMUM;
  static const int HALFSQUAREWIDTH_MINIMUM;
  static const int HALFSQUAREWIDTH_MAXIMUM;

  int getThresholdValue(const Threshold& threshold) const;
  int getThresholdDynamicMininum(const Threshold& threshold) const;

  // Half square width functions
  int getHalfSquareWidth() const;

  // Point color functions
//  int getReferencePointHue() const;
//  int getReferencePointHueWidth() const;

public slots:
  void setThresholdValue(const Threshold& threshold, int value);
  void setHalfSquareWidth(int value);
//  void setReferencePointHue(int value);
//  void setReferencePointHueWidth(int value);
  void setControlsEnabled(bool enabled);

protected:
  std::vector<int> thresholds_;
  int halfSquareWidth_;
//  int referencePointHue_;
//  int referencePointHueWidth_;

signals:
  void thresholdValueChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void dynamicMinimumChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void halfSquareWidthChanged(int value);
//  void referencePointHueChanged(int value);
  void controlStateChanged(bool);
};

#endif // DEFOPOINTRECOGNITIONMODEL_H
