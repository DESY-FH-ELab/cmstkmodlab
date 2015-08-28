#ifndef ASSEMBLYVMARKERFINDER_H
#define ASSEMBLYVMARKERFINDER_H

#include <opencv2/opencv.hpp>

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

class AssemblyVMarkerFinder : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyVMarkerFinder(QObject *parent);
    ~AssemblyVMarkerFinder();

public slots:

    virtual void findMarker(const cv::Mat&) = 0;

protected slots:

protected:

    QMutex mutex_;

signals:

    void markerFound(const cv::Mat&);
};

#endif // ASSEMBLYVMARKERFINDER_H
