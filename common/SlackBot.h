#ifndef SLACKBOT_H
#define SLACKBOT_H

#include <QObject>
#include <QMutex>
#include <QString>

class SlackBot : public QObject
{
  Q_OBJECT
public:
  explicit SlackBot(QObject *parent = 0);
  explicit SlackBot(QString username, QObject *parent = 0);
  explicit SlackBot(QString username, QString webhook, QString channel, QObject *parent = 0);
  ~SlackBot();

public slots:

  void postMessage(const QString& message);

protected:

  QString username_;
  QString webhook_;
  QString channel_;
  QMutex mutex_;
};

#endif // SLACKBOT_H
