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

#include <AssemblyVacuumWidget.h>
#include <AssemblyUtilities.h>

#include <QGridLayout>

AssemblyVacuumWidget::AssemblyVacuumWidget(const QString& label, QWidget* parent) : QWidget(parent)
{
  layout_ = new QVBoxLayout(this);
  this->setLayout(layout_);

  button_ = new QPushButton(label, this);
  layout_->addWidget(button_);

  QGridLayout* grid = new QGridLayout;

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyVacuumWidget", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[AssemblyVacuumWidget]"), tr("ApplicationConfig::instance() not initialized\n."));
  }

  /// PICKUP TOOL
  const int vacuum_pickup = config->getValue<int>("main", "Vacuum_PickupTool");

  valuemap_[vacuum_pickup] = Entry();

  this->get(vacuum_pickup).radioButton_ = new QRadioButton(tr("Pickup"));
  this->get(vacuum_pickup).label_ = new QLabel(tr(" UNKNOWN"));
  this->get(vacuum_pickup).label_->setStyleSheet("QLabel { background-color : grey; color : black; }");

  grid->addWidget(this->get(vacuum_pickup).radioButton_, 1, 0);
  grid->addWidget(this->get(vacuum_pickup).label_      , 1, 1);
  /// ------------------------------

  /// SPACERS
  const int vacuum_spacer = config->getValue<int>("main", "Vacuum_Spacers");

  valuemap_[vacuum_spacer] = Entry();

  this->get(vacuum_spacer).radioButton_ = new QRadioButton(tr("Spacers"));
  this->get(vacuum_spacer).label_ = new QLabel(tr(" UNKNOWN"));
  this->get(vacuum_spacer).label_->setStyleSheet("QLabel { background-color : gray; color : black; }");

  grid->addWidget(this->get(vacuum_spacer).radioButton_, 3, 0);
  grid->addWidget(this->get(vacuum_spacer).label_      , 3, 1);
  /// ------------------------------

  /// BASEPLATE
  const int vacuum_basepl = config->getValue<int>("main", "Vacuum_Baseplate");

  valuemap_[vacuum_basepl] = Entry();

  this->get(vacuum_basepl).radioButton_ = new QRadioButton(tr("Baseplate"));
  this->get(vacuum_basepl).label_ = new QLabel(tr(" UNKNOWN"));
  this->get(vacuum_basepl).label_->setStyleSheet("QLabel { background-color : gray; color : black; }");

  grid->addWidget(this->get(vacuum_basepl).radioButton_, 5, 0);
  grid->addWidget(this->get(vacuum_basepl).label_      , 5, 1);
  /// ------------------------------

  /// SUBASSEMBLY
  const int vacuum_sub = config->getValue<int>("main", "Vacuum_Subassembly");

  valuemap_[vacuum_sub] = Entry();

  this->get(vacuum_sub).radioButton_ = new QRadioButton(tr("Subassembly"));
  this->get(vacuum_sub).label_ = new QLabel(tr(" UNKNOWN"));
  this->get(vacuum_sub).label_->setStyleSheet("QLabel { background-color : gray; color : black; }");

  grid->addWidget(this->get(vacuum_sub).radioButton_, 7, 0);
  grid->addWidget(this->get(vacuum_sub).label_      , 7, 1);
  /// ------------------------------

  layout_->addLayout(grid);

  connect(button_, SIGNAL(clicked()), this, SLOT(toggleVacuum()));

  NQLog("AssemblyVacuumWidget", NQLog::Debug) << "constructed";
}

bool AssemblyVacuumWidget::has(const int key) const
{
  return bool(valuemap_.find(key) != valuemap_.end());
}

AssemblyVacuumWidget::Entry& AssemblyVacuumWidget::get(const int key)
{
  if(this->has(key) == false)
  {
    NQLog("AssemblyVacuumWidget", NQLog::Fatal) << "get"
       << ": no entry associated to index \"" << key << "\", closing application";

    assembly::kill_application(tr("[AssemblyVacuumWidget::get]"), tr("Failed to find entry associated to index \"%1\"\n.").arg(QString::number(key)));
  }

  return valuemap_.at(key);
}

void AssemblyVacuumWidget::toggleVacuum()
{
  for(const auto& i_vacuum : valuemap_)
  {
    QRadioButton* const radioButton = i_vacuum.second.radioButton_;

    if(radioButton != nullptr)
    {
      if(radioButton->isChecked() == true)
      {
        NQLog("AssemblyVacuumWidget") << "toggleVacuum"
          << ": emitting signal toggleVacuum(" << i_vacuum.first << ")";

        emit toggleVacuum(i_vacuum.first);

        return;
      }
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

void AssemblyVacuumWidget::updateVacuumChannelState(const int channelNumber, const SwitchState channelState)
{
  NQLog("AssemblyVacuumWidget", NQLog::Debug) << "updateVacuumChannelState"
  << "(" << channelNumber << ", " << static_cast<int>(channelState) << ")"
  << ": vacuum line with index " << channelNumber << " found. Updating status to " << static_cast<int>(channelState);

  if(this->has(channelNumber) == false)
  {
    NQLog("AssemblyVacuumWidget", NQLog::Warning) << "updateVacuumChannelState"
       << "(" << channelNumber << ", " << static_cast<int>(channelState) << ")"
       << ": vacuum line with index " << channelNumber << " not found, no action taken";

    return;
  }

  if(channelState==SwitchState::CHANNEL_ON)
  {
    this->get(channelNumber).label_->setText(" VACUUM ON");
    this->get(channelNumber).label_->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else if(channelState==SwitchState::CHANNEL_SWITCHING)
  {
    this->get(channelNumber).label_->setText(" VACUUM SWITCHING");
    this->get(channelNumber).label_->setStyleSheet("QLabel { background-color : orange; color : black; }");
  }
  else if(channelState==SwitchState::CHANNEL_OFF)
  {
    this->get(channelNumber).label_->setText(" VACUUM OFF");
    this->get(channelNumber).label_->setStyleSheet("QLabel { background-color : green; color : black; }");
  }
  else if(channelState==SwitchState::DEVICE_OFF)
  {
    this->get(channelNumber).label_->setText(" DEVICE OFF");
    this->get(channelNumber).label_->setStyleSheet("QLabel { background-color : grey; color : black; }");
  }

  return;
}

void AssemblyVacuumWidget::updateVacuumChannelsStatus()
{
  for(const auto& i_vacuum : valuemap_)
  {
    const int ch_number = i_vacuum.first;

    emit vacuumChannelState_request(ch_number);
  }
}
