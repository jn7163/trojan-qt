#ifndef PRIVOXY_H
#define PRIVOXY_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <QObject>
#include <QProcess>
#include <boost/asio.hpp>

class Privoxy : public QObject
{
    Q_OBJECT
public:
    explicit Privoxy(QObject *parent = nullptr);
protected:
    void run();
private:
    unsigned short getAvailablePort();
signals:
    void started(const bool &success);
    void exception(const QString &wtf);
public slots:
    void stop();
};

#endif // Q_OS_WIN
#endif // PRIVOXY_H
