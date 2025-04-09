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

#include <AssemblyObjectFinderPatRecWidget.h>
#include <AssemblyUtilities.h>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QPixmap>
#include <QFileDialog>

AssemblyObjectFinderPatRecWidget::AssemblyObjectFinderPatRecWidget(const bool suggest_templates, QWidget* parent) :
  QWidget(parent),

  layout_(nullptr),

  templa_load_button_(nullptr),
  templa_file_linee_ (nullptr),

  thresh_thresh_linee_(nullptr)
{
  layout_ = new QVBoxLayout;
  this->setLayout(layout_);

  /// Template Image ---------------
  QGroupBox* templa_box = new QGroupBox(tr("Template Image"));
  templa_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QGridLayout* templa_lay = new QGridLayout;

  templa_load_button_ = new QPushButton(tr("Load Image"));

  connect(templa_load_button_, SIGNAL(clicked()), this, SLOT(load_image_template()));

  templa_file_linee_ = new QLineEdit(tr(""));
  templa_file_linee_->setReadOnly(true);

  connect(this, SIGNAL(updated_image_template_path(QString)), templa_file_linee_, SLOT(setText(QString)));

  templa_lay->addWidget(templa_load_button_, 0, 0, 1, 2);
  templa_lay->addWidget(templa_file_linee_ , 0, 2, 1, 3);

  if(suggest_templates)
  {
    QLabel* suggest_label = new QLabel("Use template: ");
    suggest_psp1_button_ = new QPushButton(tr("PSP 1"));
    suggest_psp2_button_ = new QPushButton(tr("PSP 2"));
    suggest_pss1_button_ = new QPushButton(tr("PSS 1"));
    suggest_pss2_button_ = new QPushButton(tr("PSS 2"));

    templa_lay->addWidget(suggest_label, 1, 0);
    templa_lay->addWidget(suggest_psp1_button_, 1, 1);
    templa_lay->addWidget(suggest_psp2_button_, 1, 2);
    templa_lay->addWidget(suggest_pss1_button_, 1, 3);
    templa_lay->addWidget(suggest_pss2_button_, 1, 4);

    connect(suggest_psp1_button_, SIGNAL(clicked()), this, SLOT(load_image_template_from_config()));
    connect(suggest_psp2_button_, SIGNAL(clicked()), this, SLOT(load_image_template_from_config()));
    connect(suggest_pss1_button_, SIGNAL(clicked()), this, SLOT(load_image_template_from_config()));
    connect(suggest_pss2_button_, SIGNAL(clicked()), this, SLOT(load_image_template_from_config()));
  }

  auto thresh_thresh_label = new QLabel(tr("Threshold (pos int)"));
  thresh_thresh_linee_ = new QLineEdit(tr(""));

  templa_lay->addWidget(thresh_thresh_label, 2, 0);
  templa_lay->addWidget(thresh_thresh_linee_, 2, 1);

  templa_box->setLayout(templa_lay);

  layout_->addWidget(templa_box);
  /// ------------------------------

  config_ = ApplicationConfig::instance();
}

void AssemblyObjectFinderPatRecWidget::load_image_template()
{
  const QString fpath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));

  this->load_image_template_from_path(fpath);

  return;
}

void AssemblyObjectFinderPatRecWidget::load_image_template_from_path(const QString& filename)
{
  if(filename.isNull() || filename.isEmpty()){ return; }

  const cv::Mat img = assembly::cv_imread(filename, cv::IMREAD_COLOR);

  if(img.empty())
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Warning) << "load_image_template"
       << ": input image is empty, no action taken";

    return;
  }

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "load_image_template"
     << ": emitting signal \"updated_image_template_path(" << filename << ")\"";

  emit updated_image_template_path(filename);

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "load_image_template"
     << ": emitting signal \"updated_image_template\"";

  emit updated_image_template(img);
}

void AssemblyObjectFinderPatRecWidget::load_image_template_from_config()
{
  QPushButton* ptr_qedit = qobject_cast<QPushButton*>(sender());
  std::string filepath = Config::CMSTkModLabBasePath + "/";

  if(ptr_qedit == suggest_psp1_button_)
  {
    filepath += config_->getValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSP1_template_fpath");
  } else if(ptr_qedit == suggest_psp2_button_)
  {
    filepath += config_->getValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSP2_template_fpath");
  } else if(ptr_qedit == suggest_pss1_button_)
  {
    filepath += config_->getValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSS1_template_fpath");
  } else if(ptr_qedit == suggest_pss2_button_)
  {
    filepath += config_->getValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSS2_template_fpath");
  } else
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Fatal) << "load_image_template_from_config"
      << ": Could not update image template - unknown sender of signal";
    return;
  }

  this->load_image_template_from_path(QString::fromStdString(filepath));
}


void AssemblyObjectFinderPatRecWidget::transmit_configuration()
{
  bool valid_conf(false);

  const auto& conf = this->get_configuration(valid_conf);

  if(valid_conf == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object, no action taken";

    return;
  }

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "transmit_configuration"
     << ": emitting signal \"configuration(AssemblyObjectFinderPatRec::Configuration)\"";

  emit configuration(conf);
}

AssemblyObjectFinderPatRec::Configuration AssemblyObjectFinderPatRecWidget::get_configuration(bool& valid_conf) const
{
  AssemblyObjectFinderPatRec::Configuration conf;

  /// Template Image ---------------
  conf.template_filepath_ = templa_file_linee_->text();
  /// ------------------------------

  const QString thresh_str = thresh_thresh_linee_->text().remove(" ");

  bool valid_thr(false);
  conf.thresholding_threshold_ = thresh_str.toInt(&valid_thr);

  if(valid_thr == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
        << ": invalid format for threshold value (" << thresh_str << ", not a integer), no action taken";

    valid_conf = false;

    conf.reset();

    emit invalid_configuration();

    return conf;
  }

  valid_conf = true;

  conf.angles_prescan_vec_ = config_->getValueVector<double>("main", "AssemblyObjectAlignerView_PatRec_angles_prescan");
  conf.angles_finemax_ = config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finemax" , 2);
  conf.angles_finestep_ = config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finestep", 0.2);

  return conf;
}
