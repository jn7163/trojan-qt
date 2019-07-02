/*
 * Copyright (c) 2019  JonathanHouten
 *
 *  This file is free software: you may copy, redistribute and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright (c) 2016-2019, Noisyfox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SYSPROXY_H
#define SYSPROXY_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <windows.h>
#include <wininet.h>
#include "tchar.h"
#include <ras.h>
#include <raserror.h>
#include <QString>

#define DEFAULT_LAN_CONNECTION nullptr

class SysProxy
{
private:
    static bool apply(INTERNET_PER_CONN_OPTION_LIST* options);
    static bool initialize(INTERNET_PER_CONN_OPTION_LIST* options, const unsigned long option_count);
    static bool apply_connect(INTERNET_PER_CONN_OPTION_LIST* options, LPTSTR conn);
public:
    static bool setSystemProxyWin(const int port, const QString bypassList);
    static bool offSystemProxyWin();

};

#endif // Q_OS_WIN
#endif // SYSPROXY_H
