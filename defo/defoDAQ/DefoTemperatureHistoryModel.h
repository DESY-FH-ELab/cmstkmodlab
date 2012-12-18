#ifndef DEFOTEMPERATUREHISTORYMODEL_H
#define DEFOTEMPERATUREHISTORYMODEL_H

#include <deque>
#include <QObject>

class DefoTemperatureHistoryModel : public QObject
{
    Q_OBJECT
public:
  typedef std::deque<double>::const_iterator const_iterator;

  explicit DefoTemperatureHistoryModel(QObject *parent = 0);

  const_iterator begin() const;
  const_iterator end() const;

  double getSize() const;
  double getLatest() const;
  double getMaximum() const;
  double getMinimum() const;

public slots:
  void addValue( double value );

protected:
  std::deque<double> temperatures_;
  unsigned int maxPos_;
  unsigned int minPos_;

signals:
  void valueAdded( double value );
  void maximumChanged( double newMax );
  void mininumChanged( double newMin );

};

#endif // DEFOTEMPERATUREHISTORYMODEL_H
