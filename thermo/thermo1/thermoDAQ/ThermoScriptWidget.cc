/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QGroupBox>
#include <QVBoxLayout>

#include "ThermoScriptWidget.h"

ThermoScriptWidget::ThermoScriptWidget(ThermoScriptModel* scriptModel,
                                       QWidget *parent) :
    QWidget(parent),
    scriptModel_(scriptModel)
{
  // Layouts to put everything into place
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);
  //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  // Camera buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttons_ = new QWidget(this);
  //buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  buttons_->setLayout(buttonLayout);
  layout->addWidget(buttons_);

  // load a script
  openScriptButton_ = new QPushButton("&Open script", buttons_);
  connect(openScriptButton_, SIGNAL(clicked()), this, SLOT(openScriptButtonClicked()));
  buttonLayout->addWidget(openScriptButton_);

  // save a script
  saveScriptButton_ = new QPushButton("&Save script", buttons_);
  connect(saveScriptButton_, SIGNAL(clicked()), this, SLOT(saveScriptButtonClicked()));
  buttonLayout->addWidget(saveScriptButton_);

  // save a script with new name
  saveAsScriptButton_ = new QPushButton("&Save script as", buttons_);
  connect(saveAsScriptButton_, SIGNAL(clicked()), this, SLOT(saveAsScriptButtonClicked()));
  buttonLayout->addWidget(saveAsScriptButton_);

  buttonLayout->addSpacing(100);

  // save a script
  executeScriptButton_ = new QPushButton("&Execute script", buttons_);
  connect(executeScriptButton_, SIGNAL(clicked()), this, SLOT(executeScriptButtonClicked()));
  buttonLayout->addWidget(executeScriptButton_);

  // execute script
  abortScriptButton_ = new QPushButton("&Abort script", buttons_);
  connect(abortScriptButton_, SIGNAL(clicked()), this, SLOT(abortScriptButtonClicked()));
  buttonLayout->addWidget(abortScriptButton_);

  // script editor
  scriptEditor_ = new ThermoScriptEdit(this);
  //scriptEditor_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  scriptEditor_->setMinimumWidth(600);
  scriptEditor_->setMinimumHeight(150);
  scriptEditor_->setDocument(scriptModel_->scriptDocument());
  scriptEditor_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scriptEditor_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  layout->addWidget(scriptEditor_, 1);

  connect(scriptModel_->scriptDocument(), SIGNAL(contentsChanged()),
          this, SLOT(scriptChanged()));

  // message display
  messageDisplay_ = new ThermoMessageDisplay(this);
  messageDisplay_->setMinimumWidth(600);
  messageDisplay_->setMaximumHeight(100);
  messageDisplay_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  messageDisplay_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  messageDisplay_->setReadOnly(true);
  layout->addWidget(messageDisplay_, 2);

  updateGeometry();

  connect(scriptModel_, SIGNAL(clearMessageText()),
          this, SLOT(clearMessageText()));
  connect(scriptModel_, SIGNAL(appendMessageText(const QString &)),
          this, SLOT(appendMessageText(const QString &)));
}

void ThermoScriptWidget::openScriptButtonClicked() {

  QString filename = QFileDialog::getOpenFileName(this,
                                                  "open script",
                                                  "./",
                                                  "Thermo Scripts (*.tsr)",
                                                  0,
                                                  0);
  if (!filename.isNull()) scriptModel_->openScript(filename);
}

void ThermoScriptWidget::saveScriptButtonClicked() {

  QString filename = scriptModel_->currentScriptFilename();
  if (filename.isNull()) 
    this->saveAsScriptButtonClicked();
  else
    scriptModel_->saveScript(filename);
}

void ThermoScriptWidget::saveAsScriptButtonClicked() {

  QString filename = QFileDialog::getSaveFileName(this,
                                                  "save script",
                                                  "./",
                                                  "Thermo Scripts (*.tsr)",
                                                  0,
                                                  0);
  if (filename.isNull()) return;
  if (!filename.endsWith(".tsr")) filename += ".tsr";
  scriptModel_->saveScript(filename);
}

void ThermoScriptWidget::executeScriptButtonClicked() {

  scriptModel_->executeScript();
}

void ThermoScriptWidget::abortScriptButtonClicked() {

  scriptModel_->abortScript();
}

void ThermoScriptWidget::scriptChanged() {

}

void ThermoScriptWidget::clearMessageText( ) {
  
  messageDisplay_->setPlainText("");
}

void ThermoScriptWidget::appendMessageText( const QString & text ) {
  
  messageDisplay_->appendPlainText(text);
}
