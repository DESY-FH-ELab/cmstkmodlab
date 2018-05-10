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

#include <AssemblyVacuumWidget.h>

#include <QPixmap>
#include <QPainter>

AssemblyVacuumWidget::AssemblyVacuumWidget(const QString& label, QWidget* parent) : QWidget(parent)
{
  layout_ = new QVBoxLayout(this);
  this->setLayout(layout_);

  button_ = new QPushButton(label, this);
  layout_->addWidget(button_);

  QGridLayout* grid = new QGridLayout;

  QRadioButton* radio_1 = new QRadioButton(tr("&Pickup")   , this);
  QRadioButton* radio_2 = new QRadioButton(tr("&Spacers")  , this);
  QRadioButton* radio_3 = new QRadioButton(tr("&Baseplate"), this);
//  QRadioButton* radio_4 = new QRadioButton(tr("&Channel 4"), this);

  grid->addWidget(radio_1, 1, 0);
  grid->addWidget(radio_2, 3, 0);
  grid->addWidget(radio_3, 5, 0);
//  grid->addWidget(radio_4, 7, 0);

  QPixmap pixmap(100, 100);
  pixmap.fill(QColor("transparent"));

  QPainter painter(&pixmap);
  painter.setBrush(QBrush(Qt::red));
  painter.drawEllipse(0, 0, 30, 30);

  QLabel* label_1 = new QLabel("", this);
  label_1->setPixmap(pixmap);
  label_1->setText(" VACUUM OFF");
  label_1->setStyleSheet("QLabel { background-color : green; color : black; }");

  QLabel* label_2 = new QLabel("", this);
  label_2->setPixmap(pixmap);
  label_2->setText(" VACUUM OFF");
  label_2->setStyleSheet("QLabel { background-color : green; color : black; }");

  QLabel* label_3 = new QLabel("", this);
  label_3->setPixmap(pixmap);
  label_3->setText(" VACUUM OFF");
  label_3->setStyleSheet("QLabel { background-color : green; color : black; }");

//  QLabel* label_4 = new QLabel("", this);
//  label_4->setPixmap(pixmap);
//  label_4->setText(" VACUUM OFF");
//  label_4->setStyleSheet("QLabel { background-color : green; color : black; }");

  grid->addWidget(label_1, 1, 1);
  grid->addWidget(label_2, 3, 1);
  grid->addWidget(label_3, 5, 1);
//  grid->addWidget(label_4, 7, 1);

  layout_->addLayout(grid);

  valves_.clear();
  valves_.push_back(radio_1);
  valves_.push_back(radio_2);
  valves_.push_back(radio_3);
//  valves_.push_back(radio4);

  labels_.clear();
  labels_.push_back(label_1);
  labels_.push_back(label_2);
  labels_.push_back(label_3);
//  labels_.push_back(label_4);

  connect(button_, SIGNAL(clicked()), this, SLOT(toggleVacuum()));

  NQLog("AssemblyVacuumWidget", NQLog::Debug) << "constructed";
}

void AssemblyVacuumWidget::toggleVacuum()
{
  NQLog("AssemblyVacuumWidget") << ": toggling vacuum voltage";

  for(unsigned int i=0; i<valves_.size(); ++i)
  {
    if(valves_.at(i)->isChecked())
    {
      NQLog("AssemblyVacuumWidget") << ": emit signal to channel " << (i+1);

      button_->setEnabled(false);

      emit toggleVacuum(i+1);

      return;
    }
  }

  NQLog("AssemblyVacuumWidget", NQLog::Warning) << "toggleVacuum"
    << ": no channel selected, vacuum not toggled";

  return;
}

void AssemblyVacuumWidget::enableVacuumButton()
{
  button_->setEnabled(true);
}

void AssemblyVacuumWidget::disableVacuumButton()
{
  button_->setEnabled(false);
}

void AssemblyVacuumWidget::updateVacuumChannelState(const int channelNumber, const bool channelState)
{
  if(channelNumber >= int(labels_.size()))
  {
    NQLog("AssemblyVacuumWidget", NQLog::Warning) << "updateVacuumChannelState"
       << "(" << channelNumber << ", " << channelState << ")"
       << ": out-of-range input channel number (" << channelNumber << "), no action taken";

    return;
  }

  emit enableVacuumButton();

  if(channelState)
  {
    labels_.at(channelNumber)->setText(" VACUUM ON");
    labels_.at(channelNumber)->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else
  {
    labels_.at(channelNumber)->setText(" VACUUM OFF");
    labels_.at(channelNumber)->setStyleSheet("QLabel { background-color : green; color : black; }");
  }
}
