#ifndef ASSEMBLYUEYEMODEL_H
#define ASSEMBLYUEYEMODEL_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

class AssemblyUEyeModel : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyUEyeModel(int updateInterval = 60,
                               QObject *parent = 0);

    void myMoveToThread(QThread *thread);

public slots:

protected slots:

  void updateInformation();

protected:

  int updateInterval_;
  QTimer* timer_;

  QMutex mutex_;
  
signals:


};

#endif // ASSEMBLYUEYEMODEL_H
