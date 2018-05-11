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

#include <AssemblyObjectFinderPatRecWidget.h>
#include <AssemblyUtilities.h>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QPixmap>

AssemblyObjectFinderPatRecWidget::AssemblyObjectFinderPatRecWidget(QWidget* parent) :
  QWidget(parent),

  layout_(nullptr),

  templa_load_button_(nullptr),
  templa_file_lineed_(nullptr),

  thresh_thresh_radbu_(nullptr),
  thresh_thresh_linee_(nullptr),

  thresh_adathr_radbu_(nullptr),
  thresh_adathr_linee_(nullptr),

  angles_prescan_label_(nullptr),
  angles_prescan_linee_(nullptr),

  angles_finemax_label_(nullptr),
  angles_finemax_linee_(nullptr),

  angles_finestep_label_(nullptr),
  angles_finestep_linee_(nullptr)
{
  layout_ = new QVBoxLayout;
  this->setLayout(layout_);

  /// Template Image ---------------
  QGroupBox* templa_box = new QGroupBox(tr("Template Image"));
  templa_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QHBoxLayout* templa_lay = new QHBoxLayout;

  templa_load_button_ = new QPushButton(tr("Load Image"));
  templa_file_lineed_ = new QLineEdit(tr(""));
  templa_file_lineed_->setReadOnly(true);

  templa_lay->addWidget(templa_load_button_, 34);
  templa_lay->addWidget(templa_file_lineed_, 66);

  templa_box->setLayout(templa_lay);

  layout_->addWidget(templa_box);
  /// ------------------------------

  /// Master Image Thresholding ----
  QGroupBox* thresh_box = new QGroupBox(tr("Master Image Thresholding"));
  thresh_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QGridLayout* thresh_lay = new QGridLayout;

  thresh_thresh_radbu_ = new QRadioButton(tr("Threshold (int)"));
  thresh_thresh_linee_ = new QLineEdit(tr(""));

  thresh_adathr_radbu_ = new QRadioButton(tr("Adaptive Threshold (odd int)"));
  thresh_adathr_linee_ = new QLineEdit(tr(""));

  connect(thresh_thresh_radbu_, SIGNAL(toggled(bool)), thresh_thresh_linee_, SLOT(setEnabled(bool)));
  connect(thresh_adathr_radbu_, SIGNAL(toggled(bool)), thresh_adathr_linee_, SLOT(setEnabled(bool)));

  thresh_thresh_radbu_->setChecked(true);

  thresh_lay->addWidget(thresh_thresh_radbu_, 0, 0);
  thresh_lay->addWidget(thresh_thresh_linee_, 0, 1);

  thresh_lay->addWidget(thresh_adathr_radbu_, 1, 0);
  thresh_lay->addWidget(thresh_adathr_linee_, 1, 1);

  thresh_box->setLayout(thresh_lay);

  layout_->addWidget(thresh_box);
  /// ------------------------------

  /// Template-Matching Angular Scan
  QGroupBox* angles_box = new QGroupBox(tr("Template-Matching Angular Scan"));
  angles_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QGridLayout* angles_lay = new QGridLayout;

  angles_prescan_label_ = new QLabel(tr("Pre-Scan Angles (list) [deg]"));
  angles_prescan_linee_ = new QLineEdit(tr(""));

  angles_finemax_label_ = new QLabel(tr("Fine-Scan Maximum Angle [deg]"));
  angles_finemax_linee_ = new QLineEdit(tr(""));

  angles_finestep_label_ = new QLabel(tr("Fine-Scan Angular Step [deg]"));
  angles_finestep_linee_ = new QLineEdit(tr(""));

  angles_lay->addWidget(angles_prescan_label_ , 0, 0);
  angles_lay->addWidget(angles_prescan_linee_ , 0, 1);

  angles_lay->addWidget(angles_finemax_label_ , 1, 0);
  angles_lay->addWidget(angles_finemax_linee_ , 1, 1);

  angles_lay->addWidget(angles_finestep_label_, 2, 0);
  angles_lay->addWidget(angles_finestep_linee_, 2, 1);

  angles_box->setLayout(angles_lay);

  layout_->addWidget(angles_box);
  /// ------------------------------
}

void AssemblyObjectFinderPatRecWidget::update_configuration()
{
  AssemblyObjectFinderPatRec::Configuration conf;

  /// Template Image ---------------
  conf.template_filepath_ = templa_file_lineed_->text();
  /// ------------------------------

  /// Master Image Thresholding ----
  if(thresh_thresh_radbu_->isChecked() == thresh_adathr_radbu_->isChecked())
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "update_configuration"
       << ": invalid configuration for master-image thresholding (selected zero or multiple options), no action taken";

    return;
  }
  else if(thresh_thresh_radbu_->isChecked())
  {
    conf.thresholding_useAdaptiveThreshold_ = false;
    conf.thresholding_blocksize_ = -1;

    conf.thresholding_useThreshold_ = true;

    const QString thresh_str = thresh_thresh_linee_->text().remove(" ");

    bool valid_thr(false);
    conf.thresholding_threshold_ = thresh_str.toInt(&valid_thr);

    if(valid_thr == false)
    {
      NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "update_configuration"
         << ": invalid format for threshold value (" << thresh_str << ", not a integer), no action taken";

      return;
    }
  }
  else if(thresh_adathr_radbu_->isChecked())
  {
    conf.thresholding_useThreshold_ = false;
    conf.thresholding_threshold_ = -1;

    conf.thresholding_useAdaptiveThreshold_ = true;

    const QString adathr_str = thresh_adathr_linee_->text().remove(" ");

    bool valid_bks(false);
    conf.thresholding_blocksize_ = adathr_str.toInt(&valid_bks);

    if(valid_bks == false)
    {
      NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "update_configuration"
         << ": invalid format for block-size value (" << adathr_str << ", not a integer), no action taken";

      return;
    }
  }
  /// ------------------------------

  /// Template-Matching Angular Scan
  const QStringList angles_prescan_vstr = angles_prescan_linee_->text().split(" ");

  conf.angles_prescan_vec_.clear();

  for(const auto& ang_str : angles_prescan_vstr)
  {
    bool valid_ang(false);
    const double ang_val = ang_str.toDouble(&valid_ang);

    if(valid_ang == false)
    {
      NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "update_configuration"
         << ": invalid format for pre-scan angle value (" << ang_str << ", not a double), no action taken";

      return;
    }

    conf.angles_prescan_vec_.emplace_back(ang_val);
  }

  const QString angfinemax_str = angles_finemax_linee_->text().remove(" ");

  bool valid_afm(false);
  conf.angles_finemax_ = angfinemax_str.toDouble(&valid_afm);

  if(valid_afm == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "update_configuration"
       << ": invalid format for fine-scan max-angle value (" << angfinemax_str << ", not a double), no action taken";

    return;
  }

  const QString angfinestep_str = angles_finestep_linee_->text().remove(" ");

  bool valid_afs(false);
  conf.angles_finestep_ = angfinestep_str.toDouble(&valid_afs);

  if(valid_afs == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "update_configuration"
       << ": invalid format for fine-scan step-angle value (" << angfinestep_str << ", not a double), no action taken";

    return;
  }
  /// ------------------------------

  configuration_ = conf;

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "update_configuration"
     << ": emitting signal \"configuration_updated\"";

  emit configuration_updated(configuration_);
  emit configuration_updated();
}

//!!void AssemblyObjectFinderPatRecWidget::execute()
//!!{
//!!  int mode_lab(0), mode_obj(0);
//!!
//!!  if     (radio1_->isChecked()){ mode_obj = 0; }
//!!  else if(radio2_->isChecked()){ mode_obj = 1; }
//!!  else if(radio3_->isChecked()){ mode_obj = 2; }
//!!  else if(radio4_->isChecked()){ mode_obj = 3; }
//!!
//!!  if     (radio5_->isChecked()){ mode_lab = 0; }
//!!  else if(radio6_->isChecked()){ mode_lab = 1; }
//!!
//!!  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "execute"
//!!     << ": emitting signal \"mode(" << mode_lab << ", " << mode_obj << ")\"";
//!!
//!!  emit mode(mode_lab, mode_obj);
//!!}
//!!
//!!void AssemblyObjectFinderPatRecWidget::change_label(const int state)
//!!{
//!!  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "change_label(" << state << ")";
//!!
//!!  if(state == 0)
//!!  {
//!!    label_->setText(" FOUND MARKER");
//!!    label_->setStyleSheet("QLabel { background-color : green; color : black; }");
//!!  }
//!!  else if(state == 2)
//!!  {
//!!    label_->setText(" ERROR");
//!!    label_->setStyleSheet("QLabel { background-color : red; color : black; }");
//!!  }
//!!
//!!  return;
//!!}
