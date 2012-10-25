#ifndef DEFOSCRIPTWIDGET_H
#define DEFOSCRIPTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "DefoScriptModel.h"

class DefoScriptEdit : public QPlainTextEdit {
public:
    DefoScriptEdit(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 400); }
};

class DefoMessageDisplay : public QPlainTextEdit {
public:
    DefoMessageDisplay(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 100); }
};

class DefoScriptWidget : 
      public QWidget
{
  Q_OBJECT
public:
  explicit DefoScriptWidget(
      DefoScriptModel* scriptModel
    , QWidget *parent = 0
  );

public slots:
  void clearMessageText();
  void appendMessageText( const QString & text );

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
  DefoScriptEdit* scriptEditor_;
  DefoMessageDisplay* messageDisplay_;

protected slots:
  void openScriptButtonClicked();
  void saveScriptButtonClicked();
  void saveAsScriptButtonClicked();
  void executeScriptButtonClicked();
  void abortScriptButtonClicked();

  void scriptChanged();
};

#endif // DEFOCAMERAWIDGET_H
