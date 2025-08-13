/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ASSEMBLYVUEYEMODEL_H
#define ASSEMBLYVUEYEMODEL_H

#include <AssemblyVUEyeCamera.h>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QVector>

class AssemblyVUEyeModel : public QObject
{
 Q_OBJECT

  public:

    explicit AssemblyVUEyeModel(int updateInterval, QObject* parent);
    ~AssemblyVUEyeModel();

    size_t getCameraCount() const;
    AssemblyVUEyeCamera * getCamera(size_t idx);
    AssemblyVUEyeCamera * getCameraByID(unsigned int id);

  public slots:

    virtual void updateInformation() = 0;

  protected:

    int updateInterval_;
    QTimer* timer_;

    QMutex mutex_;

    QVector<QThread*> threads_;
    QVector<AssemblyVUEyeCamera*> cameras_;

    void clear();

  signals:

    void cameraCountChanged(unsigned int);
};

#endif // ASSEMBLYVUEYEMODEL_H
