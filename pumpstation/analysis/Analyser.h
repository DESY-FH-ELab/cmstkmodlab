#ifndef ANALYSER_H
#define ANALYSER_H

#include <array>

#include <QDateTime>
#include <QStringList>
#include <QObject>

class Analyser : public QObject
{
  Q_OBJECT

public:

  Analyser(QStringList& arguments);

public slots:

  void analyse();

private:

  void processFile(const QString& filename);
  void dumpData();

  QStringList arguments_;

  QDateTime utime_;
  bool dataValid_;
  std::array<bool,5> switchState_;
  std::array<bool,5> switchStateValid_;
  std::array<bool,3> gaugeState_;
  std::array<float,3> gaugePressure_;
  std::array<bool,3> gaugeValid_;
};

#endif // ANALYSER_H
