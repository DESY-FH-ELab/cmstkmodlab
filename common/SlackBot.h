/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef SLACKBOT_H
#define SLACKBOT_H

#include <QObject>
#include <QMutex>
#include <QString>

#include <ApplicationConfig.h>

/** @addtogroup common
 *  @{
 */

class SlackBot : public QObject
{
  Q_OBJECT
public:


  [[deprecated("Use SlackBot(QString username, QString webhook, QString channel, QObject *parent) instead.")]]
  explicit SlackBot(std::string config_alias, QObject *parent = 0);

  [[deprecated("Use SlackBot(QString username, QString webhook, QString channel, QObject *parent) instead.")]]
  explicit SlackBot(QString username, std::string config_alias, QObject *parent = 0);

  explicit SlackBot(QString username, QString webhook, QString channel, std::string config_alias, QObject *parent = 0);

  ~SlackBot();

public slots:

  void postMessage(const QString& message);

protected:

  QString username_;
  QString webhook_;
  QString channel_;
  QMutex mutex_;
  std::string config_alias_;
  ApplicationConfig* config_;
};

/** @} */

#endif // SLACKBOT_H
