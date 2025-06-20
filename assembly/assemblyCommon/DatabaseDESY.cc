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

#include <DatabaseDESY.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QNetworkReply>
#include <QEventLoop>

#include <iostream>
#include <nqlogger.h>

DatabaseDESY::DatabaseDESY(QObject *parent, QString base_url, QString token) : VDatabase(parent)
{
    // Initialise stuff!
    network_access_mgr_ = new QNetworkAccessManager(this);

    network_access_mgr_->setTransferTimeout(5000);

    base_url_ = QUrl(base_url);

    base_request_ = QNetworkRequest();
    base_request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    base_request_.setRawHeader("Authorization", ("Token " + token).toUtf8());

    NQLog("DatabaseDESY", NQLog::Debug) << "constructed";
}

DatabaseDESY::~DatabaseDESY()
{
}


bool DatabaseDESY::register_module_name(QString module_name, QString operator_name)
{
    // Check whether module exists in DB - it should not!
    try{
        int return_dbid = get_ID_from_name(module_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Module " << module_name << " exists in the database. ID: " << return_dbid;
        return false;
    } catch(BadResultException bre){
        NQLog("DatabaseDESY", NQLog::Message) << "Module " << module_name << " does not exist in the database (usually good!).";
    }

    // Register Module
    QUrl url_register = base_url_;
    url_register.setPath("/api/part/");

    auto request_register = base_request_;
    request_register.setUrl(url_register);

    QJsonObject data_register;
    data_register["name"] = module_name;
    data_register["group"] = "Production";
    data_register["structure"] = "PS_module";
    data_register["manufacturer"] = "DESY";
    data_register["contact"] = operator_name;
    data_register["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    try{
        auto reply_data_register = this->post(request_register, data_register);

        if(reply_data_register.isEmpty()){
            NQLog("DatabaseDESY", NQLog::Warning) << "Did not receive expected data structure (register module).";
            return false;
        }

        module_dbid_ = reply_data_register.value("id").toInt();
        module_name_ = module_name;
        NQLog("DatabaseDESY", NQLog::Message) << "Registered Module with name " << module_name_ << " and ID " << module_dbid_;

    } catch(BadReplyException bre){
        NQLog("DatabaseDESY", NQLog::Fatal) << "\"register_module_name\" (registering): " << bre.what();
        return false;
    }

    // Start assembly process
    QUrl url_start = base_url_;
    url_start.setPath("/ph2production/psmoduleprocess/start");

    auto request_start = base_request_;
    request_start.setUrl(url_start);

    QJsonObject data_start;
    data_start["parent_part"] = QJsonValue(module_dbid_);

    try{
        auto reply_data_start = this->post(request_start, data_start);
        if(reply_data_start.isEmpty()){
            NQLog("DatabaseDESY", NQLog::Warning) << "Did not receive expected data structure (start process).";
            return false;
        }

        process_dbid_ = reply_data_start.value("process_id").toInt();
        NQLog("DatabaseDESY", NQLog::Message) << "Started assembly process with ID " << process_dbid_;
    } catch(BadReplyException bre){
        NQLog("DatabaseDESY", NQLog::Fatal) << "\"register_module_name\": (start assembly process) " << bre.what();
        return false;
    }

    return true;
}

bool DatabaseDESY::MaPSA_to_BP(QString MaPSA_name, QString BP_name, QString glue_name, QString comment)
{
    try{
        // Get MaPSA_dbib_ from MaPSA_name_
        int return_dbid = get_ID_from_name(MaPSA_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained MaPSA ID (\"Take MaPSA\"): " << return_dbid;
        MaPSA_dbid_ = return_dbid;
        MaPSA_name_ = MaPSA_name;

        // // Take MaPSA
        // Get Task ID
        int task_id = get_next_task();
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained next task (\"Take MaPSA\") with ID " << task_id;

        // Assign Task
        assign_task(task_id);
        NQLog("DatabaseDESY", NQLog::Message) << "Assigned next task (\"Take MaPSA\") with ID " << task_id;

        // Perform Task
        QJsonObject data_perform;
        data_perform["comment"] = comment;
        data_perform["part"] = QJsonValue(MaPSA_dbid_);

        perform_task(task_id, data_perform);
        NQLog("DatabaseDESY", NQLog::Message) << "Performed task (\"Take MaPSA\") with ID " << task_id;

    } catch(BadResultException bre){
        NQLog("DatabaseDESY", NQLog::Warning) << "Could not perform step \"Take MaPSA\": " << bre.what();
        return false;
    }

    try{
        // // Glue MaPSA to BP
        // Get BP_dbib_ from BP_name_
        int return_MaPSA_dbid = get_ID_from_name(BP_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained Baseplate ID (\"Glue MaPSA to Baseplate\"): " << return_MaPSA_dbid;
        BP_dbid_ = return_MaPSA_dbid;
        BP_name_ = BP_name;

        // Get Glue1_dbib_ from glue_name
        int return_glue_dbid = get_ID_from_name(glue_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained Glue ID (\"Glue MaPSA to Baseplate\"): " << return_glue_dbid;
        Glue1_dbid_ = return_glue_dbid;

        // Get Task ID
        int task_id = get_next_task();
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained next task (\"Glue MaPSA to Baseplate\") with ID " << task_id;

        // Assign Task
        assign_task(task_id);
        NQLog("DatabaseDESY", NQLog::Message) << "Assigned next task (\"Glue MaPSA to Baseplate\") with ID " << task_id;

        // Perform Task
        QJsonObject data_perform;
        data_perform["comment"] = "";
        data_perform["part"] = QJsonValue(BP_dbid_);
        data_perform["glue"] = QJsonValue(Glue1_dbid_);

        perform_task(task_id, data_perform);
        NQLog("DatabaseDESY", NQLog::Message) << "Performed task (\"Glue MaPSA to Baseplate\") with ID " << task_id;

    } catch(BadResultException bre){
        NQLog("DatabaseDESY", NQLog::Warning) << "Could not perform step \"Glue MaPSA to Baseplate\": " << bre.what();
        return false;
    }

    return true;
}

bool DatabaseDESY::PSs_to_spacers(QString PSs_name, QString glue_name, QString comment)
{
    try{
        // Get PSs_dbib_ from PSs_name_
        int return_dbid = get_ID_from_name(PSs_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained PSs ID (\"Take PSs\"): " << return_dbid;
        PSs_dbid_ = return_dbid;
        PSs_name_ = PSs_name;

        // // Take PSs
        // Get Task ID
        int task_id = get_next_task();
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained next task (\"Take PSs\") with ID " << task_id;

        // Assign Task
        assign_task(task_id);
        NQLog("DatabaseDESY", NQLog::Message) << "Assigned next task (\"Take PSs\") with ID " << task_id;

        // Perform Task
        QJsonObject data_perform;
        data_perform["comment"] = comment;
        data_perform["part"] = QJsonValue(PSs_dbid_);

        perform_task(task_id, data_perform);
        NQLog("DatabaseDESY", NQLog::Message) << "Performed task (\"Take PSs\") with ID " << task_id;

    } catch(BadResultException bre){
        NQLog("DatabaseDESY", NQLog::Warning) << "Could not perform step \"Take PSs\": " << bre.what();
        return false;
    }

    try{
        // // Glue PSs to Spacers
        // Get Glue2_dbib_ from glue_name
        int return_dbid = get_ID_from_name(glue_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained Glue ID (\"Glue PSs to Spacers\"): " << return_dbid;
        Glue2_dbid_ = return_dbid;

        // Get Task ID
        int task_id = get_next_task();
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained next task (\"Glue PSs to Spacers\") with ID " << task_id;

        // Assign Task
        assign_task(task_id);
        NQLog("DatabaseDESY", NQLog::Message) << "Assigned next task (\"Glue PSs to Spacers\") with ID " << task_id;

        // Perform Task
        QJsonObject data_perform;
        data_perform["comment"] = "";
        data_perform["glue"] = QJsonValue(Glue2_dbid_);

        perform_task(task_id, data_perform);
        NQLog("DatabaseDESY", NQLog::Message) << "Performed task (\"Glue PSs to Spacers\") with ID " << task_id;

    } catch(BadResultException bre){
        NQLog("DatabaseDESY", NQLog::Warning) << "Could not perform step \"Glue PSs to Spacers\": " << bre.what();
        return false;
    }

    return true;
}

bool DatabaseDESY::PSs_to_MaPSA(QString glue_name, QString comment)
{
    try{
        // // Glue PSs to MaPSA
        // Get Glue3_dbib_ from glue_name
        int return_dbid = get_ID_from_name(glue_name);
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained Glue ID (\"Glue PSs to MaPSA\"): " << return_dbid;
        Glue3_dbid_ = return_dbid;

        // Get Task ID
        int task_id = get_next_task();
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained next task (\"Glue PSs to MaPSA\") with ID " << task_id;

        // Assign Task
        assign_task(task_id);
        NQLog("DatabaseDESY", NQLog::Message) << "Assigned next task (\"Glue PSs to MaPSA\") with ID " << task_id;

        // Perform Task
        QJsonObject data_perform;
        data_perform["comment"] = comment;
        data_perform["glue"] = QJsonValue(Glue3_dbid_);

        perform_task(task_id, data_perform);
        NQLog("DatabaseDESY", NQLog::Message) << "Performed task (\"Glue PSs to MaPSA\") with ID " << task_id;

    } catch(BadResultException bre){
        NQLog("DatabaseDESY", NQLog::Warning) << "Could not perform step \"Glue PSs to MaPSA\": " << bre.what();
        return false;
    }

    return true;
}

QJsonObject DatabaseDESY::post(QNetworkRequest request, QJsonObject json_object)
{
    QJsonDocument json_doc(json_object);
    QByteArray bytedata = json_doc.toJson();

    QNetworkReply *reply = network_access_mgr_->post(request, bytedata);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error() == QNetworkReply::NoError){
        auto read_message = reply->readAll();

        QJsonDocument reply_document = QJsonDocument::fromJson(read_message);
        QJsonObject reply_object = reply_document.object();

        return reply_object;
    }
    else{
        QString err = reply->errorString();
        QString full_reply = QString::fromUtf8(reply->readAll());
        NQLog("DatabaseDESY", NQLog::Warning) << "post - returned error message: " << err;
        NQLog("DatabaseDESY", NQLog::Warning) << "post - replied: " << full_reply;

        throw BadReplyException(err, full_reply);
    }

    reply->deleteLater();
    return QJsonObject();
}

QJsonObject DatabaseDESY::get(QNetworkRequest request)
{
    QNetworkReply *reply = network_access_mgr_->get(request);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error() == QNetworkReply::NoError){
        auto read_message = reply->readAll();

        QJsonDocument reply_document = QJsonDocument::fromJson(read_message);
        QJsonObject reply_object = reply_document.object();

        return reply_object;
    }
    else{
        QString err = reply->errorString();
        QString full_reply = QString::fromUtf8(reply->readAll());
        NQLog("DatabaseDESY", NQLog::Warning) << "get - returned error message: " << err;
        NQLog("DatabaseDESY", NQLog::Warning) << "get - replied: " << full_reply;

        throw BadReplyException(err, full_reply);
    }

    reply->deleteLater();
    return QJsonObject();
}

int DatabaseDESY::get_next_task()
{
    QUrl url_gettask = base_url_;
    url_gettask.setPath("/ph2production/api/task/");
    url_gettask.setQuery(QString("?status=NEW&process=%1").arg(process_dbid_));

    auto request_gettask = base_request_;
    request_gettask.setUrl(url_gettask);

    try{
        auto reply_data_gettask = this->get(request_gettask);
        if(reply_data_gettask.isEmpty()){
            NQLog("DatabaseDESY", NQLog::Warning) << "Did not receive expected data structure (get next task).";
            throw BadResultException("\"get_next_task\" failed to obtain task ID - DB returned empty object.");
        }

        auto results_object = reply_data_gettask.value("results").toArray().at(0).toObject();
        int task_id = results_object.value("id").toInt();
        NQLog("DatabaseDESY", NQLog::Message) << "Obtained task with ID " << task_id;
        return task_id;
    } catch(BadReplyException bre){
        NQLog("DatabaseDESY", NQLog::Fatal) << "\"get_next_task\": " << bre.what();
        throw BadResultException("\"get_next_task\" failed to obtain task ID - received bad reply from DB.");
    }
}

void DatabaseDESY::assign_task(int task_id)
{
    QUrl url_assigntask = base_url_;
    url_assigntask.setPath(QString("/ph2production/task/%1/assign").arg(task_id));

    auto request_assigntask = base_request_;
    request_assigntask.setUrl(url_assigntask);

    try{
        auto reply_data_assigntask = this->get(request_assigntask);
        if(reply_data_assigntask.isEmpty()){
            NQLog("DatabaseDESY", NQLog::Warning) << "Did not receive expected data structure (assign next task).";
            throw BadResultException("\"assign_task\" failed to assign task with ID " + std::to_string(task_id));
        }
        NQLog("DatabaseDESY", NQLog::Message) << "Assigned task with ID " << task_id;
    } catch(BadReplyException bre){
        NQLog("DatabaseDESY", NQLog::Fatal) << "\"assign_task\": " << bre.what();
        throw BadResultException("\"assign_task\" failed to assign task with ID - received bad reply from DB.");
    }
}

void DatabaseDESY::perform_task(int task_id, QJsonObject data_performtask)
{
    QUrl url_performtask = base_url_;
    url_performtask.setPath(QString("/ph2production/task/%1/perform").arg(task_id));

    auto request_performtask = base_request_;
    request_performtask.setUrl(url_performtask);

    try{
        auto reply_data_performtask = this->post(request_performtask, data_performtask);
        if(reply_data_performtask.isEmpty()){
            NQLog("DatabaseDESY", NQLog::Warning) << "Did not receive expected data structure (perform task).";
            throw BadResultException("\"perform_task\" failed to perform task with ID " + std::to_string(task_id));
        }
        NQLog("DatabaseDESY", NQLog::Message) << "Performed task with ID " << task_id;
    } catch(BadReplyException bre){
        NQLog("DatabaseDESY", NQLog::Fatal) << "\"perform_task\": " << bre.what();
        throw BadResultException("\"perform_task\" failed to perform task with ID - received bad reply from DB.");
    }
}

int DatabaseDESY::get_ID_from_name(QString part_name)
{
    QUrl url_getid = base_url_;
    url_getid.setPath("/api/part/");
    url_getid.setQuery(QString("?name=%1").arg(part_name));

    auto request_getid = base_request_;
    request_getid.setUrl(url_getid);

    try{
        auto reply_data_getid = this->get(request_getid);
        if(reply_data_getid.isEmpty()){
            NQLog("DatabaseDESY", NQLog::Warning) << "Could not find part with name " << part_name;
            throw BadResultException("\"get_ID_from_name\" failed to get ID from part " + part_name.toStdString());
        }

        auto results_object = reply_data_getid.value("results").toArray().at(0).toObject();
        int part_id = results_object.value("id").toInt();
        NQLog("DatabaseDESY", NQLog::Message) << "Part " << part_name << " found. ID: " << part_id;

        return part_id;
    } catch(BadReplyException bre){
        NQLog("DatabaseDESY", NQLog::Fatal) << "\"get_ID_from_name\": " << bre.what();
        throw BadResultException("\"get_ID_from_name\" failed get ID from part " + part_name.toStdString() + " - received bad reply from DB.");
    }
}
