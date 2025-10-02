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

#include <AssemblyDBLoggerView.h>

AssemblyDBLoggerView::AssemblyDBLoggerView(const QString& outputdir_path) :
outputdir_path_(outputdir_path)
{
//--------------------------------------------
//Main layout of the "DB Log" tab

    mainLayout_ = new QVBoxLayout;
    this->setLayout(mainLayout_);

//--------------------------------------------
//Horizontal group of boxes (push buttons)

    actionsGroup_ = new QGroupBox(tr("Actions")); //Pointer to group of boxes //The QGroupBox widget provides a group box frame with a title.
    horizLayout_ = new QBoxLayout(QBoxLayout::LeftToRight); //Layout of QGroupBox object //QFormLayout is a convenience layout class that lays out its children in a two-column form. The left column consists of labels and the right column consists of "field" widgets (line editors, spin boxes, etc.).

    //Push buttons part of the QGroupBox
    button_load_ = new QPushButton(tr("Load existing log"));
    button_load_->setMinimumWidth(200); button_load_->setMaximumWidth(500);
    horizLayout_->addWidget(button_load_);
    button_save_ = new QPushButton(tr("Save log to..."));
    button_save_->setMinimumWidth(200); button_save_->setMaximumWidth(500);
    horizLayout_->addWidget(button_save_);
    button_send_ = new QPushButton(tr("Send to Database"));
    button_send_->setStyleSheet("QPushButton { background-color: rgb(0, 204, 51); font: 18px; padding: 1px; } QPushButton:hover { background-color: rgb(0, 255, 0); font: bold 18px; padding: 2px; }");
    button_send_->setMinimumWidth(200); button_send_->setMaximumWidth(500);
    button_send_->setEnabled(false); //not activated for now
    horizLayout_->addWidget(button_send_);

    // line_ = new QFrame; //Vertical separator
    // line_->setObjectName(QString::fromUtf8("line"));
    // line_->setFrameShape(QFrame::VLine);
    // line_->setFrameShadow(QFrame::Sunken);
    // horizLayout_->addWidget(line_);

    actionsGroup_->setLayout(horizLayout_); //Apply layout to QGroupBox object
    mainLayout_->addWidget(actionsGroup_); //Add QGroupBox object to tab's main layout

//--------------------------------------------
//Boxes and writable text zone

    editGroup_ = new QGroupBox(tr("Edit log"));
    editLayout_ = new QGridLayout;
    // QBoxLayout* editLayout_ = new QBoxLayout(Qt::Horizontal); //The QBoxLayout class lines up child widgets horizontally or vertically

    locComboBox_ = new QComboBox; //Multiple choice box
    locLabel_ = new QLabel(tr("Append the comment at:"));
    locComboBox_->setStyleSheet("QComboBox { background: white; } QComboBox QAbstractItemView {border: 1px solid grey; background: white; selection-background-color: blue; } ");
    locComboBox_->addItem(tr("End of log file"));
    locComboBox_->addItem(tr("Start of log file"));

    messageTextEdit = new QPlainTextEdit; //Text zone
    messageTextEdit->setFocus();

    button_edit_ = new QPushButton(tr("Write message to log"));

    //Organize widgets within layout
    editLayout_->addWidget(locLabel_, 0, 0);
    editLayout_->addWidget(locComboBox_, 0, 1);
    editLayout_->addWidget(messageTextEdit, 1, 0, 3, 3); //(fromRow, fromColumn, rowSpan, columnSpan)
    editLayout_->addWidget(button_edit_, 5, 1);

    editGroup_->setLayout(editLayout_); //Apply layout to QGroupBox object
    mainLayout_->addWidget(editGroup_); //Add QGroupBox object to tab's main layout

//--------------------------------------------
//Display logfile content

    toolbox_ = new QToolBox;
    mainLayout_->addWidget(toolbox_);

    viewer_ = new AssemblyDBLoggerViewer();
    toolbox_->addItem(viewer_, tr("Logfile content"));

//--------------------------------------------

    connect_view(); //Connects within View class
}

AssemblyDBLoggerView::~AssemblyDBLoggerView()
{
}

//Connect signal/slots within this class
void AssemblyDBLoggerView::connect_view()
{
    connect(this->get_DBLogger_button_edit(), SIGNAL(clicked()), this, SLOT(messageBoxWriteMessage()) );
    connect(this->get_DBLogger_button_load(), SIGNAL(clicked()), this, SLOT(load_logfile()));
    connect(this->get_DBLogger_button_save(), SIGNAL(clicked()), this, SLOT(save_logfile()));

    return;
}

//Erase content of writable text zone
void AssemblyDBLoggerView::eraseComment()
{
    messageTextEdit->setPlainText("");

    return;
}

//Prompt message box to user (write/cancel written message)
void AssemblyDBLoggerView::messageBoxWriteMessage()
{
    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("Do you want to append this message to the DB log file?");
    msgBox->setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::Yes);
    int ret = msgBox->exec();

    if(ret == QMessageBox::Cancel) {eraseComment();} //Cancel  -> erase
    else if(ret == QMessageBox::Yes) //Yes -> write message
    {
        const QString comment = messageTextEdit->toPlainText();

        NQLog("AssemblyDBLoggerView", NQLog::Debug) << "messageBoxWriteMessage" << ": emitting signal \"DBLogMessage(QStringList)\"";
        emit DBLogMessage(comment);
    }
    //else -> do nothing

    delete msgBox; msgBox = NULL;

    return;
}

//Update the logfile content displayed in the GUI tab, with the content given in arg
void AssemblyDBLoggerView::update_content(const QStringList& qstr_list)
{
    for(const auto& i_qstr : qstr_list) {viewer_->appendPlainText(i_qstr);}

    return;
}

//Prompt dialog box for user to select existing logfile to be loaded
void AssemblyDBLoggerView::load_logfile()
{
    const QString filename = QFileDialog::getOpenFileName(this, tr("Load logfile"), outputdir_path_, tr("LOG Files (*.log);;All Files (*)"));
    if(filename.isNull() || filename.isEmpty()) {return;}

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    file.close();

    NQLog("AssemblyDBLoggerView", NQLog::Debug) << "load_logfile" << ": emitting signal \"found_existingLog(QStringList)\"";
    emit validPath_toExistingLog(filename);

    clear_content();
}

//Prompt dialog box for user to select path where to save current logfile
void AssemblyDBLoggerView::save_logfile()
{
    // ApplicationConfig* config = ApplicationConfig::instance();

    const QString filename = QFileDialog::getSaveFileName(this, tr("Save Logfile"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("Log Files (*.log);;All Files (*)"));
    if(filename.isNull() || filename.isEmpty()) {return;}

    NQLog("AssemblyDBLoggerView", NQLog::Debug) << "save_logfile" << ": emitting signal \"validPath_toCopy(QStringList)\"";
    emit validPath_toCopy(filename);

    return;
}

//Clear the content displayed in the GUI tab (so that it can be replaced)
void AssemblyDBLoggerView::clear_content()
{
    viewer_->setPlainText("");

    return;
}

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyDBLoggerView::display_infoTab()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Information - Database log"));

    messageBox.setText(tr("<p>This tab allows to browse the content of the database (DB) log, and provides interactive editing functionalities.</p>"
    "<p>The DB log is automatically generated whenever the program is launched <i>(NB: the path to the logfile is printed in the terminal at startup; it is stored in the cache assembly-specific directory)</i>."
    "<br>This log is filled with relevant information related to the assembly procedure, such as: timestamps of the different steps, quality critera for PatRec, etc." 
    "<br>In the future, this file will be uploaded into the central upgrade database.</p>"
    "<p>The 'Edit Log' functionality allows to interactively prepend or append additional messages to the DB log.</p>"
    ));

    QSpacerItem* horizontalSpacer = new QSpacerItem(3000, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); //Use this to enlarge box width
    QGridLayout* layout = (QGridLayout*) messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    messageBox.exec();

    return;
}

// ===============================================================================

//Custom QPlainTextEdit class, for display of logfile content within GUI tab
AssemblyDBLoggerViewer::AssemblyDBLoggerViewer()
{
  this->setReadOnly(true);

  // font
  QFont font("Monospace");
  font.setStyleHint(QFont::Monospace);
  this->setFont(font);

  // color
  this->setStyleSheet("color: white; background-color: black");
};

AssemblyDBLoggerViewer::~AssemblyDBLoggerViewer()
{
}

// ===============================================================================
