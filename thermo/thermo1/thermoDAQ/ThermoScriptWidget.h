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

#ifndef THERMOSCRIPTWIDGET_H
#define THERMOSCRIPTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "ThermoScriptModel.h"

class ThermoScriptEdit : public QPlainTextEdit {
public:
    ThermoScriptEdit(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 400); }
};

class ThermoMessageDisplay : public QPlainTextEdit {
public:
    ThermoMessageDisplay(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 100); }
};

class ThermoScriptWidget :
      public QWidget
{
  Q_OBJECT
public:
  explicit ThermoScriptWidget(ThermoScriptModel* scriptModel,
                              QWidget *parent = 0);

public slots:
  void clearMessageText();
  void appendMessageText( const QString & text );

protected:
  // Models
  ThermoScriptModel* scriptModel_;

  // Widgets
  QPushButton* openScriptButton_;
  QPushButton* saveScriptButton_;
  QPushButton* saveAsScriptButton_;
  QPushButton* executeScriptButton_;
  QPushButton* abortScriptButton_;
  QWidget* buttons_;
  ThermoScriptEdit* scriptEditor_;
  ThermoMessageDisplay* messageDisplay_;

protected slots:
  void openScriptButtonClicked();
  void saveScriptButtonClicked();
  void saveAsScriptButtonClicked();
  void executeScriptButtonClicked();
  void abortScriptButtonClicked();

  void scriptChanged();
};

#endif // THERMOSCRIPTWIDGET_H
