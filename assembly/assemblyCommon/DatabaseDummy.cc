/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <DatabaseDummy.h>

#include <QMessageBox>
#include <QTextDocumentFragment>

#include <nqlogger.h>

DatabaseDummy::DatabaseDummy(QObject *parent) : VDatabase(parent)
{
    // Initialise stuff!
}

DatabaseDummy::~DatabaseDummy()
{
    // Any connections to close?
}


bool DatabaseDummy::register_module_name(QString module_name, QString operator_name)
{
    NQLog("DatabaseDummy", NQLog::Message) << "Could not perform step \"Register module by name\"";
    error_message(QString("Module %1 already exists in the database (ID: %2). Cannot register another module with this name.").arg(module_name).arg(123));
    return false;
}

bool DatabaseDummy::MaPSA_to_BP(QString MaPSA_name, QString BP_name, QString glue_name, QString comment)
{
    NQLog("DatabaseDummy", NQLog::Message) << "Performed MaPSA to Baseplate gluing step in DB.";
    return true;
}

bool DatabaseDummy::PSs_to_spacers(QString PSs_name, QString glue_name, QString comment)
{
    return false;
}

bool DatabaseDummy::PSs_to_MaPSA(QString glue_name, QString comment)
{
    return false;
}

bool DatabaseDummy::is_component_available(QString part_name, QString structure_name)
{
    return true;
}

void DatabaseDummy::error_message(QString message)
{
    NQLog("DatabaseDummy", NQLog::Fatal) << QTextDocumentFragment::fromHtml( message ).toPlainText();

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Warning - Database"));
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
}
