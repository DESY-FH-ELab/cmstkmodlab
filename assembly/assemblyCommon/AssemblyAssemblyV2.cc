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

#include <AssemblyAssemblyV2.h>

#include <string>
#include <unistd.h>

#include <QMessageBox>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>

AssemblyAssemblyV2::AssemblyAssemblyV2(const LStepExpressMotionManager* const motion, const RelayCardManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent)
 : QObject(parent)

 , motion_(motion)
 , vacuum_(vacuum)
 , smart_motion_(smart_motion)
 , config_(nullptr)

 , vacuum_pickup_(0)
 , vacuum_spacer_(0)
 , vacuum_basepl_(0)

 , pickup1_Z_(0.)
 , pickup2_Z_(0.)

 , makespace_Y_(0.)
 , makespace_Z_(0.)
 , position_y_before_makespace_(0.)
 , position_z_before_makespace_(0.)
 , position_before_makespace_stored_(false)

 , use_smartMove_(false)
 , in_action_(false)

 , PSSPlusSpacersToMaPSAPosition_isRegistered_(false)
 , PSSPlusSpacersToMaPSAPosition_X_(0.)
 , PSSPlusSpacersToMaPSAPosition_Y_(0.)
 , PSSPlusSpacersToMaPSAPosition_Z_(0.)
 , PSSPlusSpacersToMaPSAPosition_A_(0.)

 , Baseplate_ID_()
 , MaPSA_ID_()
 , PSS_ID_()
 , Glue1_ID_()
 , Glue2_ID_()
 , Glue3_ID_()
 , Module_ID_()

 , database_()
{
  // validate pointers to controllers
  this->motion();
  this->vacuum();

  // indices of vacuum lines
  config_ = ApplicationConfig::instance();
  if(config_ == nullptr)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[AssemblyAssemblyV2]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
  }

  vacuum_pickup_ = config_->getValue<int>("main", "Vacuum_PickupTool");
  vacuum_spacer_ = config_->getValue<int>("main", "Vacuum_Spacers");
  vacuum_basepl_ = config_->getValue<int>("main", "Vacuum_Baseplate");

  // absolute Z-position of motion stage for pickup of object after gluing
  // (1: PSs to Spacers, 2: PSs+Spacers to MaPSA)
  pickup1_Z_ = config_->getValue<double>("main", "AssemblyAssembly_pickup1_Z");
  pickup2_Z_ = config_->getValue<double>("main", "AssemblyAssembly_pickup2_Z");

  makespace_Y_ = config_->getValue<double>("main", "AssemblyAssembly_makespace_Y");
  makespace_Z_ = config_->getValue<double>("main", "AssemblyAssembly_makespace_Z");

  original_Z_velocity_ = motion_->get_velocity_Z();

  alreadyClicked_LowerPickupToolOntoMaPSA = false; alreadyClicked_LowerPickupToolOntoPSS = false; alreadyClicked_LowerMaPSAOntoBaseplate = false; alreadyClicked_LowerPSSOntoSpacers = false; alreadyClicked_LowerPSSPlusSpacersOntoGluingStage = false; alreadyClicked_LowerPSSPlusSpacersOntoMaPSA = false;

  std::string assembly_center_str = QString::fromStdString(config_->getValue<std::string>("main", "assembly_center")).toUpper().toStdString();
  if(assembly_center_str == "FNAL") {
      assembly_center_ = assembly::Center::FNAL;
      database_ = new DatabaseDummy(this);
  } else if(assembly_center_str == "BROWN") {
      assembly_center_ = assembly::Center::BROWN;
      database_ = new DatabaseDummy(this);
  } else if(assembly_center_str == "DESY") {
      assembly_center_ = assembly::Center::DESY;
      auto base_url = QString::fromStdString(config_->getValue<std::string>("main", "Database_URL"));
      auto token = QString::fromStdString(config_->getValue<std::string>("main", "Database_Token"));
      database_ = new DatabaseDESY(this, base_url, token);
  } else {
      NQLog("AssemblyAssemblyV2", NQLog::Warning) << "Invalid assembly center provided: \"" << assembly_center_str << "\". Provide one of the following options: \"FNAL\", \"BROWN\", \"DESY\"";
  }
}

const LStepExpressMotionManager* AssemblyAssemblyV2::motion() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Fatal) << "motion"
       << ": pointer to LStepExpressMotionManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssemblyV2]"), tr("pointer to LStepExpressMotionManager is NULL, aborting"));
  }

  return motion_;
}

const RelayCardManager* AssemblyAssemblyV2::vacuum() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Fatal) << "vacuum"
       << ": pointer to RelayCardManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssemblyV2]"), tr("pointer to RelayCardManager is NULL, aborting"));
  }

  return vacuum_;
}

void AssemblyAssemblyV2::use_smartMove(const int state)
{
  if(state == 2)
  {
    if(smart_motion_ == nullptr)
    {
      NQLog("AssemblyAssemblyV2", NQLog::Warning) << "use_smartMove(" << state << ")"
         << ": attempting to enable \"smartMove\" mode, but pointer to AssemblySmartMotionManager is invalid (NULL), \"smartMove\" mode will stay disabled";

      use_smartMove_ = false;
    }
    else
    {
      NQLog("AssemblyAssemblyV2", NQLog::Message) << "use_smartMove(" << state << ")"
         << ": \"smartMove\" mode switched ON";

      use_smartMove_ = true;
    }
  }
  else if(state == 0)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Message) << "use_smartMove(" << state << ")"
       << ": \"smartMove\" mode switched OFF";

    use_smartMove_ = false;
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "use_smartMove(" << state << ")"
       << ": invalid argument (state=" << state << "), no action taken (smartMove=" << use_smartMove_ << ")";
  }

  return;
}
// ----------------------------------------------------------------------------------------------------

void AssemblyAssemblyV2::ScanMaPSAID_start()
{
    bool ok = false;
    QString MaPSA_ID = QInputDialog::getText(nullptr, tr("Scan MaPSA ID"),
                                         tr("Scan MaPSA ID:"), QLineEdit::Normal,
                                         tr(""), &ok);
    if (!ok || MaPSA_ID.isEmpty()){
        emit ScanMaPSAID_aborted();
        return;
    } else {
        if(!database_->is_component_available(MaPSA_ID, "MaPSA"))
        {
            emit ScanMaPSAID_aborted();
            return;
        }
        MaPSA_ID_ = MaPSA_ID;
        emit MaPSA_ID_updated(MaPSA_ID);
        emit ScanMaPSAID_finished();
    }
}

void AssemblyAssemblyV2::ScanPSSID_start()
{
    bool ok = false;
    QString PSS_ID = QInputDialog::getText(nullptr, tr("Scan PS-s ID"),
                                         tr("Scan PS-s ID:"), QLineEdit::Normal,
                                         tr(""), &ok);
    if (!ok || PSS_ID.isEmpty()){
        emit ScanPSSID_aborted();
        return;
    } else {
        if(!database_->is_component_available(PSS_ID, "PSs%20Sensor"))
        {
            emit ScanPSSID_aborted();
            return;
        }
        PSS_ID_ = PSS_ID;
        emit PSS_ID_updated(PSS_ID);
        emit ScanPSSID_finished();
    }
}

void AssemblyAssemblyV2::ScanBaseplateID_start()
{
    bool ok = false;
    QString Baseplate_ID = QInputDialog::getText(nullptr, tr("Scan Baseplate ID"),
                                         tr("Scan Baseplate ID:"), QLineEdit::Normal,
                                         tr(""), &ok);
    if (!ok || Baseplate_ID.isEmpty()){
        emit ScanBaseplateID_aborted();
        return;
    } else {
        if(!database_->is_component_available(Baseplate_ID, "PS%20Baseplate"))
        {
            emit ScanBaseplateID_aborted();
            return;
        }
        Baseplate_ID_ = Baseplate_ID;
        emit Baseplate_ID_updated(Baseplate_ID);
        emit ScanBaseplateID_finished();
    }
}

void AssemblyAssemblyV2::ScanGlue1ID_start()
{
    bool ok = false;
    QString Glue1_ID = QInputDialog::getText(nullptr, tr("Scan Slow Glue ID (step 1)"),
                                         tr("Scan Slow Glue ID (step 1):"), QLineEdit::Normal,
                                         tr(""), &ok);
    if (!ok || Glue1_ID.isEmpty()){
        emit ScanGlue1ID_aborted();
        return;
    } else {
        if(!database_->is_component_available(Glue1_ID, "Glue"))
        {
            emit ScanGlue1ID_aborted();
            return;
        }
        Glue1_ID_ = Glue1_ID;
        emit Glue1_ID_updated(Glue1_ID);
        emit ScanGlue1ID_finished();
    }
}

void AssemblyAssemblyV2::ScanGlue2ID_start()
{
    bool ok = false;
    QString Glue2_ID = QInputDialog::getText(nullptr, tr("Scan Slow Glue ID (step 2)"),
                                         tr("Scan Slow Glue ID (step 2):"), QLineEdit::Normal,
                                         Glue1_ID_, &ok);
    if (!ok || Glue2_ID.isEmpty()){
        emit ScanGlue2ID_aborted();
        return;
    } else {
        if(!database_->is_component_available(Glue2_ID, "Glue"))
        {
            emit ScanGlue2ID_aborted();
            return;
        }
        Glue2_ID_ = Glue2_ID;
        emit Glue2_ID_updated(Glue2_ID);
        emit ScanGlue2ID_finished();
    }
}

void AssemblyAssemblyV2::ScanGlue3ID_start()
{
    bool ok = false;
    QString Glue3_ID = QInputDialog::getText(nullptr, tr("Scan Slow Glue ID (step 3)"),
                                         tr("Scan Slow Glue ID (step 3):"), QLineEdit::Normal,
                                         Glue2_ID_, &ok);
    if (!ok || Glue3_ID.isEmpty()){
        emit ScanGlue3ID_aborted();
        return;
    } else {
        if(!database_->is_component_available(Glue3_ID, "Glue"))
        {
            emit ScanGlue3ID_aborted();
            return;
        }
        Glue3_ID_ = Glue3_ID;
        emit Glue3_ID_updated(Glue3_ID);
        emit ScanGlue3ID_finished();
    }
}

void AssemblyAssemblyV2::ScanModuleID_start()
{
    bool ok = false;
    QString Module_ID = QInputDialog::getText(nullptr, tr("Scan Module ID"),
                                         tr("Scan Module ID:"), QLineEdit::Normal,
                                         tr(""), &ok);
    if (!ok || Module_ID.isEmpty()){
        emit ScanModuleID_aborted();
        return;
    } else {
        Module_ID_ = Module_ID;
        emit Module_ID_updated(Module_ID);
        emit ScanModuleID_finished();
    }
}

void AssemblyAssemblyV2::RegisterModuleID_start()
{
    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "RegisterModuleID_start: "
    << "Attempting to register module ID in DB";

    if(database_->register_module_name(Module_ID_, "OperatorName"))
    {
        NQLog("AssemblyAssemblyV2", NQLog::Spam) << "RegisterModuleID_start: "
        << "Successfully registered module in DB";
        emit RegisterModuleID_finished();
    } else {
        NQLog("AssemblyAssemblyV2", NQLog::Fatal) << "RegisterModuleID_start: "
        << "Could not register module in DB";
        emit RegisterModuleID_aborted();
    }
}

void AssemblyAssemblyV2::PushAllToDB_start()
{
    if(Baseplate_ID_.isEmpty() || MaPSA_ID_.isEmpty() || PSS_ID_.isEmpty() || Module_ID_.isEmpty() || Glue1_ID_.isEmpty() || Glue2_ID_.isEmpty() || Glue3_ID_.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information for Database Upload missing"));
        QString msg = QString("The following IDs are missing:") + (Baseplate_ID_.isEmpty() ? "\n\tBaseplate ID" : "") + (MaPSA_ID_.isEmpty() ? "\n\tMaPSA ID" : "") + (PSS_ID_.isEmpty() ? "\n\tPSS ID" : "") + (Glue1_ID_.isEmpty() ? "\n\tGlue 1 ID" : "") + (Glue2_ID_.isEmpty() ? "\n\tGlue 2 ID" : "") + (Glue3_ID_.isEmpty() ? "\n\tGlue 3 ID" : "") + (Module_ID_.isEmpty() ? "\n\tModule ID" : "");
        msgBox.setText(msg);
        msgBox.setInformativeText("Please add this information via the toolbar.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        int ret = msgBox.exec();

        emit PushAllToDB_aborted();
        return;
    }

    QDialog* msgBox = new QDialog();
    msgBox->setWindowTitle(tr("Push Full Module Information to Database"));

    QVBoxLayout* vlay = new QVBoxLayout();
    msgBox->setLayout(vlay);

    QLabel* main_txt = new QLabel(QString("Push the following information to database:\n\tBP:\t%1\n\tMaPSA:\t%2\n\tPS-s:\t%3\n\tGlues:\t%4 %5 %6\n\tModule:\t%7").arg(Baseplate_ID_).arg(MaPSA_ID_).arg(PSS_ID_).arg(Glue1_ID_).arg(Glue2_ID_).arg(Glue3_ID_).arg(Module_ID_));
    vlay->addWidget(main_txt);

    QHBoxLayout* comment_lay = new QHBoxLayout();

    QLabel* comment_lab = new QLabel("Comments:");
    QTextEdit* comment_lin = new QTextEdit("");
    comment_lin->setTabChangesFocus(true);
    comment_lin->setFixedHeight(60);

    comment_lay->addWidget(comment_lab);
    comment_lay->addWidget(comment_lin);

    vlay->addLayout(comment_lay);

    QLabel* info_txt = new QLabel("Do you want to push this information to the Database?");
    vlay->addWidget(info_txt);

    QDialogButtonBox* button_box = new QDialogButtonBox(Qt::Horizontal);
    button_box->addButton(QDialogButtonBox::Yes);
    button_box->addButton(QDialogButtonBox::No);
    button_box->setCenterButtons(true);

    vlay->addWidget(button_box);

    connect(button_box, SIGNAL(accepted()), msgBox, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), msgBox, SLOT(reject()));

    int ret = msgBox->exec();

    switch(msgBox->result())
    {
      case QDialog::Rejected:
        emit PushAllToDB_aborted();
        return;
      case QDialog::Accepted:
        // <--- Insert function to push to database here. --->
        NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushAllToDB_start: "
           << QString("Push the following information to database:\n\tBaseplate:\t%1\n\tMaPSA:\t\t%2\n\tPS-s:\t\t%3\n\tPS-s:\t\t%4\n\tModule:\t\t%5 %6 %7\n\tComment:\t\t%8").arg(Baseplate_ID_).arg(MaPSA_ID_).arg(PSS_ID_).arg(Glue1_ID_).arg(Glue2_ID_).arg(Glue3_ID_).arg(Module_ID_).arg(comment_lin->toPlainText()).toStdString();
        emit PushAllToDB_finished();
        break;
      default:
        emit PushAllToDB_aborted();
        return;
    }
}

void AssemblyAssemblyV2::PushStep1ToDB_start()
{
    if(Module_ID_.isEmpty() || Baseplate_ID_.isEmpty() || MaPSA_ID_.isEmpty() || Glue1_ID_.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information for Database Upload missing"));
        QString msg = QString("The following IDs are missing:") + (Module_ID_.isEmpty() ? "\n\tModule ID" : "") + (Baseplate_ID_.isEmpty() ? "\n\tBaseplate ID" : "") + (MaPSA_ID_.isEmpty() ? "\n\tMaPSA ID" : "") + (Glue1_ID_.isEmpty() ? "\n\tGlue 1 ID" : "");
        msgBox.setText(msg);
        msgBox.setInformativeText("Please add this information via the toolbar or assembly actions.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        int ret = msgBox.exec();

        emit PushStep1ToDB_aborted();
        return;
    }

    QDialog* msgBox = new QDialog();
    msgBox->setWindowTitle(tr("Push Module Information for MaPSA to BP assembly to Database"));

    QVBoxLayout* vlay = new QVBoxLayout();
    msgBox->setLayout(vlay);

    QLabel* main_txt = new QLabel(QString("Push the following information to database:\n\n\tModule:\t%1\n\tBP:\t%2\n\tMaPSA:\t%3\n\tGlue:\t%4").arg(Module_ID_).arg(Baseplate_ID_).arg(MaPSA_ID_).arg(Glue1_ID_));
    vlay->addWidget(main_txt);

    QHBoxLayout* comment_lay = new QHBoxLayout();

    QLabel* comment_lab = new QLabel("Comments:");
    QTextEdit* comment_lin = new QTextEdit("");
    comment_lin->setTabChangesFocus(true);
    comment_lin->setFixedHeight(60);

    comment_lay->addWidget(comment_lab);
    comment_lay->addWidget(comment_lin);

    vlay->addLayout(comment_lay);

    QLabel* info_txt = new QLabel("Do you want to push this information to the Database?");
    vlay->addWidget(info_txt);

    QDialogButtonBox* button_box = new QDialogButtonBox(Qt::Horizontal);
    button_box->addButton(QDialogButtonBox::Yes);
    button_box->addButton(QDialogButtonBox::No);
    button_box->setCenterButtons(true);

    vlay->addWidget(button_box);

    connect(button_box, SIGNAL(accepted()), msgBox, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), msgBox, SLOT(reject()));

    int ret = msgBox->exec();

    switch(msgBox->result())
    {
      case QDialog::Rejected:
        emit PushStep1ToDB_aborted();
        return;
      case QDialog::Accepted:
        NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushStep1ToDB_start: "
           << QString("Push the following information to database:\n\tModule:\t\t%1\n\tBaseplate:\t%2\n\tMaPSA:\t\t%3\n\tGlue:\t\t%4\n\tComment:\t%5").arg(Module_ID_).arg(Baseplate_ID_).arg(MaPSA_ID_).arg(Glue1_ID_).arg(comment_lin->toPlainText()).toStdString();

        if(!(database_->MaPSA_to_BP(MaPSA_ID_, Baseplate_ID_, Glue1_ID_, comment_lin->toPlainText())))
        {
            NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushStep1ToDB_start: Something went wrong.";
            emit PushStep1ToDB_aborted();
            return;
        }

        emit PushStep1ToDB_finished();
        break;
      default:
        emit PushStep1ToDB_aborted();
        return;
    }
}

void AssemblyAssemblyV2::PushStep2ToDB_start()
{
    if(Module_ID_.isEmpty() || PSS_ID_.isEmpty() || Glue2_ID_.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information for Database Upload missing"));
        QString msg = QString("The following IDs are missing:") + (Module_ID_.isEmpty() ? "\n\tModule ID" : "") + (PSS_ID_.isEmpty() ? "\n\tPSS ID" : "") + (Glue2_ID_.isEmpty() ? "\n\tGlue 2 ID" : "");
        msgBox.setText(msg);
        msgBox.setInformativeText("Please add this information via the toolbar or assembly actions.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        int ret = msgBox.exec();

        emit PushStep2ToDB_aborted();
        return;
    }

    QDialog* msgBox = new QDialog();
    msgBox->setWindowTitle(tr("Push Module Information for PSS to Spacers assembly to Database"));

    QVBoxLayout* vlay = new QVBoxLayout();
    msgBox->setLayout(vlay);

    QLabel* main_txt = new QLabel(QString("Push the following information to database:\n\n\tModule:\t%1\n\tPSS:\t%2\n\tGlue:\t%3").arg(Module_ID_).arg(PSS_ID_).arg(Glue2_ID_));
    vlay->addWidget(main_txt);

    QHBoxLayout* comment_lay = new QHBoxLayout();

    QLabel* comment_lab = new QLabel("Comments:");
    QTextEdit* comment_lin = new QTextEdit("");
    comment_lin->setTabChangesFocus(true);
    comment_lin->setFixedHeight(60);

    comment_lay->addWidget(comment_lab);
    comment_lay->addWidget(comment_lin);

    vlay->addLayout(comment_lay);

    QLabel* info_txt = new QLabel("Do you want to push this information to the Database?");
    vlay->addWidget(info_txt);

    QDialogButtonBox* button_box = new QDialogButtonBox(Qt::Horizontal);
    button_box->addButton(QDialogButtonBox::Yes);
    button_box->addButton(QDialogButtonBox::No);
    button_box->setCenterButtons(true);

    vlay->addWidget(button_box);

    connect(button_box, SIGNAL(accepted()), msgBox, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), msgBox, SLOT(reject()));

    int ret = msgBox->exec();

    switch(msgBox->result())
    {
      case QDialog::Rejected:
        emit PushStep2ToDB_aborted();
        return;
      case QDialog::Accepted:
        NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushStep2ToDB_start: "
           << QString("Push the following information to database:\n\tModule:\t\t%1\n\tPSS:\t\t%2\n\tGlue:\t\t%3\n\tComment:\t%4").arg(Module_ID_).arg(PSS_ID_).arg(Glue2_ID_).arg(comment_lin->toPlainText()).toStdString();

        if(!(database_->PSs_to_spacers(PSS_ID_, Glue2_ID_, comment_lin->toPlainText())))
        {
            NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushStep2ToDB_start: Something went wrong.";
            emit PushStep2ToDB_aborted();
            return;
        }

        emit PushStep2ToDB_finished();
        break;
      default:
        emit PushStep2ToDB_aborted();
        return;
    }
}

void AssemblyAssemblyV2::PushStep3ToDB_start()
{
    if(Module_ID_.isEmpty() || Glue3_ID_.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information for Database Upload missing"));
        QString msg = QString("The following IDs are missing:") + (Module_ID_.isEmpty() ? "\n\tModule ID" : "") + (Glue3_ID_.isEmpty() ? "\n\tGlue 3 ID" : "");
        msgBox.setText(msg);
        msgBox.setInformativeText("Please add this information via the toolbar or assembly actions.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        int ret = msgBox.exec();

        emit PushStep3ToDB_aborted();
        return;
    }

    QDialog* msgBox = new QDialog();
    msgBox->setWindowTitle(tr("Push Module Information for PSS to MaPSA assembly to Database"));

    QVBoxLayout* vlay = new QVBoxLayout();
    msgBox->setLayout(vlay);

    QLabel* main_txt = new QLabel(QString("Push the following information to database:\n\n\tModule:\t%1\n\tGlue:\t%2").arg(Module_ID_).arg(Glue3_ID_));
    vlay->addWidget(main_txt);

    QHBoxLayout* comment_lay = new QHBoxLayout();

    QLabel* comment_lab = new QLabel("Comments:");
    QTextEdit* comment_lin = new QTextEdit("");
    comment_lin->setTabChangesFocus(true);
    comment_lin->setFixedHeight(60);

    comment_lay->addWidget(comment_lab);
    comment_lay->addWidget(comment_lin);

    vlay->addLayout(comment_lay);

    QLabel* info_txt = new QLabel("Do you want to push this information to the Database?");
    vlay->addWidget(info_txt);

    QDialogButtonBox* button_box = new QDialogButtonBox(Qt::Horizontal);
    button_box->addButton(QDialogButtonBox::Yes);
    button_box->addButton(QDialogButtonBox::No);
    button_box->setCenterButtons(true);

    vlay->addWidget(button_box);

    connect(button_box, SIGNAL(accepted()), msgBox, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), msgBox, SLOT(reject()));

    int ret = msgBox->exec();

    switch(msgBox->result())
    {
      case QDialog::Rejected:
        emit PushStep3ToDB_aborted();
        return;
      case QDialog::Accepted:
        NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushStep3ToDB_start: "
           << QString("Push the following information to database:\n\tModule:\t\t%1\n\tGlue:\t\t%2\n\tComment:\t%3").arg(Module_ID_).arg(Glue3_ID_).arg(comment_lin->toPlainText()).toStdString();

        if(!(database_->PSs_to_MaPSA(Glue3_ID_, comment_lin->toPlainText())))
        {
            NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PushStep3ToDB_start: Something went wrong.";
            emit PushStep3ToDB_aborted();
            return;
        }

        emit PushStep3ToDB_finished();
        break;
      default:
        emit PushStep3ToDB_aborted();
        return;
    }
}

// ----------------------------------------------------------------------------------------------------
// GoToSensorMarkerPreAlignment -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::GoToSensorMarkerPreAlignment_start(bool isMapsa)
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "GoToSensorMarkerPreAlignment_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double x0 = config_->getValue<double>("parameters", "RefPointSensor_X");
  const double y0 = config_->getValue<double>("parameters", "RefPointSensor_Y");
  const double z0 = config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
   + (isMapsa ? config_->getValue<double>("parameters", "Thickness_PSP") : config_->getValue<double>("parameters", "Thickness_PSS"));
  const double a0 = config_->getValue<double>("parameters", "RefPointSensor_A");

  connect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToSensorMarkerPreAlignment_start"
     << ": emitting signal \"move_absolute_request(" << x0 << ", " << y0 << ", " << z0 << ", " << a0 << ")\"";

  emit move_absolute_request(x0, y0, z0, a0);
}

void AssemblyAssemblyV2::GoToSensorMarkerPreAlignment_finish()
{
  disconnect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToSensorMarkerPreAlignment_finish"
     << ": emitting signal \"GoToSensorMarkerPreAlignment_finished\"";

  emit GoToSensorMarkerPreAlignment_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "GoToSensorMarkerPreAlignment_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to sensor's reference marker (pre-alignment)]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumPickupTool -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::EnableVacuumPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "EnableVacuumPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "EnableVacuumPickupTool_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_pickup_ << ")\"";

  emit vacuum_ON_request(vacuum_pickup_);
}

void AssemblyAssemblyV2::EnableVacuumPickupTool_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "EnableVacuumPickupTool_finish"
     << ": emitting signal \"EnableVacuumPickupTool_finished\"";

  emit EnableVacuumPickupTool_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "EnableVacuumPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable pickup tool vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumPickupTool ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::DisableVacuumPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "DisableVacuumPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "DisableVacuumPickupTool_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_pickup_ << ")\"";

  emit vacuum_OFF_request(vacuum_pickup_);
}

void AssemblyAssemblyV2::DisableVacuumPickupTool_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "DisableVacuumPickupTool_finish"
     << ": emitting signal \"DisableVacuumPickupTool_finished\"";

  emit DisableVacuumPickupTool_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "DisableVacuumPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable pickup tool vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::EnableVacuumSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "EnableVacuumSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "EnableVacuumSpacers_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_spacer_ << ")\"";

  emit vacuum_ON_request(vacuum_spacer_);
}

void AssemblyAssemblyV2::EnableVacuumSpacers_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "EnableVacuumSpacers_finish"
     << ": emitting signal \"EnableVacuumSpacers_finished\"";

  emit EnableVacuumSpacers_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "EnableVacuumSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable spacers vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumSpacers -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::DisableVacuumSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "DisableVacuumSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "DisableVacuumSpacers_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_spacer_ << ")\"";

  emit vacuum_OFF_request(vacuum_spacer_);
}

void AssemblyAssemblyV2::DisableVacuumSpacers_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "DisableVacuumSpacers_finish"
     << ": emitting signal \"DisableVacuumSpacers_finished\"";

  emit DisableVacuumSpacers_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "DisableVacuumSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable spacers vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumBaseplate ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::EnableVacuumBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "EnableVacuumBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumBaseplate_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "EnableVacuumBaseplate_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_basepl_ << ")\"";

  emit vacuum_ON_request(vacuum_basepl_);
}

void AssemblyAssemblyV2::EnableVacuumBaseplate_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "EnableVacuumBaseplate_finish"
     << ": emitting signal \"EnableVacuumBaseplate_finished\"";

  emit EnableVacuumBaseplate_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "EnableVacuumBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable baseplate vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumBaseplate -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::DisableVacuumBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "DisableVacuumBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumBaseplate_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "DisableVacuumBaseplate_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_basepl_ << ")\"";

  emit vacuum_OFF_request(vacuum_basepl_);
}

void AssemblyAssemblyV2::DisableVacuumBaseplate_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "DisableVacuumBaseplate_finish"
     << ": emitting signal \"DisableVacuumBaseplate_finished\"";

  emit DisableVacuumBaseplate_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "DisableVacuumBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable baseplate vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoFromSensorMarkerToPickupXY -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::GoFromSensorMarkerToPickupXY_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "GoFromSensorMarkerToPickupXY_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dX");
  const double dy0 = config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::GoFromSensorMarkerToPickupXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_finish"
     << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

  emit GoFromSensorMarkerToPickupXY_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "GoFromSensorMarkerToPickupXY_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move from sensor's reference marker to pickup XY position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPickupToolOntoMaPSA ---------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LowerPickupToolOntoMaPSA_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPickupToolOntoMaPSA_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerPickupToolOntoMaPSA)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPickupToolOntoMaPSA_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerPickupToolOntoMaPSA = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;
    const double dz0 =
        config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_MPA") //NB: focused on PS-p marker --> to get correct distance to MaPSA, only need to account for additional width due to MPAs (not that of PS-p itself)
    ;
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoMaPSA_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPickupToolOntoMaPSA_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LowerPickupToolOntoMaPSA_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::LowerPickupToolOntoMaPSA_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPickupToolOntoMaPSA_finish"
       << ": emitting signal \"LowerPickupToolOntoMaPSA_finished\"";

    emit LowerPickupToolOntoMaPSA_finished();

    return;
  }
}

void AssemblyAssemblyV2::LowerPickupToolOntoMaPSA_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoMaPSA_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPickupToolOntoMaPSA_finish"
     << ": emitting signal \"LowerPickupToolOntoMaPSA_finished\"";

  emit LowerPickupToolOntoMaPSA_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LowerPickupToolOntoMaPSA_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower pickup tool onto MaPSA]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPickupToolOntoPSS -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LowerPickupToolOntoPSS_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPickupToolOntoPSS_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerPickupToolOntoPSS)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPickupToolOntoPSS_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerPickupToolOntoPSS = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;
    const double dz0 = config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ");
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoPSS_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPickupToolOntoPSS_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LowerPickupToolOntoPSS_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::LowerPickupToolOntoPSS_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPickupToolOntoPSS_finish"
       << ": emitting signal \"LowerPickupToolOntoPSS_finished\"";

    emit LowerPickupToolOntoPSS_finished();

    return;
  }
}

void AssemblyAssemblyV2::LowerPickupToolOntoPSS_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoPSS_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPickupToolOntoPSS_finish"
     << ": emitting signal \"LowerPickupToolOntoPSS_finished\"";

  emit LowerPickupToolOntoPSS_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LowerPickupToolOntoPSS_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower pickup tool onto PS-s]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupMaPSA ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::PickupMaPSA_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "PickupMaPSA_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "PickupMaPSA_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupMaPSA_finish"
       << ": emitting signal \"PickupMaPSA_finished\"";

    emit PickupMaPSA_finished();

    return;
  }

  if(use_smartMove_) {
      connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
      connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupMaPSA_finish()));
  } else {
      connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
      connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupMaPSA_finish()));
  }

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupMaPSA_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::PickupMaPSA_finish()
{
  if(use_smartMove_) {
      disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
      disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupMaPSA_finish()));
  } else {
      disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
      disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupMaPSA_finish()));
  }

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupMaPSA_finish"
     << ": emitting signal \"PickupMaPSA_finished\"";

  emit PickupMaPSA_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "PickupMaPSA_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupPSS ------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::PickupPSS_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "PickupPSS_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "PickupPSS_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupPSS_finish"
       << ": emitting signal \"PickupPSS_finished\"";

    emit PickupPSS_finished();

    return;
  }

  if(use_smartMove_) {
      connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
      connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupPSS_finish()));
  } else {
      connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
      connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));
  }

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupPSS_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::PickupPSS_finish()
{
  if(use_smartMove_) {
      disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
      disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupPSS_finish()));
  } else {
      disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
      disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));
  }

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupPSS_finish"
     << ": emitting signal \"PickupPSS_finished\"";

  emit PickupPSS_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "PickupPSS_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Pick up PS-s]");
}
// ----------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
// GoToXYAPositionToGlueMaPSAToBaseplate --------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::GoToXYAPositionToGlueMaPSAToBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "GoToXYAPositionToGlueMaPSAToBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 =
     config_->getValue<double>("parameters", "RefPointPlatform_X")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToPSPEdge_dX")
   + config_->getValue<double>("parameters", "FromPSPEdgeToPSPRefPoint_dX")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dX")
   - motion_->get_position_X();

  const double dy0 =
     config_->getValue<double>("parameters", "RefPointPlatform_Y")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToPSPEdge_dY")
   + config_->getValue<double>("parameters", "FromPSPEdgeToPSPRefPoint_dY")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dY")
   - motion_->get_position_Y();

  const double dz0 = 0.0;

  const double da0 =
     config_->getValue<double>("parameters", "RefPointPlatform_A")
   - motion_->get_position_A();

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGlueMaPSAToBaseplate_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToXYAPositionToGlueMaPSAToBaseplate_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::GoToXYAPositionToGlueMaPSAToBaseplate_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGlueMaPSAToBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToXYAPositionToGlueMaPSAToBaseplate_finish"
     << ": emitting signal \"GoToXYAPositionToGlueMaPSAToBaseplate_finished\"";

  emit GoToXYAPositionToGlueMaPSAToBaseplate_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "GoToXYAPositionToGlueMaPSAToBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to XYA position for gluing MaPSA to baseplate]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerMaPSAOntoBaseplate ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LowerMaPSAOntoBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerMaPSAOntoBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerMaPSAOntoBaseplate)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerMaPSAOntoBaseplate_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerMaPSAOntoBaseplate = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
      + config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_MPA")
      + config_->getValue<double>("parameters", "Thickness_PSP")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Baseplate")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerMaPSAOntoBaseplate_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerMaPSAOntoBaseplate_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LowerMaPSAOntoBaseplate_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::LowerMaPSAOntoBaseplate_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerMaPSAOntoBaseplate_finish"
       << ": emitting signal \"LowerMaPSAOntoBaseplate_finished\"";

    emit LowerMaPSAOntoBaseplate_finished();

    return;
  }
}

void AssemblyAssemblyV2::LowerMaPSAOntoBaseplate_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerMaPSAOntoBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerMaPSAOntoBaseplate_finish"
     << ": emitting signal \"LowerMaPSAOntoBaseplate_finished\"";

  emit LowerMaPSAOntoBaseplate_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LowerMaPSAOntoBaseplate_finish"
     << ": assembly-step completed";

 emit DBLogMessage("== Assembly step completed : [Lower MaPSA onto baseplate]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToXYAPositionToGluePSSToSpacers ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::GoToXYAPositionToGluePSSToSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "GoToXYAPositionToGluePSSToSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 =
     config_->getValue<double>("parameters", "RefPointPlatform_X")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToSpacerEdge_dX")
   + config_->getValue<double>("parameters", "FromSpacerEdgeToPSSRefPoint_dX")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dX")
   - motion_->get_position_X();

  const double dy0 =
     config_->getValue<double>("parameters", "RefPointPlatform_Y")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToSpacerEdge_dY")
   + config_->getValue<double>("parameters", "FromSpacerEdgeToPSSRefPoint_dY")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dY")
   - motion_->get_position_Y();

  const double dz0 = 0.0;

  const double da0 =
     config_->getValue<double>("parameters", "RefPointPlatform_A")
   - motion_->get_position_A();

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGluePSSToSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::GoToXYAPositionToGluePSSToSpacers_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGluePSSToSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_finish"
     << ": emitting signal \"GoToXYAPositionToGluePSSToSpacers_finished\"";

  emit GoToXYAPositionToGluePSSToSpacers_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "GoToXYAPositionToGluePSSToSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to XYA position for gluing PS-s to spacers]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSOntoSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LowerPSSOntoSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPSSOntoSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerPSSOntoSpacers)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPSSOntoSpacers_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerPSSOntoSpacers = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    bool use_spacer_clamp = GetAssemblyCenter()==assembly::Center::FNAL || GetAssemblyCenter()==assembly::Center::BROWN;
    const double dz0 =
        config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
      - config_->getValue<double>("parameters", "Depth_SpacerSlots")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + (use_spacer_clamp ? config_->getValue<double>("parameters", "Thickness_SpacerClamp") : 0)
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSOntoSpacers_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSOntoSpacers_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LowerPSSOntoSpacers_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::LowerPSSOntoSpacers_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSOntoSpacers_finish"
       << ": emitting signal \"LowerPSSOntoSpacers_finished\"";

    emit LowerPSSOntoSpacers_finished();

    return;
  }
}

void AssemblyAssemblyV2::LowerPSSOntoSpacers_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSOntoSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSOntoSpacers_finish"
     << ": emitting signal \"LowerPSSOntoSpacers_finished\"";

  emit LowerPSSOntoSpacers_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LowerPSSOntoSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower PS-s onto spacers]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToPSPMarkerIdealPosition -------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::GoToPSPMarkerIdealPosition_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "GoToPSPMarkerIdealPosition_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double x0 =
     config_->getValue<double>("parameters", "RefPointPlatform_X")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToPSPEdge_dX")
   + config_->getValue<double>("parameters", "FromPSPEdgeToPSPRefPoint_dX");

  const double y0 =
     config_->getValue<double>("parameters", "RefPointPlatform_Y")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToPSPEdge_dY")
   + config_->getValue<double>("parameters", "FromPSPEdgeToPSPRefPoint_dY");

  const double z0 =
     config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
   + config_->getValue<double>("parameters", "Thickness_PSP") //NB: don't need to account for thickness of MPAs here, since we simply just to focus on the PS-p marker (no pickup)
   + config_->getValue<double>("parameters", "Thickness_GlueLayer")
   + config_->getValue<double>("parameters", "Thickness_Baseplate");

  const double a0 = config_->getValue<double>("parameters", "RefPointPlatform_A");

  connect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToPSPMarkerIdealPosition_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToPSPMarkerIdealPosition_start"
     << ": emitting signal \"move_absolute_request(" << x0 << ", " << y0 << ", " << z0 << ", " << a0 << ")\"";

  emit move_absolute_request(x0, y0, z0, a0);
}

void AssemblyAssemblyV2::GoToPSPMarkerIdealPosition_finish()
{
  disconnect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToPSPMarkerIdealPosition_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoToPSPMarkerIdealPosition_finish"
     << ": emitting signal \"GoToPSPMarkerIdealPosition_finished\"";

  emit GoToPSPMarkerIdealPosition_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "GoToPSPMarkerIdealPosition_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// ApplyPSPToPSSXYOffset ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::ApplyPSPToPSSXYOffset_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ApplyPSPToPSSXYOffset_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dX");
  const double dy0 = config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(ApplyPSPToPSSXYOffset_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ApplyPSPToPSSXYOffset_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::ApplyPSPToPSSXYOffset_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(ApplyPSPToPSSXYOffset_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ApplyPSPToPSSXYOffset_finish"
     << ": emitting signal \"ApplyPSPToPSSXYOffset_finished\"";

  emit ApplyPSPToPSSXYOffset_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "ApplyPSPToPSSXYOffset_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move by PS-s/PS-p relative XY offset]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Make Space in Z -----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::MakeSpaceOnPlatform_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "MakeSpaceOnPlatform_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = makespace_Y_;
  const double dz0 = makespace_Z_;
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "MakeSpaceOnPlatform_start"
       << ": invalid (non-positive) value for vertical upward movement for makespace (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "MakeSpaceOnPlatform_start"
       << ": emitting signal \"MakeSpaceOnPlatform_finished\"";

    emit MakeSpaceOnPlatform_finished();

    return;
  }

  position_y_before_makespace_ = motion_->get_position_Y();
  position_z_before_makespace_ = motion_->get_position_Z();
  position_before_makespace_stored_ = true;
  NQLog("AssemblyAssemblyV2", NQLog::Message) << "MakeSpaceOnPlatform_start"
     << ": Position of stage before making space on the platform is (y/z): " << position_y_before_makespace_ << " / " << position_z_before_makespace_;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(MakeSpaceOnPlatform_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "MakeSpaceOnPlatform_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::MakeSpaceOnPlatform_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(MakeSpaceOnPlatform_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "MakeSpaceOnPlatform_finish"
     << ": emitting signal \"LiftUpPickupTool_finished\"";

  emit MakeSpaceOnPlatform_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "MakeSpaceOnPlatform_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Make space on platform]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Return to initial position -----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::ReturnToPlatform_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ReturnToPlatform_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(!position_before_makespace_stored_)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ReturnToPlatform_start"
       << ": stage position before making space on platform has not been stored. Cannot continue.";
    QMessageBox::warning(0, tr("[LStepExpressMotionManager]"),
       QString("Stage position before making space on platform has not been stored. Cannot continue."),
       QMessageBox::Abort
    );
    return;
  }

  const double dx0 = 0.0;
  const double dy0 = position_y_before_makespace_ - motion_->get_position_Y();
  const double dz0 = position_z_before_makespace_ - motion_->get_position_Z();
  const double da0 = 0.0;

  if(dz0 >= 0.)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "ReturnToPlatform_start"
       << ": invalid (non-negative) value for vertical downward movement for makespace (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPlatform_start"
       << ": emitting signal \"ReturnToPlatform_finished\"";

    emit ReturnToPlatform_finished();

    return;
  }

  QMessageBox* msgBox = new QMessageBox;
  msgBox->setInformativeText(QString("This will move the y / z stages to the absolute position %1 / %2 (relative: %3 / %4).\nContinue?").arg(position_y_before_makespace_).arg(position_z_before_makespace_).arg(dy0).arg(dz0));
  msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox->setDefaultButton(QMessageBox::Yes);
  int ret = msgBox->exec();
  switch(ret)
  {
    case QMessageBox::No:
      NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ReturnToPlatform_start"
         << ": abort motion after user decision.";
      NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPlatform_start"
            << ": emitting signal \"ReturnToPlatform_finished\"";
      emit ReturnToPlatform_finished();
      return;

    case QMessageBox::Yes:
      NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ReturnToPlatform_start"
       << ": start motion after user decision.";
      break;
    default:
      NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ReturnToPlatform_start"
         << ": Invalid user selection. Finish this step.";
      NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPlatform_start"
         << ": emitting signal \"ReturnToPlatform_finished\"";
      emit ReturnToPlatform_finished();
      return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(ReturnToPlatform_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPlatform_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::ReturnToPlatform_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(ReturnToPlatform_finish()));

  if(in_action_){ in_action_ = false; }

  position_before_makespace_stored_ = false;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPlatform_finish"
     << ": emitting signal \"ReturnToPlatform_finished\"";

  emit ReturnToPlatform_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "ReturnToPlatform_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Return to platform]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// RegisterPSSPlusSpacersToMaPSAPosition --------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::RegisterPSSPlusSpacersToMaPSAPosition_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "RegisterPSSPlusSpacersToMaPSAPosition_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  PSSPlusSpacersToMaPSAPosition_X_ = motion_->get_position_X();
  PSSPlusSpacersToMaPSAPosition_Y_ = motion_->get_position_Y();
  PSSPlusSpacersToMaPSAPosition_Z_ = motion_->get_position_Z();
  PSSPlusSpacersToMaPSAPosition_A_ = motion_->get_position_A();

  PSSPlusSpacersToMaPSAPosition_isRegistered_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "RegisterPSSPlusSpacersToMaPSAPosition_start"
     << ": registered position (x=" << PSSPlusSpacersToMaPSAPosition_X_ << ", y=" << PSSPlusSpacersToMaPSAPosition_Y_
     << ", z=" << PSSPlusSpacersToMaPSAPosition_Z_ << ", a=" << PSSPlusSpacersToMaPSAPosition_A_ << ")";

  connect(this, SIGNAL(PSSPlusSpacersToMaPSAPosition_registered()), this, SLOT(RegisterPSSPlusSpacersToMaPSAPosition_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "RegisterPSSPlusSpacersToMaPSAPosition_start"
     << ": emitting signal \"PSSPlusSpacersToMaPSAPosition_registered\"";

  emit PSSPlusSpacersToMaPSAPosition_registered();
}

void AssemblyAssemblyV2::RegisterPSSPlusSpacersToMaPSAPosition_finish()
{
  disconnect(this, SIGNAL(PSSPlusSpacersToMaPSAPosition_registered()), this, SLOT(RegisterPSSPlusSpacersToMaPSAPosition_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "RegisterPSSPlusSpacersToMaPSAPosition_finish"
     << ": emitting signal \"RegisterPSSPlusSpacersToMaPSAPosition_finished\"";

  emit RegisterPSSPlusSpacersToMaPSAPosition_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "RegisterPSSPlusSpacersToMaPSAPosition_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Register {PS-s + spacers} Z position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY -----------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = config_->getValue<double>("parameters", "FromPSSPlusSpacersToMaPSAPositionToGluingStage_dX");
    const double dy0 = config_->getValue<double>("parameters", "FromPSSPlusSpacersToMaPSAPositionToGluingStage_dY");
    const double dz0 = 0.0;
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish"
       << ": emitting signal \"GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished\"";

    emit GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished();

    return;
  }
}

void AssemblyAssemblyV2::GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish"
     << ": emitting signal \"GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished\"";

  emit GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move from {PS-s + spacers --> MaPSA + baseplate} position to gluing stage reference XY position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSPlusSpacersOntoGluingStage -----------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LowerPSSPlusSpacersOntoGluingStage_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPSSPlusSpacersOntoGluingStage_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerPSSPlusSpacersOntoGluingStage)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPSSPlusSpacersOntoGluingStage_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerPSSPlusSpacersOntoGluingStage = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "CameraFocusOnGluingStage_Z")
      + config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoGluingStage_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSPlusSpacersOntoGluingStage_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LowerPSSPlusSpacersOntoGluingStage_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::LowerPSSPlusSpacersOntoGluingStage_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSPlusSpacersOntoGluingStage_finish"
       << ": emitting signal \"LowerPSSPlusSpacersOntoGluingStage_finished\"";

    emit LowerPSSPlusSpacersOntoGluingStage_finished();

    return;
  }
}

void AssemblyAssemblyV2::LowerPSSPlusSpacersOntoGluingStage_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoGluingStage_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSPlusSpacersOntoGluingStage_finish"
     << ": emitting signal \"LowerPSSPlusSpacersOntoGluingStage_finished\"";

  emit LowerPSSPlusSpacersOntoGluingStage_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LowerPSSPlusSpacersOntoGluingStage_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower {PS-s + spacers} onto gluing stage]");
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
// SlowlyLiftFromGluingStage --------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::SlowlyLiftFromGluingStage_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "SlowlyLiftFromGluingStage_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {

    original_Z_velocity_ = motion_->get_velocity_Z();

    double slowVelocity = 0.5; // mm/s
    motion_->set_velocity_Z(slowVelocity);

    while(slowVelocity != motion_->get_velocity_Z())
    {
      sleep(1);
    }

    const double dx0 = 0.;
    const double dy0 = 0.;
    const double dz0 = 5.;
    const double da0 = 0.;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(SlowlyLiftFromGluingStage_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "SlowlyLiftFromGluingStage_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "SlowlyLiftFromGluingStage_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::SlowlyLiftFromGluingStage_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "SlowlyLiftFromGluingStage_finish"
       << ": emitting signal \"SlowlyLiftFromGluingStage_finished\"";

    emit SlowlyLiftFromGluingStage_finished();

    return;
  }
}

void AssemblyAssemblyV2::SlowlyLiftFromGluingStage_finish()
{
  motion_->set_velocity_Z(original_Z_velocity_);
  while(original_Z_velocity_ != motion_->get_velocity_Z())
  {
    sleep(1);
  }

  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(SlowlyLiftFromGluingStage_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "SlowlyLiftFromGluingStage_finish"
     << ": emitting signal \"SlowlyLiftFromGluingStage_finished\"";

  emit SlowlyLiftFromGluingStage_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "SlowlyLiftFromGluingStage_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Slowly lifted PSS + spacers from gluing station]");
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
// ReturnToPSSPlusSpacersToMaPSAPosition --------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::ReturnToPSSPlusSpacersToMaPSAPosition_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(PSSPlusSpacersToMaPSAPosition_isRegistered_ == false)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": \"PSS+Spacers To MaPSA\" position not registered, but mandatory for this step (see button in \"Assembly\" tab)";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::ReturnToPSSPlusSpacersToMaPSAPosition_start -- \"PSS+Spacers To MaPSA\" position not registered, but mandatory for this step (see button in \"Assembly\" tab)"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
       << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

    emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = PSSPlusSpacersToMaPSAPosition_X_ - motion_->get_position_X();
    const double dy0 = PSSPlusSpacersToMaPSAPosition_Y_ - motion_->get_position_Y();
    const double dz0 = PSSPlusSpacersToMaPSAPosition_Z_ - motion_->get_position_Z();
    const double da0 = 0.;

    if(dz0 <= 0.)
    {
      NQLog("AssemblyAssemblyV2", NQLog::Critical) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
         << ": invalid (non-positive) value for vertical upward movement (dz=" << dz0 << "), no action taken";

      NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
         << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

      emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

      return;
    }

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(ReturnToPSSPlusSpacersToMaPSAPosition_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::ReturnToPSSPlusSpacersToMaPSAPosition_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
       << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

    emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

    return;
  }
}

void AssemblyAssemblyV2::ReturnToPSSPlusSpacersToMaPSAPosition_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(ReturnToPSSPlusSpacersToMaPSAPosition_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
     << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

  emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move {PS-s + spacers} from gluing stage to motion stage for gluing to {MaPSA + baseplate}]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSPlusSpacersOntoMaPSA -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LowerPSSPlusSpacersOntoMaPSA_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPSSPlusSpacersOntoMaPSA_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerPSSPlusSpacersOntoMaPSA)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LowerPSSPlusSpacersOntoMaPSA_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerPSSPlusSpacersOntoMaPSA = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_MPA"); //NB: focused on PS-p marker --> to get correct distance to MaPSA, only need to account for additional width due to MPAs (not that of PS-p itself)

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoMaPSA_finish()));

    in_action_ = true;

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSPlusSpacersOntoMaPSA_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LowerPSSPlusSpacersOntoMaPSA_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssemblyV2::LowerPSSPlusSpacersOntoMaPSA_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSPlusSpacersOntoMaPSA_finish"
       << ": emitting signal \"LowerPSSPlusSpacersOntoMaPSA_finished\"";

    emit LowerPSSPlusSpacersOntoMaPSA_finished();

    return;
  }
}

void AssemblyAssemblyV2::LowerPSSPlusSpacersOntoMaPSA_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoMaPSA_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LowerPSSPlusSpacersOntoMaPSA_finish"
     << ": emitting signal \"LowerPSSPlusSpacersOntoMaPSA_finished\"";

  emit LowerPSSPlusSpacersOntoMaPSA_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LowerPSSPlusSpacersOntoMaPSA_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower {PS-s + spacers} onto {MaPSA + baseplate}]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupPSSPlusSpacers -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::PickupPSSPlusSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "PickupPSSPlusSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "PickupPSSPlusSpacers_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupPSSPlusSpacers_finish"
       << ": emitting signal \"PickupPSSPlusSpacers_finished\"";

    emit PickupPSSPlusSpacers_finished();

    return;
  }

  if(use_smartMove_){
      connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
      connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupPSSPlusSpacers_finish()));
  } else {
      connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
      connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSSPlusSpacers_finish()));
  }

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupPSSPlusSpacers_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::PickupPSSPlusSpacers_finish()
{
  if(use_smartMove_){
      disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
      disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupPSSPlusSpacers_finish()));
  } else {
      disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
      disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSSPlusSpacers_finish()));
  }

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "PickupPSSPlusSpacers_finish"
     << ": emitting signal \"PickupPSSPlusSpacers_finished\"";

  emit PickupPSSPlusSpacers_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "PickupPSSPlusSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Pickup {PS-s + spacers}]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LiftUpPickupTool -----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::LiftUpPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "LiftUpPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup2_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssemblyV2", NQLog::Critical) << "LiftUpPickupTool_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #2 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LiftUpPickupTool_finish"
       << ": emitting signal \"LiftUpPickupTool_finished\"";

    emit LiftUpPickupTool_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(LiftUpPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LiftUpPickupTool_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssemblyV2::LiftUpPickupTool_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(LiftUpPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "LiftUpPickupTool_finish"
     << ": emitting signal \"LiftUpPickupTool_finished\"";

  emit LiftUpPickupTool_finished();

  NQLog("AssemblyAssemblyV2", NQLog::Message) << "LiftUpPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lift up pickup tool]");
}
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::AssemblyCompleted_start()
{
  if(in_action_){

    NQLog("AssemblyAssemblyV2", NQLog::Warning) << "AssemblyCompleted_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  QMessageBox* msgBox = new QMessageBox;
  msgBox->setStyleSheet("QLabel{min-width: 300px;}");
  msgBox->setInformativeText("The assembly is completed! Very well done!");

  msgBox->setStandardButtons(QMessageBox::Ok);
  msgBox->setDefaultButton(QMessageBox::Ok);

  int ret = msgBox->exec();

  NQLog("AssemblyAssemblyV2", NQLog::Spam) << "AssemblyCompleted_start called - no action taken";

  emit AssemblyCompleted_finished();
}
// ----------------------------------------------------------------------------------------------------
// switchToAlignmentTab_PSP ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::switchToAlignmentTab_PSP()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "switchToAlignmentTab_PSP"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "switchToAlignmentTab_PSP"
    << ": emitting signal \"switchToAlignmentTab_PSP_request\"";

  emit switchToAlignmentTab_PSP_request(); //Will auto-switch to 'Alignment' sub-tab, and select PSP mode

  in_action_ = false;

  return;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// switchToAlignmentTab_PSS ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssemblyV2::switchToAlignmentTab_PSS()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "switchToAlignmentTab_PSS"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "switchToAlignmentTab_PSS"
    << ": emitting signal \"switchToAlignmentTab_PSS_request\"";

  emit switchToAlignmentTab_PSS_request(); //Will auto-switch to 'Alignment' sub-tab, and select PSS mode

  in_action_ = false;

  return;
}
// ----------------------------------------------------------------------------------------------------
