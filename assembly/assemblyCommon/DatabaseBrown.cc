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

#include <DatabaseBrown.h>

#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QFile>
#include <QTextStream>

#include <nqlogger.h>

DatabaseBrown::DatabaseBrown(QObject *parent, QFileInfo file_path) : VDatabase(parent)
{
    m_file_path = file_path;
    if (!(m_file_path.isDir() && m_file_path.isWritable())){
        error_message(QString("Path for database file storage is not a writable directory: %1").arg(m_file_path.path()));
    }
    error_message(QString("Path for database file storage: %1").arg(m_file_path.canonicalFilePath()));
}

DatabaseBrown::~DatabaseBrown()
{
    // Any connections to close?
}


bool DatabaseBrown::register_module_name(QString module_name, QString operator_name)
{
    NQLog("DatabaseBrown", NQLog::Message) << "Could not perform step \"Register module by name\"";
    error_message(QString("Module %1 already exists in the database (ID: %2). Cannot register another module with this name.").arg(module_name).arg(123));
    return false;
}

bool DatabaseBrown::MaPSA_to_BP(QString MaPSA_name, QString BP_name, QString glue_name, QString comment)
{
    QFile outfile(m_file_path.canonicalFilePath() + "/PSp.txt");
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        error_message(QString("File with name %1 cannot be opened.").arg(outfile.fileName()));
        return false;
    }

    QTextStream outstream(&outfile);
    outstream << MaPSA_name << "\t" << "y" << "\t" << BP_name << "\n";

    outfile.close();
    return true;
}

bool DatabaseBrown::PSs_to_spacers(QString PSs_name, QString glue_name, QString comment)
{
    QFile outfile(m_file_path.canonicalFilePath() + "/PSs.txt");
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        error_message(QString("File with name %1 cannot be opened.").arg(outfile.fileName()));
        return false;
    }

    QTextStream outstream(&outfile);
    outstream << PSs_name << "\t" << "y" << "\n";

    outfile.close();
    return true;
}

bool DatabaseBrown::PSs_to_MaPSA(QString module_name, QString MaPSA_name, QString PSs_name, QString glue_name, QString comment)
{
    QFile outfile(m_file_path.canonicalFilePath() + "/Module.txt");
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        error_message(QString("File with name %1 cannot be opened.").arg(outfile.fileName()));
        return false;
    }

    QTextStream outstream(&outfile);
    outstream << module_name << "\t" << PSs_name << "\t" << MaPSA_name << "\n";

    outfile.close();
    return true;
}

void DatabaseBrown::error_message(QString message)
{
    NQLog("DatabaseBrown", NQLog::Fatal) << QTextDocumentFragment::fromHtml( message ).toPlainText();

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Warning - Database"));
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
}
