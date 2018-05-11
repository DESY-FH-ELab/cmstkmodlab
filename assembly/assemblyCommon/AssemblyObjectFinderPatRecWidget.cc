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

#include <QPixmap>

AssemblyObjectFinderPatRecWidget::AssemblyObjectFinderPatRecWidget(const QString& label, QWidget* parent) :
  QWidget(parent),

  layout_(nullptr),
  button_(nullptr),
  lineed_(nullptr),
  label_ (nullptr),

  groupBox1_(nullptr),
  groupBox2_(nullptr),

  radio1_(nullptr),
  radio2_(nullptr),
  radio3_(nullptr),
  radio4_(nullptr),
  radio5_(nullptr),
  radio6_(nullptr),

  vbox1_(nullptr),
  vbox2_(nullptr),

  sw_angrough_(nullptr),
  sw_angscanp_(nullptr)
{
  layout_ = new QFormLayout;
  this->setLayout(layout_);

  QGridLayout* layout_1 = new QGridLayout;
  layout_->addRow(layout_1);

  button_ = new QPushButton(label, this);
  layout_1->addWidget(button_, 0, 0);

  groupBox1_ = new QGroupBox(tr("Object"));
  groupBox2_ = new QGroupBox(tr("Mode"));

  radio1_ = new QRadioButton(tr("&Fiducial marker"), this);
  radio2_ = new QRadioButton(tr("&Positioning pin"), this);
  radio3_ = new QRadioButton(tr("&Sensor corner")  , this);
  radio4_ = new QRadioButton(tr("&Spacer corner")  , this);

  radio1_->setChecked(true);

  vbox1_ = new QVBoxLayout;
  vbox1_->addWidget(radio1_);
  vbox1_->addWidget(radio2_);
  vbox1_->addWidget(radio3_);
  vbox1_->addWidget(radio4_);
  vbox1_->addStretch(1);

  groupBox1_->setLayout(vbox1_);

  radio5_ = new QRadioButton(tr("&Demo"), this);
  radio6_ = new QRadioButton(tr("&Lab") , this);

  radio5_->setChecked(true);

  vbox2_ = new QVBoxLayout;
  vbox2_->addWidget(radio5_);
  vbox2_->addWidget(radio6_);
  vbox2_->addStretch(1);

  groupBox2_->setLayout(vbox2_);

  layout_1->addWidget(groupBox1_, 1, 0);
  layout_1->addWidget(groupBox2_, 1, 1);

  QPixmap pixmap(100, 100);
  pixmap.fill(QColor("transparent"));

  label_ = new QLabel("", this);
  label_->setPixmap(pixmap);
  label_->setText(" WAITING");
  label_->setStyleSheet("QLabel { background-color : orange; color : black; }");

  layout_1->addWidget(label_, 0, 1);

  // PatRec: angular analysis configuration
  QGridLayout* layout_2 = new QGridLayout;
  layout_->addRow(layout_2);

  // widget: PatRec rough angles
  sw_angrough_ = new AssemblyStringWidget("Pre-Scan Angles (list)", "0,180", this);
  layout_2->addWidget(sw_angrough_->button(), 0, 0);
  layout_2->addWidget(sw_angrough_->lineed(), 0, 1);

  // widget: PatRec angular-scan parameters
  sw_angscanp_ = new AssemblyStringWidget("Scan Params (fine-range, fine-step)", "5,0.125", this);
  layout_2->addWidget(sw_angscanp_->button(), 1, 0);
  layout_2->addWidget(sw_angscanp_->lineed(), 1, 1);
  // ---------------------

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

void AssemblyObjectFinderPatRecWidget::execute()
{
  int mode_lab(0), mode_obj(0);

  if     (radio1_->isChecked()){ mode_obj = 0; }
  else if(radio2_->isChecked()){ mode_obj = 1; }
  else if(radio3_->isChecked()){ mode_obj = 2; }
  else if(radio4_->isChecked()){ mode_obj = 3; }

  if     (radio5_->isChecked()){ mode_lab = 0; }
  else if(radio6_->isChecked()){ mode_lab = 1; }

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "execute"
     << ": emitting signal \"mode(" << mode_lab << ", " << mode_obj << ")\"";

  emit mode(mode_lab, mode_obj);
}

void AssemblyObjectFinderPatRecWidget::change_label(const int state)
{
  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "change_label(" << state << ")";

  if(state == 0)
  {
    label_->setText(" FOUND MARKER");
    label_->setStyleSheet("QLabel { background-color : green; color : black; }");
  }
  else if(state == 2)
  {
    label_->setText(" ERROR");
    label_->setStyleSheet("QLabel { background-color : red; color : black; }");
  }

  return;
}
