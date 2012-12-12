#include <QGroupBox>
#include <QVBoxLayout>

#include "DefoScriptWidget.h"

DefoScriptWidget::DefoScriptWidget(
    DefoScriptModel* scriptModel
  , QWidget *parent
) :
    QWidget(parent)
  , scriptModel_(scriptModel)
{
  // Layouts to put everything into place
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  // Camera buttons
  QGridLayout *buttonLayout = new QGridLayout();
  buttons_ = new QWidget(this);
  buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  buttons_->setLayout(buttonLayout);
  layout->addWidget(buttons_);

  // load a script
  openScriptButton_ = new QPushButton("&Open script", buttons_);
  connect(openScriptButton_, SIGNAL(clicked()), this, SLOT(openScriptButtonClicked()));
  buttonLayout->addWidget(openScriptButton_, 0, 0);

  // save a script
  saveScriptButton_ = new QPushButton("&Save script", buttons_);
  connect(saveScriptButton_, SIGNAL(clicked()), this, SLOT(saveScriptButtonClicked()));
  buttonLayout->addWidget(saveScriptButton_, 0, 1);

  // save a script with new name
  saveAsScriptButton_ = new QPushButton("&Save script as", buttons_);
  connect(saveAsScriptButton_, SIGNAL(clicked()), this, SLOT(saveAsScriptButtonClicked()));
  buttonLayout->addWidget(saveAsScriptButton_, 0, 2);

  // save a script
  executeScriptButton_ = new QPushButton("&Execute script", buttons_);
  connect(executeScriptButton_, SIGNAL(clicked()), this, SLOT(executeScriptButtonClicked()));
  buttonLayout->addWidget(executeScriptButton_, 0, 3);

  // execute script
  abortScriptButton_ = new QPushButton("&Abort script", buttons_);
  connect(abortScriptButton_, SIGNAL(clicked()), this, SLOT(abortScriptButtonClicked()));
  buttonLayout->addWidget(abortScriptButton_, 0, 4);

  // script editor
  scriptEditor_ = new DefoScriptEdit(this);
  scriptEditor_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  scriptEditor_->resize(600, 400);
  scriptEditor_->setDocument(scriptModel_->scriptDocument());
  scriptEditor_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scriptEditor_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  layout->addWidget(scriptEditor_, 1);

  connect(scriptModel_->scriptDocument(), SIGNAL(contentsChanged()),
          this, SLOT(scriptChanged()));

  // message display
  messageDisplay_ = new DefoMessageDisplay(this);
  messageDisplay_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  messageDisplay_->resize(600, 100);
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

void DefoScriptWidget::openScriptButtonClicked() {

  QString filename = QFileDialog::getOpenFileName(this
						, "open script"
                                                , "./"
						, "Defo Scripts (*.dsr)"
						, 0
						, 0);
  if (!filename.isNull()) scriptModel_->openScript(filename);
}

void DefoScriptWidget::saveScriptButtonClicked() {

  QString filename = scriptModel_->currentScriptFilename();
  if (filename.isNull()) 
    this->saveAsScriptButtonClicked();
  else
    scriptModel_->saveScript(filename);
}

void DefoScriptWidget::saveAsScriptButtonClicked() {

  QString filename = QFileDialog::getSaveFileName(this
						, "save script"
                                                , "./"
						, "Defo Scripts (*.dsr)"
						, 0
						, 0);
  if (filename.isNull()) return;
  if (!filename.endsWith("dsr")) filename += ".dsr";
  scriptModel_->saveScript(filename);
}

void DefoScriptWidget::executeScriptButtonClicked() {

  scriptModel_->executeScript();
}

void DefoScriptWidget::abortScriptButtonClicked() {

  scriptModel_->abortScript();
}

void DefoScriptWidget::scriptChanged() {

}

void DefoScriptWidget::clearMessageText( ) {
  
  messageDisplay_->setPlainText("");
}

void DefoScriptWidget::appendMessageText( const QString & text ) {
  
  messageDisplay_->appendPlainText(text);
}
