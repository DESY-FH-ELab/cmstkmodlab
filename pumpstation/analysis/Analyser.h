#ifndef ANALYSER_H
#define ANALYSER_H

#include <array>

#include <QDateTime>
#include <QStringList>
#include <QObject>

#include <TFile.h>
#include <TTree.h>
#include <TDatime.h>

typedef struct {
	unsigned int   uTime;
	TDatime        datime;
	int            switchState[5];
	int            gaugeState[3];
	double         pressure[3];
} Measurement_t;

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

  bool measurementValid_;
  Measurement_t measurement_;
  TFile *ofile_;
  TTree *otree_;
};

#endif // ANALYSER_H
