#ifndef LOG_H
#define LOG_H

#include <nqlogger.h>

#include <string>

#include <QString>
#include <QTextStream>

namespace Log {

  void KILL   (const std::string&);
  void WARNING(const std::string&);

  extern int verbosity;

  class NQLog2
  {
   public:

    enum LogLevel {
        Debug       = 0,
        Spam        = 1,
        Message     = 2,
        Warning     = 3,
        Critical    = 4,
        Fatal       = 5
    };

    explicit NQLog2(const QString&, const LogLevel level=NQLog2::Message);
    virtual ~NQLog2();

    inline NQLog2 &operator<<(QChar t) { stream_ << '\'' << t << '\''; return *this;}
    inline NQLog2 &operator<<(char t) { stream_ << t; return *this; }

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    inline NQLog2 &operator<<(QBool t) { stream_ << (bool(t != 0) ? "true" : "false"); return *this; }
#endif
    inline NQLog2 &operator<<(bool t) { stream_ << (t ? "true" : "false"); return *this; }

    inline NQLog2 &operator<<(signed short t) { stream_ << t; return *this; }
    inline NQLog2 &operator<<(unsigned short t) { stream_ << t; return *this; }

    inline NQLog2 &operator<<(signed int t) { stream_ << t; return *this; }
    inline NQLog2 &operator<<(unsigned int t) { stream_ << t; return *this; }

    inline NQLog2 &operator<<(signed long t) { stream_ << t; return *this; }
    inline NQLog2 &operator<<(unsigned long t) { stream_ << t; return *this; }

    inline NQLog2 &operator<<(qint64 t) { stream_ << QString::number(t); return *this; }
    inline NQLog2 &operator<<(quint64 t) { stream_ << QString::number(t); return *this; }

    inline NQLog2 &operator<<(float t) { stream_ << t; return *this; }
    inline NQLog2 &operator<<(double t) { stream_ << t; return *this; }

    inline NQLog2 &operator<<(const char* t) { stream_ << QString::fromLatin1(t); return *this; }
    inline NQLog2 &operator<<(const std::string& t) { stream_ << QString::fromStdString(t); return *this; }
    inline NQLog2 &operator<<(const QString& t) { stream_ << '\"' << t << '\"'; return *this; }
    inline NQLog2 &operator<<(const QStringRef & t) { return operator<<(t.toString()); }
    inline NQLog2 &operator<<(const QLatin1String &t) { stream_ << '\"'  << t.latin1() << '\"'; return *this; }
    inline NQLog2 &operator<<(const QByteArray & t) { stream_  << '\"' << t << '\"'; return *this; }

    inline NQLog2 &operator<<(const void * t) { stream_ << t; return *this; }

    inline NQLog2 &operator<<(QTextStreamFunction f) { stream_ << f; return *this; }
    inline NQLog2 &operator<<(QTextStreamManipulator m) { stream_ << m; return *this; }

   protected:

    QString module_;
    LogLevel level_;
    QString buffer_;
    QTextStream stream_;
  };

}

#endif
