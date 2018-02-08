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

#include <AssemblyRegistryView.h>
#include <LStepExpressWidget.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>
#include <Util.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPalette>

AssemblyRegistryView::AssemblyRegistryView(const LStepExpressMotionManager* motion_manager, QWidget* parent) :
  QWidget(parent),
  motion_manager_(motion_manager)
{
  //// INITIALIZATION
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }
  //// --------------

  QFormLayout* l = new QFormLayout;
  this->setLayout(l);

  //// IMAGE
  QGroupBox* box_image = new QGroupBox(tr("Image"));

  QFormLayout* lay_image = new QFormLayout;

  image_widget_ = new AssemblyRegistryImageWidget(this);
  lay_image->addRow(image_widget_);

  box_image->setLayout(lay_image);

  l->addRow(box_image);
  //// ---------------

  //// POSITION(S)
  QGroupBox* box_posit = new QGroupBox(tr("Positions"));

  QFormLayout* l2 = new QFormLayout;
  box_posit->setLayout(l2);

  v_wpos_.clear();
  for(unsigned int i=0; i<6; ++i)
  {
    v_wpos_.emplace_back(new AssemblyRegistryPositionWidget("#"+QString::number(i), motion_manager_));
    l2->addRow(v_wpos_.back());
  }

  pos_calc_lineed_from_ = new QLineEdit("", box_posit);
  pos_calc_lineed_to_   = new QLineEdit("", box_posit);
  pos_calc_lineed_res_  = new QLineEdit("", box_posit);

  pos_calc_lineed_from_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  pos_calc_lineed_from_->setMaximumWidth(50);

  pos_calc_lineed_to_  ->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  pos_calc_lineed_to_  ->setMaximumWidth(50);

  pos_calc_button_ = new QPushButton("Show", box_posit);

  QHBoxLayout* hbox = new QHBoxLayout;
  hbox->addWidget(new QLabel("Get moveRelative from", box_posit));
  hbox->addWidget(pos_calc_lineed_from_);
  hbox->addWidget(new QLabel("to", box_posit));
  hbox->addWidget(pos_calc_lineed_to_);
  hbox->addWidget(new QLabel(" : ", box_posit));
  hbox->addWidget(pos_calc_button_);
  hbox->addWidget(pos_calc_lineed_res_);

  l2->addRow(hbox);

  l->addRow(box_posit);
  //// ---------------

  //// connection(s)
  connect(pos_calc_button_, SIGNAL(clicked()), this, SLOT(calculate_relative_distance()));
  //// ---------------
}

void AssemblyRegistryView::calculate_relative_distance()
{
  // FROM
  QString idx_qstr_from = pos_calc_lineed_from_->text();

  bool idx_from_valid(false);
  const int idx_from = idx_qstr_from.remove(" ").toInt(&idx_from_valid);

  if(!idx_from_valid)
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "calculate_relative_distance"
      << ": invalid input format for index of position \"From\" (" << idx_qstr_from << "), no action taken";

    return;
  }
  else if(idx_from >= int(v_wpos_.size()))
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "calculate_relative_distance"
      << ": invalid input value for index of position \"From\" (out-of-range value, " << idx_from << "), no action taken";

    return;
  }
  else if(idx_from < 0)
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "calculate_relative_distance"
      << ": invalid input value for index of position \"From\" (negative value, " << idx_from << "), no action taken";

    return;
  }

  const QString qstr_from = v_wpos_.at(idx_from)->position_qstring();
  // ----

  // TO
  QString idx_qstr_to = pos_calc_lineed_to_->text();

  bool idx_to_valid(false);
  const int idx_to = idx_qstr_to.remove(" ").toInt(&idx_to_valid);

  if(!idx_to_valid)
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "calculate_relative_distance"
      << ": invalid input format for index of position \"To\" (" << idx_qstr_to << "), no action taken";

    return;
  }
  else if(idx_to >= int(v_wpos_.size()))
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "calculate_relative_distance"
      << ": invalid input format for index of position \"To\" (out-of-range value, " << idx_to << "), no action taken";

    return;
  }
  else if(idx_to < 0)
  {
    NQLog("AssemblyRegistryView", NQLog::Critical) << "calculate_relative_distance"
      << ": invalid input format for index of position \"To\" (negative value, " << idx_to << "), no action taken";

    return;
  }

  const QString qstr_to = v_wpos_.at(idx_to)->position_qstring();
  // ----

  std::vector<double> vpos_from, vpos_to;

  const bool pass_vpos_from = this->load_position_4vector(vpos_from, qstr_from);
  if(!pass_vpos_from){ return; }

  const bool pass_vpos_to   = this->load_position_4vector(vpos_to, qstr_to);
  if(!pass_vpos_to){ return; }

  const double dX = (vpos_to.at(0) - vpos_from.at(0));
  const double dY = (vpos_to.at(1) - vpos_from.at(1));
  const double dZ = (vpos_to.at(2) - vpos_from.at(2));
  const double dA = (vpos_to.at(3) - vpos_from.at(3));

  std::stringstream posi_strs;
  posi_strs << dX << ", " << dY << ", " << dZ << ", " << dA;

  const QString qstr_res = QString::fromStdString(posi_strs.str());

  pos_calc_lineed_res_->setText(qstr_res);

  NQLog("AssemblyRegistryView", NQLog::Spam) << "calculate_relative_distance"
    << ": relative distance " << idx_from << "->" << idx_to << " calculated successfully (" << qstr_res << ")";

  return;
}

bool AssemblyRegistryView::load_position_4vector(std::vector<double>& vec, QString line_entry) const
{
  const QStringList entries = line_entry.remove(" ").split(",");

  if(entries.length() == 4)
  {
    bool valid_x(false);
    bool valid_y(false);
    bool valid_z(false);
    bool valid_a(false);

    const double x_d = entries.value(0).toDouble(&valid_x);
    const double y_d = entries.value(1).toDouble(&valid_y);
    const double z_d = entries.value(2).toDouble(&valid_z);
    const double a_d = entries.value(3).toDouble(&valid_a);

    if(!valid_x)
    {
      NQLog("AssemblyRegistryView", NQLog::Spam) << "load_position_4vector"
         << ": invalid format for x-axis position (" << entries.value(0) << "), returning \"false\"";

      return false;
    }
    else if(!valid_y)
    {
      NQLog("AssemblyRegistryView", NQLog::Spam) << "load_position_4vector"
         << ": invalid format for y-axis position (" << entries.value(1) << "), returning \"false\"";

      return false;
    }
    else if(!valid_z)
    {
      NQLog("AssemblyRegistryView", NQLog::Spam) << "load_position_4vector"
         << ": invalid format for z-axis position (" << entries.value(2) << "), returning \"false\"";

      return false;
    }
    else if(!valid_a)
    {
      NQLog("AssemblyRegistryView", NQLog::Spam) << "load_position_4vector"
         << ": invalid format for a-axis position (" << entries.value(3) << "), returning \"false\"";

      return false;
    }
    else
    {
      vec.clear();
      vec.reserve(4);

      vec.emplace_back(x_d);
      vec.emplace_back(y_d);
      vec.emplace_back(z_d);
      vec.emplace_back(a_d);

      NQLog("AssemblyRegistryView", NQLog::Spam) << "load_position_4vector"
         << ": 4-vector position read successfully ("
         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")";

      return true;
    }
  }
  else
  {
    NQLog("AssemblyRegistryView", NQLog::Warning) << "load_position_4vector"
       << ": invalid format in QLineEdit (" << line_entry << "), returning \"false\"";

    return false;
  }

  return false;
}
// ==================================================

AssemblyRegistryImageWidget::AssemblyRegistryImageWidget(QWidget* parent) : QWidget(parent)
{
  //// layout
  QGridLayout* g1 = new QGridLayout;
  this->setLayout(g1);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  img_view_ = new AssemblyUEyeView(this);
  img_view_->setMinimumSize(500, 300);
  img_view_->setPalette(palette);
  img_view_->setBackgroundRole(QPalette::Background);
  img_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  img_view_->setScaledContents(true);
  img_view_->setAlignment(Qt::AlignCenter);

  img_scroll_ = new QScrollArea(this);
  img_scroll_->setMinimumSize(500, 300);
  img_scroll_->setPalette(palette);
  img_scroll_->setBackgroundRole(QPalette::Background);
  img_scroll_->setAlignment(Qt::AlignCenter);
  img_scroll_->setWidget(img_view_);
  img_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g1->addWidget(img_scroll_, 0, 0); 

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QFormLayout*  lImg = new QFormLayout;
  g1->addLayout(lImg, 0, 1);

  img_save_button_ = new QPushButton("Save Image", this);
  lImg->addRow(img_save_button_);

  img_celi_button_ = new QPushButton("Draw Center Lines", this);
  lImg->addRow(img_celi_button_);
  //// ---------------

  //// connection(s)
  connect(img_save_button_, SIGNAL(clicked()), this, SLOT(save_image()));
  connect(img_celi_button_, SIGNAL(clicked()), this, SLOT(modify_image_centerlines()));

  this->connectImageProducer(this, SIGNAL(image_updated(cv::Mat)));
  //// ---------------
}

void AssemblyRegistryImageWidget::update_image(const cv::Mat& img)
{
  if(img.channels() == 1)
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    image_ = img_color.clone();
  }
  else
  {
    image_ = img.clone();
  }

  NQLog("AssemblyRegistryImageWidget", NQLog::Spam) << "update_image"
     << ": emitting signal \"image_updated\"";

  emit image_updated(image_);
}

void AssemblyRegistryImageWidget::save_image()
{
    if(image_.rows == 0)
    {
      NQLog("AssemblyRegistryImageWidget", NQLog::Warning) << "save_image"
         << ": input cv::Mat object has zero rows, no image saved";

      return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
    if(filename.isNull() || filename.isEmpty()){ return; }

    if(!filename.endsWith(".png")){ filename += ".png"; }

    cv::imwrite(filename.toStdString(), image_);

    return;
}

void AssemblyRegistryImageWidget::modify_image_centerlines()
{
  if(image_.rows == 0)
  {
    NQLog("AssemblyRegistryImageWidget", NQLog::Warning) << "modify_image_centerlines"
       << ": input cv::Mat object has zero rows, no image saved";

    return;
  }

  cv::Mat img = image_.clone();

  line(img, cv::Point(   img.cols/2.0, 0), cv::Point(img.cols/2.0, img.rows    ), cv::Scalar(255,0,0), 2, 8, 0);
  line(img, cv::Point(0, img.rows/2.0   ), cv::Point(img.cols    , img.rows/2.0), cv::Scalar(255,0,0), 2, 8, 0);

  NQLog("AssemblyRegistryImageWidget", NQLog::Spam) << "modify_image_centerlines"
     << ": updating image";

  this->update_image(img);

  return;
}

void AssemblyRegistryImageWidget::connectImageProducer(const QObject* sender, const char* signal)
{
    NQLog("AssemblyRegistryImageWidget", NQLog::Debug) << "connectImageProducer";

    img_view_->connectImageProducer(sender, signal);
}

void AssemblyRegistryImageWidget::disconnectImageProducer(const QObject* sender, const char* signal)
{
    NQLog("AssemblyRegistryImageWidget", NQLog::Debug) << "disconnectImageProducer";

    img_view_->disconnectImageProducer(sender, signal);
}

void AssemblyRegistryImageWidget::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch(event->key())
    {
      case Qt::Key_0:
//        img_view_->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        img_view_->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        img_view_->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        img_view_->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}
// ==================================================

AssemblyRegistryPositionWidget::AssemblyRegistryPositionWidget(const QString& qstr, const LStepExpressMotionManager* motion_manager, QWidget* parent) :
  QWidget(parent),
  motion_manager_(motion_manager)
{
  //// INITIALIZATION
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyRegistryPositionWidget", NQLog::Critical) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }
  //// --------------

  QGridLayout* l = new QGridLayout;
  this->setLayout(l);

  pos_button_       = new QPushButton("Read"   , this);
  pos_lineed_value_ = new QLineEdit  (""       , this);
  pos_lineed_comme_ = new QLineEdit  ("Comment", this);
  l->addWidget(new QLabel(qstr, this), 0, 0);
  l->addWidget(pos_button_           , 0, 1);
  l->addWidget(pos_lineed_value_     , 0, 2);
  l->addWidget(pos_lineed_comme_     , 0, 3);

  connect(pos_button_, SIGNAL(clicked()), this, SLOT(update_position()));

  NQLog("AssemblyRegistryPositionWidget", NQLog::Debug) << "constructed";
}

void AssemblyRegistryPositionWidget::update_position()
{
  const double x = motion_manager_->get_position_X();
  const double y = motion_manager_->get_position_Y();
  const double z = motion_manager_->get_position_Z();
  const double a = motion_manager_->get_position_A();

  std::stringstream posi_strs;
  posi_strs << x << ", " << y << ", " << z << ", " << a;

  pos_lineed_value_->setText(QString::fromStdString(posi_strs.str()));

  return;
}
// ==================================================
