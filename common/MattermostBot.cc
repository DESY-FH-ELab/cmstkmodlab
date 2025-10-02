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

#include <iostream>
#include <future>

#include <curl/curl.h>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "MattermostBot.h"

MattermostBot::MattermostBot(std::string config_alias, QObject *parent)
  : QObject(parent),
    channel_(),
    username_(),
    webhook_(),
    config_alias_(config_alias)
{

}

MattermostBot::MattermostBot(QString channel, std::string config_alias, QObject *parent)
  : QObject(parent),
    channel_(channel),
    config_alias_(config_alias)
{

}

MattermostBot::MattermostBot(QString channel, QString username, std::string config_alias, QObject *parent)
  : QObject(parent),
    channel_(channel),
    username_(username),
    config_alias_(config_alias)
{

}

MattermostBot::MattermostBot(QString channel, QString username, QString webhook, std::string config_alias, QObject *parent)
  : QObject(parent),
    channel_(channel),
    username_(username),
    webhook_(webhook),
    config_alias_(config_alias)
{
    config_ = ApplicationConfig::instance();
}

MattermostBot::~MattermostBot()
{

}

int mattermostpost(std::string webhook, std::string data)
{
  curl_global_init(CURL_GLOBAL_ALL);

  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {

    curl_easy_setopt(curl, CURLOPT_URL, webhook.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    res = curl_easy_perform(curl);

    if (res!=CURLE_OK) {
      NQLogWarning("MattermostBot") << "MattermostBot failed: " << curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return true;
}

void MattermostBot::postMessage(const QString& message)
{
  QMutexLocker locker(&mutex_);

  std::string data = "payload={\"channel\": \"";

  if (channel_.isNull()) {
    data += config_->getValue(config_alias_, "mattermostchannel");
  } else {
    data += channel_.toStdString();
  }

  data += "\", \"username\": \"";

  if (username_.isNull()) {
    data += config_->getValue(config_alias_, "mattermostusername");
  } else {
    data += username_.toStdString();
  }

  data += "\", \"text\": \"";
  data += message.toStdString();
  data += "\"}";

  std::string webhook;
  if (webhook_.isNull()) {
    webhook = config_->getValue(config_alias_, "mattermostwebhook");
  } else {
    webhook = webhook_.toStdString();
  }

#ifndef DARWIN
  auto handle = std::async(std::launch::async, mattermostpost, webhook, data);
#else
  mattermostpost(webhook, data);
#endif
}
