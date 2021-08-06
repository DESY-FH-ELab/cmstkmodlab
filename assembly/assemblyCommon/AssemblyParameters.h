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
 *    - contains parameters for assembly procedure
 *    - can read params from file and write them to file
 *    - params can be updated via AssemblyParametersView
 */

#include <string>
#include <map>

#include <AssemblyParametersView.h>

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QMutex>
#include <QMutexLocker>

class AssemblyParameters : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyParameters(const std::string&, const QString&, QObject* parent=nullptr);
  virtual ~AssemblyParameters();

  static AssemblyParameters* instance(const std::string&, const QString&, QObject* parent=nullptr);
  static AssemblyParameters* instance(const bool permissive=true);

  void issue_key_error(const std::string&) const;

  double get(const std::string&) const;

  const std::map<std::string, double>& map_double() const { return map_double_; }

  void set_view(const AssemblyParametersView* const);

  bool update();

  void update_file(const QString&, bool=true);
  void Update_AssemblyParameter_Value_inDBLogfile(const QString&, const QRegExp&, const QString&);

 private:
  Q_DISABLE_COPY(AssemblyParameters)

 protected:

  static AssemblyParameters* instance_;

  const AssemblyParametersView* view_;

  std::map<std::string, double> map_double_;

  const QString& DBlogfile_path;

  mutable QMutex mutex_;

 public slots:

  void write_to_file(const QString&);
  void write_to_file(const std::string&);
  void append_to_file(const QString&);

  void read_from_file(const QString&);
  void read_from_file(const std::string&);

  bool isValidConfig();

  void replace_assemblyParameter_value_inDBLogfile(const QString&);

 signals:
};

#endif // ASSEMBLYPARAMETERS_H
