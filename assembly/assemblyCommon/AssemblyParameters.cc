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

AssemblyParameters* AssemblyParameters::instance_ = nullptr;

AssemblyParameters::AssemblyParameters(const std::string& file_path, const QString& dblogfilepath, QObject* parent)
 : QObject(parent)
 , view_(nullptr)
 , DBlogfile_path(dblogfilepath) //New: need this path to update DBlogfile from this class
{
  NQLog("AssemblyParameters", NQLog::Debug) << "constructed";

  this->read_from_file(file_path);
}

AssemblyParameters::~AssemblyParameters()
{
  NQLog("AssemblyParameters", NQLog::Debug) << "destructed";
}

AssemblyParameters* AssemblyParameters::instance(const std::string& file_path, const QString& dblogfilepath, QObject* parent)
{
  instance_ = new AssemblyParameters(file_path, dblogfilepath, parent);

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
      assembly::kill_application(tr("[AssemblyParameters::instance]"), tr("Pointer to AssemblyParameters instance is NULL. Aborting."));
    }
  }

  return instance_;
}

void AssemblyParameters::issue_key_error(const std::string& key) const
{
  NQLog("AssemblyParameters", NQLog::Critical) << "issue_key_error"
     << " [issue_key_error::getValue] ** ERROR: failed to get value for key: " << key;

  assembly::kill_application(tr("[AssemblyParameters::issue_key_error]"), tr("Failed to find value for key: \"%1\"\n. Aborting.").arg(QString(key.c_str())));
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
    disconnect(view_, SIGNAL(append_to_file_request(QString)), this, SLOT(append_to_file(QString)));

    for(const auto& key : view_->entries_map())
    {
      disconnect(view_->get(key.first), SIGNAL(textChanged(const QString&)), this, SLOT(replace_assemblyParameter_value_inDBLogfile(const QString&)));
    }
  }

  view_ = view;

  connect(view_, SIGNAL(read_from_file_request(QString)), this, SLOT(read_from_file(QString)));
  connect(view_, SIGNAL(write_to_file_request(QString)), this, SLOT(write_to_file(QString)));
  connect(view_, SIGNAL(append_to_file_request(QString)), this, SLOT(append_to_file(QString)));

  //For all QLineEdits in the 'Parameters' tab, connect sig/slot such that the DBLogfile will automatically get updated everytime some parameter gets changed in the GUI
  for(const auto& key : view_->entries_map())
  {
    connect(view_->get(key.first), SIGNAL(textChanged(const QString&)), this, SLOT(replace_assemblyParameter_value_inDBLogfile(const QString&)));
  }

  return;
}

//Write all assembly parameters to file (overwriting by default, otherwise appending)
void AssemblyParameters::update_file(const QString& f_path, bool overwrite/*=true*/)
{
  QFile data(f_path);

  if((overwrite && data.open(QFile::WriteOnly | QFile::Truncate)) || (!overwrite && data.open(QFile::Append))) //Overwrite or append
  {
    QTextStream out(&data);

    for(const auto& i_pair : map_double_)
    {
      std::stringstream strs;
      strs << i_pair.second;
      out<<QString::fromStdString(i_pair.first)<<" "<<QString::fromStdString(strs.str())<< "\n"; //NB: Leave only one space between keyword-value, for easier regexp parsing
    }
  }

  return;
}

//Update the assembly parameter block in the DBLogfile with updated value(s)
void AssemblyParameters::Update_AssemblyParameter_Value_inDBLogfile(const QString& f_path, const QRegExp& expr, const QString& newText)
{
  QMutexLocker ml(&mutex_); //Lock thread <-> avoid multiple concurrent edits to same file

  //Read and save the entire current DBLogfile content into buffer
  QFile data(f_path);
  data.open(QIODevice::Text | QIODevice::ReadOnly);
  QString dataText = data.readAll();
  data.close();

  // std::cout<<"-- Regexp"<<(dataText.contains(QRegExp(expr))? " ":" NOT ")<<"matched"<<std::endl;

  //Replace old --> new QString
  dataText.replace(QRegExp(expr), QString(newText));

  //Dump modified text to terminal (for debugging)
  // std::cout<<dataText.toUtf8().constData()<<std::endl;

  //Overwrite DBLogfile with updated text
  data.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream out(&data);
  out << dataText;
  data.close();

  return;
}

void AssemblyParameters::write_to_file(const std::string& f_path)
{
  this->write_to_file(QString::fromStdString(f_path));
}

void AssemblyParameters::write_to_file(const QString& f_path)
{
  this->update_file(f_path);
}

void AssemblyParameters::append_to_file(const QString& f_path)
{
  this->update_file(f_path, false); //Append (instead of overwriting)
}

void AssemblyParameters::read_from_file(const QString& f_path)
{
  this->read_from_file(std::string(f_path.toUtf8().constData()));
}

void AssemblyParameters::read_from_file(const std::string& f_path)
{
  NQLog("AssemblyParameters", NQLog::Spam) << "read_from_file"
    << " - filename " << f_path;

  auto config = ApplicationConfig::instance();
  config->append(f_path, "parameters");

  map_double_.clear();

  for(const auto& i_key : config->getKeys()) {

    if(!(i_key.alias == "parameters"))
    {
      continue;
    }

    if(map_double_.find(i_key.key) != map_double_.end())
    {
      NQLog("AssemblyParameters", NQLog::Warning) << "read_from_file"
         << ": duplicate assembly parameter \"" << i_key.alias << "." << i_key.key << "\", parameter value will be overwritten";
    }

    double i_val_double;
    try
    {
      i_val_double = config->getValue<double>(i_key);
      map_double_[i_key.key] = i_val_double;
    } catch (...) {
      NQLog("AssemblyParameters", NQLog::Warning) << "read_from_file"
      << ": invalid format for input parameter \"" << i_key.key << "\" (" << config->getValue<std::string>(i_key) << "), cannot be added to AssemblyParameters";
    }
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

//Check validity of some parameters
bool AssemblyParameters::isValidConfig()
{
    for(std::map<std::string, double>::iterator it = map_double_.begin(); it != map_double_.end(); it++)
    {
        if(it->first == "Thickness_Baseplate" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
        if(it->first == "Thickness_PSS" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
        if(it->first == "Thickness_PSP" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
        if(it->first == "Thickness_MPA" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
        if(it->first == "Thickness_Spacer" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
        if(it->first == "Thickness_Spacer" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
        if(it->first == "Thickness_GlueLayer" && it->second < 0)
        {
            NQLog("AssemblyParameters", NQLog::Fatal) << "\e[1;31mInvalid AssemblyParameters configuration !\e[0m ("<<it->first<<" = "<<it->second<<")";
            return false;
        }
    }

    return true;
}

//Whenever the user updates some assembly parameter value interactively in the GUI --> automatically update the DBLogfile with new value(s)
//NB: the live view of the DBLogfile's contents in the GUI does not get updated, only the actual file !
void AssemblyParameters::replace_assemblyParameter_value_inDBLogfile(const QString& newVal)
{
    QLineEdit* ptr_qedit = qobject_cast<QLineEdit*>(sender()); //Get pointer address of QLineEdit that triggered the SIGNAL(textChanged)
    // std::cout<<"Ptr to QLineEdit sender = "<<ptr_qedit<<std::endl;

    for(const auto& key : view_->entries_map()) //Loop on all QLineEdits
    {
        if(ptr_qedit == view_->get(key.first)) //Found relevant QLineEdit
        {
            // std::cout<<"MATCHED: address="<<view_->get(key.first)<<" / key="<<key.first<<std::endl;

            QRegExp regexp(QString("("+QString::fromStdString(key.first)+"\\s\\-?\\d*.?\\d*.\\n)")); //Define regexp to be matched = keyword + whitespace <-> [\\s] + potentially a minus sign in front <-> [\\-?] + any decimal number <-> [\\d*.?\\d*] + potentially some invalid character at the end <-> [.] + newline <-> [\\n]

            // std::cout<<"newVal="<<newVal.toUtf8().constData()<<std::endl;
            QString newText = QString::fromStdString(key.first) + QString(" ") + newVal + QString("\n"); //New text = Keyword + new value + newline
            // QString newText = QString::fromStdString(key.first) + QString(" ") + newVal; //New text = Keyword + new value + newline

            this->Update_AssemblyParameter_Value_inDBLogfile(DBlogfile_path, regexp, newText); //Update DBLogfile
        }
    }

    return;
}
