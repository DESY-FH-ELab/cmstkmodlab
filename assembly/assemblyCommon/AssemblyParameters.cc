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

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

AssemblyParameters* AssemblyParameters::instance_ = nullptr;

AssemblyParameters::AssemblyParameters(const std::string& file_path, QObject* parent) : QObject(parent)
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

AssemblyParameters* AssemblyParameters::instance()
{
  if(instance_ == nullptr)
  {
    NQLog("AssemblyParameters", NQLog::Critical) << "instance"
       << ": returning invalid (null) pointer to AssemblyParameters instance";
  }

  return instance_;
}

void AssemblyParameters::issueKeyError(const std::string& key) const
{
  NQLog("AssemblyParameters", NQLog::Critical) << "issueKeyError"
     << " [issueKeyError::getValue] ** ERROR: failed to get value for key: " << key;

  QMessageBox::critical(0
   , tr("[AssemblyParameters::issueKeyError]")
   , tr("Failed to find value for key: \"%1\"\n.").arg(QString(key.c_str()))
   , QMessageBox::Ok
  );
}

void AssemblyParameters::read_from_file(const QString& f_path)
{
  this->read_from_file(std::string(f_path.toUtf8().constData()));
}

void AssemblyParameters::read_from_file(const std::string& f_path)
{
  ApplicationConfigReader reader(f_path);

  keymap_.clear();

  reader.fill(keymap_);

  return;
}

void AssemblyParameters::write_to_file(const QString& f_path)
{
  QFile data(f_path);

  if(data.open(QFile::WriteOnly | QFile::Truncate))
  {
    QTextStream out(&data);

    for(const auto& key : keymap_)
    {
      out
        << qSetFieldWidth(30) << left  << QString::fromStdString(key.first)
        << qSetFieldWidth(20) << right << QString::fromStdString(key.second)
        << "\n";
    }
  }

  return;
}

void AssemblyParameters::write_to_file(const std::string& f_path)
{
  this->write_to_file(QString::fromStdString(f_path));
}
