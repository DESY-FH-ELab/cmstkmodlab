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
 *    - contains parameters for assembly procedure
 */

#include <string>
#include <sstream>
#include <map>
#include <stdexcept>

#include <QObject>
#include <QString>

class AssemblyParameters : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyParameters(const std::string&, QObject* parent=nullptr);
  virtual ~AssemblyParameters();

  static AssemblyParameters* instance(const std::string&, QObject* parent=nullptr);
  static AssemblyParameters* instance();

  void issueKeyError(const std::string&) const;

  template <class T> T get(const std::string&, const T&) const;
  template <class T> T get(const std::string&)           const;

 private:
  Q_DISABLE_COPY(AssemblyParameters)

 protected:

  static AssemblyParameters* instance_;

  std::multimap<std::string, std::string> keymap_;

 public slots:

  void read_from_file(const QString&);
  void read_from_file(const std::string&);

  void write_to_file(const QString&);
  void write_to_file(const std::string&);
};

template <class T>
T AssemblyParameters::get(const std::string& key, const T& default_val) const
{
  T return_val = default_val;

  auto iter = keymap_.find(key);

  if(iter != keymap_.end())
  {
    std::istringstream iss(iter->second.c_str(), std::istringstream::in);
    iss >> return_val;
  }

  return return_val;
}

template <class T>
T AssemblyParameters::get(const std::string& key) const
{
  T return_val;

  auto iter = keymap_.find(key);

  if(iter != keymap_.end())
  {
    std::istringstream iss(iter->second.c_str(), std::istringstream::in);
    iss >> return_val;
  }
  else
  {
    this->issueKeyError(key);
  }

  return return_val;
}

#endif // ASSEMBLYPARAMETERS_H
