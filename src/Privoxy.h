#ifndef PRIVOXY_H
#define PRIVOXY_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <QObject>
#include <QString>
#include <QProcess>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <AppManager.h>

#include <boost/asio.hpp>

class Privoxy : public QProcess
{
    Q_OBJECT
public:
    explicit Privoxy(QObject *parent = nullptr);
    void configure(bool localOnly,unsigned short &trojanPort);
private:
    unsigned short getAvailablePort();
    QString _config;

signals:
    void started(const bool &success);
    void exception(const QString &wtf);
public slots:
    void stop();
};

#endif // Q_OS_WIN
#endif // PRIVOXY_H
