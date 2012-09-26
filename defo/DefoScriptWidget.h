#ifndef DEFOSCRIPTWIDGET_H
#define DEFOSCRIPTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextEdit>

#include "DefoScriptModel.h"

class DefoScriptWidget : 
      public QWidget
{
  Q_OBJECT
public:
  explicit DefoScriptWidget(
      DefoScriptModel* scriptModel
    , QWidget *parent = 0
  );

protected:
  // Models
  DefoScriptModel* scriptModel_;

  // Widgets
  QPushButton* openScriptButton_;
  QPushButton* saveScriptButton_;
  QPushButton* saveAsScriptButton_;
  QPushButton* executeScriptButton_;
  QPushButton* abortScriptButton_;
  QWidget* buttons_;
  QTextEdit* scriptEditor_;

protected slots:
  void openScriptButtonClicked();
  void saveScriptButtonClicked();
  void saveAsScriptButtonClicked();
  void executeScriptButtonClicked();
  void abortScriptButtonClicked();

};

#endif // DEFOCAMERAWIDGET_H
