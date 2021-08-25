/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2SCRIPTWIDGET_H
#define THERMO2SCRIPTWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "Thermo2ScriptModel.h"
#include "Thermo2ScriptSnippets.h"

class Thermo2ScriptEdit : public QPlainTextEdit
{
public:
  Thermo2ScriptEdit(QWidget* parent) : QPlainTextEdit(parent) { }
  virtual QSize sizeHint() const { return QSize(500, 400); }
};

class Thermo2MessageDisplay : public QPlainTextEdit
{
public:
  Thermo2MessageDisplay(QWidget* parent) : QPlainTextEdit(parent) { }
  virtual QSize sizeHint() const { return QSize(500, 100); }
};

class Thermo2ScriptWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit Thermo2ScriptWidget(const QString& title,
                               Thermo2ScriptModel* scriptModel,
                               QWidget *parent = 0);

public slots:
  void clearMessageText();
  void appendMessageText( const QString & text );

protected:

  // Models
  Thermo2ScriptModel* scriptModel_;

  // Widgets
  QPushButton* openScriptButton_;
  QPushButton* saveScriptButton_;
  QPushButton* saveAsScriptButton_;
  QPushButton* executeScriptButton_;
  QPushButton* abortScriptButton_;

  Thermo2ScriptSnippets* scriptSnippets_;
  QPushButton* insertSnippetButton_;

  Thermo2ScriptEdit* scriptEditor_;
  Thermo2MessageDisplay* messageDisplay_;

  QString scriptDirectory_;

protected slots:

  void openScriptButtonClicked();
  void saveScriptButtonClicked();
  void saveAsScriptButtonClicked();
  void executeScriptButtonClicked();
  void abortScriptButtonClicked();

  void insertSnippetButtonClicked();

  void scriptChanged();

  void controlStateChanged(bool);
};

#endif // THERMO2SCRIPTWIDGET_H
