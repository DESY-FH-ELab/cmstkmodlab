#ifndef ASSEMBLYVMARKERFINDER_H
#define ASSEMBLYVMARKERFINDER_H

#include <opencv2/opencv.hpp>

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

class AssemblyMarkerCircle
{
public:

    AssemblyMarkerCircle(float x, float y, float r, float q);

    float x() const { return x_; }
    float y() const { return y_; }
    float r() const { return r_; }
    float q() const { return q_; }

protected:

    float x_, y_, r_, q_;
};

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
