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

#ifndef ASSEMBLYPARAMETERS_H
#define ASSEMBLYPARAMETERS_H

/*  Description:
 *   Controller to convert image to binary (BW) image
 */

#include <QObject>
#include <QString>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyParameters : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyParameters(QObject* parent=nullptr);
  virtual ~AssemblyParameters();

  double aaa(){ return aaa_; }

 private:
  Q_DISABLE_COPY(AssemblyParameters)

 protected:

  mutable QMutex mutex_;

  double aaa_;

 public slots:

 signals:
};

#endif // ASSEMBLYPARAMETERS_H
