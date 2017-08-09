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

#ifndef ASSEMBLYVMARKERFINDER_H
#define ASSEMBLYVMARKERFINDER_H

#include <opencv2/opencv.hpp>

#include <QObject>
#include <QMutex>
#include <QMutexLocker>


#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif


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
