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

#include <QWidget>
#include <QPlainTextEdit>
#include <QString>
#include <QStringList>
#include <QMap>

class AssemblyLogTextViewer;

class AssemblyLogFileView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyLogFileView(const QStringList&, QWidget* parent=nullptr);
  virtual ~AssemblyLogFileView();

 protected:

  QMap<QString, AssemblyLogTextViewer*> viewers_;

 public slots:

  void append_text(const QStringList&);
};

class AssemblyLogTextViewer : public QPlainTextEdit
{
 Q_OBJECT

 public:

  explicit AssemblyLogTextViewer();
  virtual ~AssemblyLogTextViewer();
};
