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

#include <iostream>
#include <future>

#include <curl/curl.h>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "SlackBot.h"

SlackBot::SlackBot(QObject *parent)
  : QObject(parent),
    username_(),
    webhook_(),
    channel_()
{

}

SlackBot::SlackBot(QString username, QObject *parent)
  : QObject(parent),
    username_(username)
{

}

SlackBot::SlackBot(QString username, QString webhook, QString channel, QObject *parent)
  : QObject(parent),
    username_(username),
    webhook_(webhook),
    channel_(channel)
{

}

SlackBot::~SlackBot()
{

}

int slackpost(std::string webhook, std::string data)
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
      NQLogWarning("SlackBot") << "SlackBot failed: " << curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return true;
}

void SlackBot::postMessage(const QString& message)
{
  QMutexLocker locker(&mutex_);

  std::string data = "payload={\"channel\": \"";

  if (channel_.isNull()) {
    data += ApplicationConfig::instance()->getValue("slackchannel");
  } else {
    data += channel_.toStdString();
  }

  data += "\", \"username\": \"";

  if (username_.isNull()) {
    data += ApplicationConfig::instance()->getValue("slackusername");
  } else {
    data += username_.toStdString();
  }

  data += "\", \"text\": \"";
  data += message.toStdString();
  data += "\", \"icon_emoji\": \":ghost:\"}";

  std::string webhook;
  if (webhook_.isNull()) {
    webhook = ApplicationConfig::instance()->getValue("slackwebhook");
  } else {
    webhook = webhook_.toStdString();
  }

#ifndef DARWIN
  auto handle = std::async(std::launch::async, slackpost, webhook, data);
#else
  slackpost(webhook, data);
#endif
}
