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
#include "Sysproxy.h"
#ifdef Q_OS_WIN

bool SysProxy::initialize(INTERNET_PER_CONN_OPTION_LIST* options, const unsigned long option_count){
        DWORD dwBufferSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
        options->dwSize = dwBufferSize;

        options->dwOptionCount = option_count;
        options->dwOptionError = 0;
        options->pOptions = static_cast<INTERNET_PER_CONN_OPTION *>(calloc(option_count, sizeof(INTERNET_PER_CONN_OPTION)));
        if (!options->pOptions)
            {
                return false;
        }

        options->pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
        return true;
}

bool SysProxy::offSystemProxyWin(){
    INTERNET_PER_CONN_OPTION_LIST options;
    initialize(&options, 1);
    options.pOptions[0].Value.dwValue = PROXY_TYPE_AUTO_DETECT | PROXY_TYPE_DIRECT;
    bool ret = apply(&options);
    free(options.pOptions);
    return ret;
}

bool SysProxy::setSystemProxyWin(const int port , const QString bypassList){
    INTERNET_PER_CONN_OPTION_LIST options;
    if (bypassList.contains(" "))return false;
    wchar_t *bypasslistWc=new wchar_t;
    wchar_t *serverWc=new wchar_t;
    QString server="127.0.0.1:";
    server.append(port);
    server.toWCharArray(serverWc);
    bypassList.toWCharArray(bypasslistWc);

    if (initialize(&options, 3)==false) return false;

    options.pOptions[0].Value.dwValue = PROXY_TYPE_PROXY | PROXY_TYPE_DIRECT;
    options.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    options.pOptions[1].Value.pszValue = serverWc;
    options.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    options.pOptions[2].Value.pszValue = bypasslistWc;

    delete bypasslistWc;
    delete serverWc;
    bool ret = apply(&options);
    free(options.pOptions);
    return ret;
}

bool SysProxy::apply_connect(INTERNET_PER_CONN_OPTION_LIST* options, LPTSTR conn){
        options->pszConnection = conn;

        BOOL result = InternetSetOption(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION, options, sizeof(INTERNET_PER_CONN_OPTION_LIST));
        if (!result)return false;
        result = InternetSetOption(nullptr, INTERNET_OPTION_PROXY_SETTINGS_CHANGED, nullptr, 0);
        if (!result)return false;
        result = InternetSetOption(nullptr, INTERNET_OPTION_REFRESH, nullptr, 0);
        if (!result)return false;
        return true;
}

bool SysProxy::apply(INTERNET_PER_CONN_OPTION_LIST* options){
    DWORD dwCb = 0;
    DWORD dwEntries = 0;
    DWORD dwRet;
    dwRet = RasEnumEntries(nullptr, nullptr, nullptr, &dwCb, &dwEntries);
    if (dwRet == ERROR_BUFFER_TOO_SMALL){
      LPRASENTRYNAME lpRasEntryName = static_cast<LPRASENTRYNAME>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb));
      if (lpRasEntryName == nullptr)return false;
      for (DWORD i = 0; i < dwEntries; i++){
                  lpRasEntryName[i].dwSize = sizeof(RASENTRYNAME);
              }
      dwRet = RasEnumEntries(nullptr, nullptr, lpRasEntryName, &dwCb, &dwEntries);
      if(dwRet != ERROR_SUCCESS)
      {
          HeapFree(GetProcessHeap(), 0, lpRasEntryName);
          return false;
      }
      apply_connect(options, DEFAULT_LAN_CONNECTION);
      for (DWORD i = 0; i < dwEntries;i++){
            apply_connect(options, lpRasEntryName[i].szEntryName);
        }
      HeapFree(GetProcessHeap(),0,lpRasEntryName);
      return true;
      }
    return apply_connect(options, DEFAULT_LAN_CONNECTION);
}

#endif // Q_OS_WIN
