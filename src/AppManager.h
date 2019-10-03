#ifndef GLOBAL_H
#define GLOBAL_H

#include "trojan/src/config.h"

#include <QDebug>

#include <QtGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QMessageBox>
#include <QProcess>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#ifdef Q_OS_WIN
#include <QSettings>
#include "Sysproxy.h"
#endif // Q_OS_WIN

#define APP_DATA_DIR QStandardPaths::writableLocation(QStandardPaths::DataLocation)
#define SERVER_CONFIG_PATH APP_DATA_DIR + "/client.json"
#define CLIENT_CONFIG_PATH APP_DATA_DIR + "/server.json"
#define APP_CONFIG_PATH APP_DATA_DIR + "/settings.json"

class AppManager
{
public:
  enum LogLevel {
      ALL = 0,
      INFO = 1,
      WARN = 2,
      ERROR = 3,
      FATAL = 4,
      OFF = 5
  };

  static QJsonObject client_config_obj;
  static QJsonObject server_config_obj;
  static QString client_config_path;
#ifdef Q_OS_WIN
  static QString clint_real_config_path;
  static QJsonObject client_real_config_obj;
#endif // Q_OS_WIN
  static QString server_config_path;
  static Config::RunType current_run_type;

  static bool checkDir(const QString &dir);
  static bool checkFile(const QString &path, const QString &copy);
  static bool loadJson(const QString &fileDir, QJsonObject &obj);
  static bool writeJson(const QString &fileDir, QJsonObject &obj);
  static bool loadAppConfig();
  static bool writeAppConfig();
  static bool loadTrojanConfig();
  static bool writeTrojanConfig();

  static void setSystemProxy(const bool &enabled);

  static void popMessageBox(LogLevel l, QString info, QWidget *parent);

  static QString logLevelToString(LogLevel l);
};

#endif // GLOBAL_H
