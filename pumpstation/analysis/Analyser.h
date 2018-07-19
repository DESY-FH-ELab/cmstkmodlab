/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ANALYSER_H
#define ANALYSER_H

#include <array>
#include <map>
#include <string>

#include <QDateTime>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <QDir>

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

class DataFileDate
{
public:

	DataFileDate();
	DataFileDate(const DataFileDate& other);
	DataFileDate(const QDate& date, int n);
	DataFileDate(int y, int m, int d, int n);

	const QDate& getDate() const { return date_; }
	int getNumber() const { return n_; }

	bool operator<(const DataFileDate& other) const {
		if (date_==other.date_) return (n_<other.n_);
		return date_<other.date_;
	}

protected:

	QDate date_;
	int n_;
};

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

  QDir dataDirectory_;
  std::map<DataFileDate, QString> fileMap_;

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
