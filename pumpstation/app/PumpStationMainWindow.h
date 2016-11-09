#ifndef PUMPSTATIONMAINWINDOW_H
#define PUMPSTATIONMAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QTimer>

class PumpStationMainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit PumpStationMainWindow(double updateInterval,
                                 QWidget *parent = 0);

public slots:

  void quit();

signals:

protected slots:


protected:

  QWebView* view_;

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;
};

#endif // PUMPSTATIONMAINWINDOW_H
