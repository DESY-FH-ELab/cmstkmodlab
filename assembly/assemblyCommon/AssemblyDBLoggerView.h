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

#ifndef DBLOGGERVIEW_H
#define DBLOGGERVIEW_H

#include <iostream>

#include "nqlogger.h"
#include "ApplicationConfig.h"

#include <QWidget>
#include <QString>
#include <QObject>
#include <QFile>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFrame>

class AssemblyDBLoggerViewer;

class AssemblyDBLoggerView : public QWidget
{
    Q_OBJECT //Needed to be able to connect slots, etc.

    public:

        AssemblyDBLoggerView(const QString&); //, QWidget* parent=nullptr
        virtual ~AssemblyDBLoggerView();

        QPushButton* get_DBLogger_button_edit() const {return button_edit_;}
        QPushButton* get_DBLogger_button_load() const {return button_load_;}
        QPushButton* get_DBLogger_button_save() const {return button_save_;}
        QPushButton* get_DBLogger_button_send() const {return button_send_;}
        QComboBox* get_DBLogger_appendModeBox() const {return locComboBox_;}

    protected:

        AssemblyDBLoggerViewer* viewer_;
        const QString outputdir_path_;

        QVBoxLayout* mainLayout_;
        QGroupBox* actionsGroup_;
        QBoxLayout* horizLayout_;
        QGroupBox* editGroup_;
        QLabel* locLabel_;
        QToolBox* toolbox_;
        QGridLayout* editLayout_;
        QPushButton* button_edit_;
        QPushButton* button_load_;
        QPushButton* button_save_;
        QPushButton* button_send_;
        QPlainTextEdit* messageTextEdit;
        QComboBox* locComboBox_;
        QFrame* line_;

        void connect_view();
        void eraseComment();

    public slots:

        void messageBoxWriteMessage();
        void update_content(const QStringList&);
        void load_logfile();
        void save_logfile();
        void clear_content();
        void display_infoTab();

    signals:

        void DBLogMessage(const QString);
        void validPath_toExistingLog(const QString);
        void validPath_toCopy(const QString);
};

//--------------------------------------------
//--------------------------------------------

//Helper class -- custom instance of QPlainTextEdit
class AssemblyDBLoggerViewer : public QPlainTextEdit
{
 Q_OBJECT

 public:

  explicit AssemblyDBLoggerViewer();
  virtual ~AssemblyDBLoggerViewer();
};

#endif // DBLOGGERVIEW_H
