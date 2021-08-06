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

#include <AssemblyDBLoggerModel.h>

AssemblyDBLoggerModel::AssemblyDBLoggerModel(const QString path) :
dblogfilepath_(path)
{
    writeLogHeader(); //Write header first

    reading_ = false;
    overwrite_ = false;
    appendToEnd_ = true;
    last_read_endline_ = 0;
    verbosity_ = 0;

    //Set timer -- used to update log display in GUI
    timer_ = new QTimer;
    timer_->setInterval(500); //in ms
    timer_->start();
    connect(timer_, SIGNAL(timeout()), this, SLOT(read_newlines()));
}

AssemblyDBLoggerModel::~AssemblyDBLoggerModel()
{
}

//Returns a string corresponding to the current timestamp
QString AssemblyDBLoggerModel::getTimestamp(QString format/*="hh:mm:ss"*/)
{
    //-- Examples of valid formats are : (cf. https://doc.qt.io/archives/qt-4.8/qdatetime.html)
    // dd.MM.yyyy       21.05.2001
    // ddd MMMM d yy    Tue May 21 01
    // hh:mm:ss.zzz     14:13:09.120
    // hh:mm:ss.z       14:13:09.12
    // h:m:s ap         2:13:9 pm
    // format="yyyyMMdd_hhmmss" <-> '20200414_131052'

    return QDateTime::currentDateTime().toString(format);
}

//Writes default header to log
void AssemblyDBLoggerModel::writeLogHeader()
{
    QMutexLocker locker(&mutex_);
    QFile file(dblogfilepath_);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text) == false) {return;}
    QTextStream stream(&file);

    stream << "########################################\n";
    stream << "########################################\n";
    stream << "###### PS AUTOMATED ASSEMBLY LOG #######\n";
    stream << "########################################\n";
    stream << "########################################\n\n";

    // QLocale locale = QLocale(QLocale::English, QLocale::Germany);
    QLocale locale = QLocale(QLocale::C, QLocale::Germany); //Simplified english language format
    QString englishDate = locale.toString(QDateTime::currentDateTime());
    stream << "Date : " << englishDate << "\n\n";

    stream << "-----------------------" << "\n\n";

    // stream << getTimestamp() << ": Launch GUI\n";
    stream << "\n\n";
    stream.flush();
    file.close();

    return;
}

//Writes desired message to log
void AssemblyDBLoggerModel::writeMessage(QString message)
{
    // std::cout<<"\e[1;31m writeMessage \e[0m"<<message.toUtf8().constData()<<std::endl;

    QMutexLocker locker(&mutex_); //Lock mutex to protect following data from being accessed by multiple threads at once
    QFile file(dblogfilepath_);

    if(this->appendToEnd_) //Message will be appended at end of file [default]
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) ) {return;} //Open file in write mode

        QTextStream stream(&file);
        stream << "[" << getTimestamp() << "] "; //Print timestamp
        stream << message << endl; //Print message
        file.close();
    }
    else if(!this->appendToEnd_) //Message will be appended at beginning of file
    {
        if(!file.open(QFile::ReadOnly | QFile::Text) ) {return;} //Open file in read mode
        QByteArray buffer = file.readAll(); //Get file's content
        file.close();

        file.open(QFile::WriteOnly | QFile::Text); //Re-open file in write mode
        QTextStream out(&file);
        out << "[" << getTimestamp() << "] "; //Print timestamp
        out << message << endl << endl; //Print message
        out << buffer; //Print previous file's content
        file.close();

        overwrite_ = true; //Tell the Viewer it needs to update the entire GUI display
    }

    return;
}

//After each time interval of timer_, read the DBLog file and look for new lines to display in the GUI tab
void AssemblyDBLoggerModel::read_newlines()
{
    // std::cout<<"\e[1;31m read_newlines \e[0m"<<std::endl;

    if(reading_ == false) //Avoid concurrent writes
    {
        reading_ = true;

        if(overwrite_) //Need to overwrite previous content display
        {
            NQLog("AssemblyDBLoggerModel", NQLog::Debug) << "read_newlines" << ": emitting signal \"overwrite_content_view(QStringList)\"";
            emit overwrite_content_view();
        }

        QFile file(dblogfilepath_);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        {
            NQLog("AssemblyDBLoggerModel", NQLog::Warning) << "read_newlines" << ": file " << dblogfilepath_ << " not opened...";
            return;
        }

        QTextStream in(&file);
        uint line_number(0);
        QStringList qstr_list;
        while(in.atEnd() == false)
        {
            const QString line = in.readLine();
            // std::cout<<"line : "<<line.toUtf8().constData()<<std::endl;
            ++line_number;
            if(overwrite_ || line_number > last_read_endline_) {qstr_list.append(line);} //Store all 'new' lines
        }

        NQLog("AssemblyDBLoggerModel", NQLog::Debug) << "read_newlines" << ": emitting signal \"refresh_content_view(QStringList)\"";
        emit refresh_content_view(qstr_list);

        last_read_endline_ = line_number; //Update current line index
        file.close();

        overwrite_ = false;
        reading_ = false;
    }

    return;
}

//Choose whether the next message will be appended at the beginning or end of the DBLog file
void AssemblyDBLoggerModel::setAppendMode(const QString& mode)
{
    if(mode == "End of log file") {appendToEnd_ = true;}
    else if(mode == "Start of log file") {appendToEnd_ = false;}
    else {NQLog("AssemblyDBLoggerModel", NQLog::Warning) << "wrong value for variable \"appendToEnd_\" " << appendToEnd_;}

    return;
}

// ===============================================================================
