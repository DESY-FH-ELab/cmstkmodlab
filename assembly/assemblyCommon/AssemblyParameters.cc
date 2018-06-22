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
#include <ApplicationConfig.h>
#include <ApplicationConfigReader.h>

#include <AssemblyParameters.h>
#include <AssemblyUtilities.h>

#include <sstream>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

AssemblyParameters* AssemblyParameters::instance_ = nullptr;

AssemblyParameters::AssemblyParameters(const std::string& file_path, QObject* parent)
 : QObject(parent)
 , view_(nullptr)
{
  NQLog("AssemblyParameters", NQLog::Debug) << "constructed";

  this->read_from_file(file_path);
}

AssemblyParameters::~AssemblyParameters()
{
  NQLog("AssemblyParameters", NQLog::Debug) << "destructed";
}

AssemblyParameters* AssemblyParameters::instance(const std::string& file_path, QObject* parent)
{
  instance_ = new AssemblyParameters(file_path, parent);

  return instance_;
}

AssemblyParameters* AssemblyParameters::instance(const bool permissive)
{
  if(instance_ == nullptr)
  {
    NQLog("AssemblyParameters", NQLog::Critical) << "instance"
       << ": returning invalid (null) pointer to AssemblyParameters instance";

    if(permissive == false)
    {
      QMessageBox::critical(0
       , tr("[AssemblyParameters::instance]")
       , tr("Pointer to AssemblyParameters instance is NULL. Aborting.")
       , QMessageBox::Abort
      );

      throw; // must abort
    }
  }

  return instance_;
}

void AssemblyParameters::issue_key_error(const std::string& key) const
{
  NQLog("AssemblyParameters", NQLog::Critical) << "issue_key_error"
     << " [issue_key_error::getValue] ** ERROR: failed to get value for key: " << key;

  QMessageBox::critical(0
   , tr("[AssemblyParameters::issue_key_error]")
   , tr("Failed to find value for key: \"%1\"\n. Aborting.").arg(QString(key.c_str()))
   , QMessageBox::Abort
  );

  throw; // must abort
}

double AssemblyParameters::get(const std::string& key) const
{
  if(map_double_.find(key) == map_double_.end())
  {
    this->issue_key_error(key);

    return -1.;
  }

  return map_double_.at(key);
}

void AssemblyParameters::set_view(const AssemblyParametersView* const view)
{
  if(view == nullptr)
  {
    NQLog("AssemblyParameters", NQLog::Critical) << "set_view"
       << ": NULL pointer to AssemblyParametersView, no action taken";

    return;
  }

  if(view_ != nullptr)
  {
    disconnect(view_, SIGNAL(read_from_file_request(QString)), this, SLOT(read_from_file(QString)));
    disconnect(view_, SIGNAL( write_to_file_request(QString)), this, SLOT( write_to_file(QString)));
  }

  view_ = view;

  connect(view_, SIGNAL(read_from_file_request(QString)), this, SLOT(read_from_file(QString)));
  connect(view_, SIGNAL( write_to_file_request(QString)), this, SLOT( write_to_file(QString)));

  return;
}

void AssemblyParameters::write_to_file(const QString& f_path)
{
  QFile data(f_path);

  if(data.open(QFile::WriteOnly | QFile::Truncate))
  {
    QTextStream out(&data);

    for(const auto& i_pair : map_double_)
    {
      std::stringstream strs;
      strs << i_pair.second;

      out
        << qSetFieldWidth(30) << left  << QString::fromStdString(i_pair.first)
        << qSetFieldWidth(20) << right << QString::fromStdString(strs.str())
        << "\n";
    }
  }

  return;
}

void AssemblyParameters::write_to_file(const std::string& f_path)
{
  this->write_to_file(QString::fromStdString(f_path));
}

void AssemblyParameters::read_from_file(const QString& f_path)
{
  this->read_from_file(std::string(f_path.toUtf8().constData()));
}

void AssemblyParameters::read_from_file(const std::string& f_path)
{
  ApplicationConfigReader reader(f_path);

  std::multimap<std::string, std::string> multimap_str;
  reader.fill(multimap_str);

  map_double_.clear();

  for(const auto& i_pair : multimap_str)
  {
    const std::string& i_key = i_pair.first;

    if(map_double_.find(i_key) != map_double_.end())
    {
      NQLog("AssemblyParameters", NQLog::Warning) << "read_from_file"
         << ": duplicate assembly parameter \"" << i_key << "\", parameter value will be overwritten";
    }

    const QString i_val_qstr = QString::fromStdString(i_pair.second);

    bool i_val_valid(false);
    const double i_val_double = i_val_qstr.toDouble(&i_val_valid);

    if(i_val_valid == false)
    {
      NQLog("AssemblyParameters", NQLog::Warning) << "read_from_file"
         << ": invalid format for input parameter \"" << i_val_qstr << "\", cannot be added to AssemblyParameters";

      continue;
    }

    map_double_[i_key] = i_val_double;
  }

  return;
}

bool AssemblyParameters::update()
{
  if(view_ == nullptr)
  {
    NQLog("AssemblyParameters", NQLog::Critical) << "update"
       << ": NULL pointer to AssemblyParametersView, no action taken";

    return false;
  }

  const auto& map_str = view_->entries_map();

  map_double_.clear();

  for(const auto& i_pair : map_str)
  {
    const std::string& i_key = i_pair.first;

    if(map_double_.find(i_key) != map_double_.end())
    {
      NQLog("AssemblyParameters", NQLog::Warning) << "update"
         << ": duplicate assembly parameter \"" << i_key << "\", parameter value will be overwritten";
    }

    const QString i_val_qstr = QString::fromStdString(i_pair.second);

    bool i_val_valid(false);
    const double i_val_double = i_val_qstr.toDouble(&i_val_valid);

    if(i_val_valid == false)
    {
      NQLog("AssemblyParameters", NQLog::Warning) << "update"
         << ": invalid format for input parameter \"" << i_val_qstr << "\", cannot be added to AssemblyParameters";

      return false;
    }

    map_double_[i_key] = i_val_double;
  }

  return true;
}
