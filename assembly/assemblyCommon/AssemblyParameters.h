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

class AssemblyParameters : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyParameters(const std::string&, QObject* parent=nullptr);
  virtual ~AssemblyParameters();

  static AssemblyParameters* instance(const std::string&, QObject* parent=nullptr);
  static AssemblyParameters* instance(const bool permissive=true);

  void issue_key_error(const std::string&) const;

  double get(const std::string&) const;

  const std::map<std::string, double>& map_double() const { return map_double_; }

  void set_view(const AssemblyParametersView* const);

  bool update();

 private:
  Q_DISABLE_COPY(AssemblyParameters)

 protected:

  static AssemblyParameters* instance_;

  const AssemblyParametersView* view_;

  std::map<std::string, double> map_double_;

 public slots:

  void write_to_file(const QString&);
  void write_to_file(const std::string&);

  void read_from_file(const QString&);
  void read_from_file(const std::string&);

 signals:
};

#endif // ASSEMBLYPARAMETERS_H
