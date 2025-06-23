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

#ifndef DATABASEDESY_H
#define DATABASEDESY_H

#include <VDatabase.h>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>

class DatabaseDESY : public VDatabase
{
 Q_OBJECT

  public:
      explicit DatabaseDESY(QObject* parent, QString base_url, QString token);
      ~DatabaseDESY();

      bool register_module_name(QString, QString);
      bool MaPSA_to_BP(QString, QString, QString, QString="");
      bool PSs_to_spacers(QString, QString, QString="");
      bool PSs_to_MaPSA(QString, QString);

  protected:

      QJsonObject post(QNetworkRequest request, QJsonObject json_object);
      QJsonObject get(QNetworkRequest request);

      int get_next_task(QString task_name);
      void assign_task(int task_id);
      void perform_task(int task_id, QJsonObject data);
      int get_ID_from_name(QString part_name, QString structure_name="");
      int validate_glue_mixture(QString glue_name);

      QNetworkAccessManager* network_access_mgr_;

      QUrl base_url_;
      QNetworkRequest base_request_;

      int process_dbid_;

};

class BadReplyException : public std::exception {
  private:
      QString error_message;
      QString full_reply;
      std::string return_message;
  public:
      BadReplyException(QString msg, QString reply) : error_message(std::move(msg)), full_reply(std::move(reply)) {}
      const char* what () {
          return_message = "Bad reply from database.\n\tError: " + error_message.toStdString() + "\n\tFull Reply: " + full_reply.toStdString();
          return return_message.c_str();
      }
};

class BadResultException : public std::exception {
  private:
      QString message;
      std::string return_message;
  public:
      BadResultException(QString msg) : message(std::move(msg)) {}
      const char* what () {
          return_message = "Failed to perform DB task: " + message.toStdString();
          return return_message.c_str();
      }
};

class PartDoesNotExistException : public std::exception {
  private:
      QString part_;
      QString structure_;
      std::string return_message_;
  public:
      PartDoesNotExistException(QString part, QString structure="") : part_(std::move(part)), structure_(std::move(structure)) {}
      const char* what () {
          return_message_ = "Required part does not exist: " + part_.toStdString() + (structure_.isEmpty() ? "" : (" (structure: " + structure_.toStdString() + ")"));
          return return_message_.c_str();
      }
};

#endif // DATABASEDESY_H
