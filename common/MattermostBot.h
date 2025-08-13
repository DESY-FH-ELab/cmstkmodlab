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

#ifndef MATTERMOSTBOT_H
#define MATTERMOSTBOT_H

#include <QObject>
#include <QMutex>
#include <QString>

/** @addtogroup common
 *  @{
 */

class MattermostBot : public QObject
{
  Q_OBJECT
public:

  [[deprecated("Use MattermostBot(QString channel, QString username, QString webhook, QObject *parent) instead.")]]
  explicit MattermostBot(std::string config_alias, QObject *parent = 0);

  [[deprecated("Use MattermostBot(QString channel, QString username, QString webhook, QObject *parent) instead.")]]
  explicit MattermostBot(QString channel, std::string config_alias, QObject *parent = 0);

  [[deprecated("Use MattermostBot(QString channel, QString username, QString webhook, QObject *parent) instead.")]]
  explicit MattermostBot(QString channel, QString username, std::string config_alias, QObject *parent = 0);

  explicit MattermostBot(QString channel, QString username, QString webhook, std::string config_alias, QObject *parent = 0);

  ~MattermostBot();

public slots:

  void postMessage(const QString& message);

protected:

  QString channel_;
  QString username_;
  QString webhook_;
  QMutex mutex_;
  std::string config_alias_;
  ApplicationConfig* config_;
};

/** @} */

#endif // MATTERMOSTBOT_H
