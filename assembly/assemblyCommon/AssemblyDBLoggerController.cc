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

#include <AssemblyDBLoggerController.h>

// AssemblyDBLoggerController* AssemblyDBLoggerController::instance_ = NULL; //Init pointer to instance

AssemblyDBLoggerController::AssemblyDBLoggerController(AssemblyDBLoggerModel* model, AssemblyDBLoggerView* view) :
model_(model),
view_(view)
{
    if(model_ == NULL) {NQLog("AssemblyDBLoggerController", NQLog::Critical) << "\e[31m Invalid (null) pointer to AssemblyDBLoggerModel object \e[0m";}
    if(view_ == NULL) {NQLog("AssemblyDBLoggerController", NQLog::Critical) << "\e[31m Invalid (null) pointer to AssemblyDBLoggerView object \e[0m";}

    connect_all();
}

AssemblyDBLoggerController::~AssemblyDBLoggerController()
{
}

/*
//Create single instance of the class, to be used by all other classes
AssemblyDBLoggerController* AssemblyDBLoggerController::instance(AssemblyDBLoggerModel* model, AssemblyDBLoggerView* view)
{
    instance_ = new AssemblyDBLoggerController(model, view); //Create dynamic instance

    return instance_; //Return pointer to new instance
}

//Returns pointer to single instance of this class (must have been previously created in main assembly code)
//Same instance is then accessed by all other classes
AssemblyDBLoggerController* AssemblyDBLoggerController::instance()
{
    if(instance_ == NULL) {NQLog("AssemblyDBLoggerController", NQLog::Critical) << "\e[31minstance" << ": returning invalid (null) pointer to AssemblyDBLoggerController instance\e[0m";}

    return instance_;
}
*/

//Copy pre-existing logfile to current logfile path (overwrite) <-> 'Load'
void AssemblyDBLoggerController::copy_existing_logfile(const QString path_existingFile)
{
    if(!QFile::exists(path_existingFile)) {NQLog("AssemblyDBLoggerController", NQLog::Warning) << "copy_existing_logfile" << ": logfile " << path_existingFile << " not found ! Can not load it... "; return;}

    QFile::remove(get_dblogfilepath()); //Delete current logfile
    QFile::copy(path_existingFile, get_dblogfilepath()); //Copy pre-existing logfile

    this->model_->setOverwritingMode(); //Need to also overwrite the logfile content displayed in the GUI tab

    NQLog("AssemblyDBLoggerController", NQLog::Message) << "copy_existing_logfile" << ": copied existing logfile " << path_existingFile << " to path " << get_dblogfilepath();

    return;
}

//Copy current logfile to user-chosen path  <-> 'Save'
void AssemblyDBLoggerController::copy_current_logfile(const QString dest_path)
{
    if(!QFile::exists(dest_path)) {QFile::remove(dest_path); NQLog("AssemblyDBLoggerController", NQLog::Message) << "copy_current_logfile" << ": overwrite file " << dest_path;}

    QFile::copy(get_dblogfilepath(), dest_path);

    NQLog("AssemblyDBLoggerController", NQLog::Message) << "copy_current_logfile" << ": copied current logfile " << get_dblogfilepath() << " to path " << dest_path;

    return;
}

//Connect signal/slots between Model/View/Controller classes
void AssemblyDBLoggerController::connect_all()
{
    connect(this->model_, SIGNAL(refresh_content_view(QStringList)), this->view_, SLOT(update_content(QStringList)));
    connect(this->model_, SIGNAL(overwrite_content_view()), this->view_, SLOT(clear_content()));

    connect(this->view_->get_DBLogger_appendModeBox(), SIGNAL(currentIndexChanged(const QString&)), this->model_, SLOT(setAppendMode(const QString&)));

    connect(this->view_, SIGNAL(validPath_toExistingLog(const QString)), this, SLOT(copy_existing_logfile(const QString)));
    connect(this->view_, SIGNAL(validPath_toCopy(const QString)), this, SLOT(copy_current_logfile(const QString)));
    connect(this->view_, SIGNAL(DBLogMessage(const QString)), this, SLOT(writeMessageToLog(QString)));

    return;
}

//Get path of the DBLog file
const QString AssemblyDBLoggerController::get_dblogfilepath()
{
    if(model_ == NULL) {NQLog("AssemblyDBLoggerController", NQLog::Critical) << "\e[31mInvalid (null) pointer to AssemblyDBLoggerModel object\e[0m";}

    return this->model_->get_dblogfilepath();
}

//Write message into DBLog file via the model
void AssemblyDBLoggerController::writeMessageToLog(QString message)
{
    NQLog("AssemblyDBLoggerController", NQLog::Debug) << "writeMessageToLog " << message.toUtf8().constData();

    this->model_->writeMessage(message);

    return;
}

// ===============================================================================
