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

#include <nqlogger.h>

#include <QObject>
#include <QString>
#include <QTimer>

class AssemblyLogFileController : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyLogFileController(const QString&, QObject* parent=nullptr);
  virtual ~AssemblyLogFileController();

 protected:

  QString file_path_;
  uint last_read_endline_;

  bool reading_;

  QTimer* update_timer_;

 public slots:

  void update();

 signals:

  void new_lines(const QStringList&);
};
