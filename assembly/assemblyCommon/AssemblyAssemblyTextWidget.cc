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
#include <AssemblyAssemblyTextWidget.h>

AssemblyAssemblyTextWidget::AssemblyAssemblyTextWidget(QWidget* parent)
 : QWidget(parent)
 , layout_(nullptr)
 , label_(nullptr)
 , text_(nullptr)
 , checkbox_(nullptr)
{
  // layout
  layout_ = new QHBoxLayout;
  this->setLayout(layout_);

  label_ = new QLabel;
  label_->setStyleSheet("QLabel { font-weight : bold; }");

  text_ = new QLabel;
  text_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  checkbox_ = new QCheckBox("Done");

  connect(checkbox_, SIGNAL(stateChanged(int)), this, SLOT(disable(int)));

  layout_->addWidget(label_, 2, Qt::AlignRight);
  layout_->addWidget(text_, 40);
  layout_->addWidget(new QLabel, 48);
  layout_->addWidget(checkbox_, 10);
  // --------------
}

void AssemblyAssemblyTextWidget::disable(const bool to_be_disabled)
{
  this->disable(int(to_be_disabled ? 2 : 0));
}

void AssemblyAssemblyTextWidget::disable(const int state)
{
  if(state == 2)
  {
    label_->setEnabled(false);
    text_->setEnabled(false);
  }
  else if(state == 0)
  {
    label_->setEnabled(true);
    text_->setEnabled(true);
  }

  return;
}

void AssemblyAssemblyTextWidget::reset_action()
{
  qobject_ = nullptr;
  check_slot_ = nullptr;
  uncheck_slot_ = nullptr;
}


void AssemblyAssemblyTextWidget::connect_check_action(const QObject* qobject, const char* check_slot, const char* uncheck_slot)
{
  if(qobject_) {this->reset_action();}

  if(qobject)
  {
    qobject_ = qobject;
    check_slot_ = check_slot;
    uncheck_slot_ = uncheck_slot;

    connect(checkbox_, SIGNAL(stateChanged(int)), this, SLOT(change_action(int)));
  }
  else
  {
    NQLog("AssemblyAssemblyTextWidget", NQLog::Warning) << "connect_check_action"
       << ": invalid (NULL) input pointer to QObject, no action taken";
  }
}

void AssemblyAssemblyTextWidget::change_action(int state)
{
  NQLog("AssemblyAssemblyTextWidget", NQLog::Spam) << "calling check_action";

  if(qobject_)
  {
    if(state == Qt::Checked)
    {
      if(check_slot_)
      {
        connect(this, SIGNAL(action_request()), qobject_, check_slot_);
        emit action_request();
        disconnect(this, SIGNAL(action_request()), qobject_, check_slot_);
      }
    } else if(state == Qt::Unchecked)
    {
      if(uncheck_slot_)
      {
        connect(this, SIGNAL(action_request()), qobject_, uncheck_slot_);
        emit action_request();
        disconnect(this, SIGNAL(action_request()), qobject_, uncheck_slot_);
      }
    }
  }
  else
  {
    NQLog("AssemblyAssemblyTextWidget", NQLog::Warning) << "check_action"
       << ": invalid (NULL) pointer to QObject, no action taken";
  }
}
