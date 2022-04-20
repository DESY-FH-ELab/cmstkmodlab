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
#include <AssemblyAssemblyActionWidget.h>

#include <QMessageBox>

AssemblyAssemblyActionWidget::AssemblyAssemblyActionWidget(QWidget* parent)
 : QWidget(parent)

 , layout_(nullptr)

 , label_(nullptr)
 , button_(nullptr)
 , checkbox_(nullptr)

 , qobject_(nullptr)
 , start_slot_(nullptr)
 , stop_signal_(nullptr)

 , inhibit_dialogue_(false)
{
  // layout
  layout_ = new QHBoxLayout;
  this->setLayout(layout_);

  label_ = new QLabel;
  label_->setStyleSheet("QLabel { font-weight : bold; }");

  button_ = new QPushButton;
  button_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  checkbox_ = new QCheckBox("Done");

  connect(checkbox_, SIGNAL(stateChanged(int)), this, SLOT(disable(int)));

  layout_->addWidget(label_,  2, Qt::AlignRight);
  layout_->addWidget(button_, 40);
  layout_->addWidget(new QLabel, 48);
  layout_->addWidget(checkbox_, 10);
}

void AssemblyAssemblyActionWidget::disable(const bool to_be_disabled)
{
  this->disable(int(to_be_disabled ? 2 : 0));
}

void AssemblyAssemblyActionWidget::disable(const int state)
{
  if(state == 2)
  {
    if(!inhibit_dialogue_)
    {
      QMessageBox* msgBox = new QMessageBox;
      msgBox->setStyleSheet("QLabel{min-width: 300px;}");
      msgBox->setInformativeText("WARNING: this will not perform any action but just mark the step as \"done\".\nWould you like to proceed?");
      msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
      msgBox->setDefaultButton(QMessageBox::Yes);
      int ret = msgBox->exec();
      switch(ret)
      {
        case QMessageBox::No:
          checkbox_->setCheckState(Qt::Unchecked);
          return;
        case QMessageBox::Yes:
          break;
        default: return;
      }
    } else {
      inhibit_dialogue_ = false;
    }
    label_->setEnabled(false);
    button_->setEnabled(false);
  }
  else if(state == 0)
  {
    label_->setEnabled(true);
    button_->setEnabled(true);
  }

  return;
}

void AssemblyAssemblyActionWidget::connect_action(const QObject* qobject, const char* start_slot, const char* stop_signal)
{
  if(qobject_){ this->reset_action(); }

  if(qobject)
  {
    qobject_ = qobject;
    start_slot_ = start_slot;
    stop_signal_ = stop_signal;

    connect(this->button(), SIGNAL(clicked()), this, SLOT(start_action()));
  }
  else
  {
    NQLog("AssemblyAssemblyActionWidget", NQLog::Warning) << "connect_action"
       << ": invalid (NULL) input pointer to QObject, no action taken";
  }
}

void AssemblyAssemblyActionWidget::reset_action()
{
  qobject_ = nullptr;
  start_slot_ = nullptr;
  stop_signal_ = nullptr;
}

void AssemblyAssemblyActionWidget::start_action()
{
  if(qobject_)
  {
    connect(this, SIGNAL(action_request()), qobject_, start_slot_);

    connect(qobject_, stop_signal_, this, SLOT(disable_action()));

    emit action_request();
  }
  else
  {
    NQLog("AssemblyAssemblyActionWidget", NQLog::Warning) << "start_action"
       << ": invalid (NULL) pointer to QObject, no action taken";
  }
}

void AssemblyAssemblyActionWidget::disable_action()
{
  if(qobject_)
  {
    disconnect(this, SIGNAL(action_request()), qobject_, start_slot_);

    disconnect(qobject_, stop_signal_, this, SLOT(disable_action()));

    inhibit_dialogue_ = true;
    checkbox_->setCheckState(Qt::Checked);
  }
  else
  {
    NQLog("AssemblyAssemblyActionWidget", NQLog::Warning) << "disable_action"
       << ": invalid (NULL) pointer to QObject, no action taken";
  }
}
