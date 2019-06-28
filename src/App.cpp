/*
 * This file is part of the trojan project.
 * Trojan is an unidentifiable mechanism that helps you bypass GFW.
 * Copyright (C) 2018  Light Bob
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "App.h"

App::App(int &argc, char **argv)
  : QApplication(argc, argv)
  , window(new Window())
  , service(new ServiceThread(this))
#ifdef Q_OS_WIN
  , privoxy(new Privoxy(this))
#endif // Q_OS_WIN
{
  QApplication::setQuitOnLastWindowClosed(false);

  AppManager::checkDir(APP_DATA_DIR);
  AppManager::checkFile(APP_CONFIG_PATH, ":/file/file/settings.json");
  AppManager::loadAppConfig();
  AppManager::loadTrojanConfig();

  connect(service, &ServiceThread::exception, this, &App::popErrorBox);
  connect(service, &ServiceThread::started, window, &Window::onServerStarted);
  connect(window, &Window::startTriggered, this, &App::startTrojan);
  connect(window, &Window::stopTriggered, this, &App::stopTrojan);

  window->setCurrentMode(AppManager::current_run_type);
}

App::~App()
{
  AppManager::setSystemProxy(false);
  AppManager::writeAppConfig();
  Log::log_with_date_time("Exit", Log::INFO);
}

void App::startTrojan()
{
#ifdef Q_OS_WIN
  AppManager::clint_real_config_path=APP_DATA_DIR+"/client.real.json"; //先写在这里

  AppManager::loadJson(AppManager::client_config_path,AppManager::clint_real_config_obj);
  unsigned short local_port=AppManager::clint_real_config_obj.take("local_port").toString().toUShort() ;
  privoxy->configure(true,local_port);
  AppManager::clint_real_config_obj.insert("local_port",local_port);
  AppManager::writeJson(AppManager::clint_real_config_path,AppManager::clint_real_config_obj);

  service->config().load(AppManager::clint_real_config_path.toStdString());
  privoxy->start();
#else
  service->config().load(AppManager::current_run_type == Config::CLIENT ?
                             AppManager::client_config_path.toStdString() : AppManager::server_config_path.toStdString());
#endif // Q_OS_WIN
  service->start();
}

void App::stopTrojan()
{
  service->stop();
  privoxy->kill();
}

void App::popErrorBox(const QString &what)
{
  window->onServerStarted(false);
  AppManager::popMessageBox(AppManager::ERROR, what, window);
}
