 # This file is part of the trojan project.
 # Trojan is an unidentifiable mechanism that helps you bypass GFW.
 # Copyright (C) 2018  Light Bob
 #
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.


QT       += core gui widgets
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app
TARGET = Trojan

CONFIG += c++11

mac{
    QMAKE_INCDIR += /System/Library/Frameworks/AppKit.framework/Headers /System/Library/Frameworks/Security.framework/Headers /System/Library/Frameworks/ServiceManagement.framework/Headers
    QMAKE_LFLAGS += -framework IOKit -framework Cocoa -framework Security
    QMAKE_LFLAGS += -F /System/Library/Frameworks/Security.framework/
    LIBS += -framework Security
    INCLUDEPATH += /usr/local/include/
    ICON = trojan-qt.icns
}

unix{
    INCLUDEPATH += /usr/local/opt/openssl/include/
    INCLUDEPATH += /usr/local/opt/boost/include/
    INCLUDEPATH += /usr/include/mysql
    LIBS += -L/usr/local/lib -lpthread
    LIBS += -L/usr/local/opt/openssl/lib -lssl -lcrypto
    LIBS += -L/usr/local/opt/boost/lib -lboost_system
    LIBS += -lmysqlclient

#    The right way to do it with pkg-config, but it points to the deprecated system OpenSSL.
#    PKGCONFIG += openssl
}

win32{#win64 also included
    INCLUDEPATH += $$(OPENSSL64_PATH)/include
    LIBS += -L$$(OPENSSL64_PATH)/lib -llibcrypto -llibssl
    LIBS += -lwsock32 -lws2_32
    LIBS += -lCrypt32
    DEFINES +=WIN32_LEAN_AND_MEAN
    RC_ICONS = trojan-qt.ico
    BOOST_DIR_FOLDERS=$$system(dir $$(BOOST_PATH)\include /b /ad-s-h /on)
    for(folder,BOOST_DIR_FOLDERS):contains(folder,boost-[1-9]_[6-9][0-9]){
        folder_folder=$$system(dir $$(BOOST_PATH)\include\\$$folder /b /ad-s-h,boost)
        contains(folder_folder,boost):INCLUDEPATH +=$$(BOOST_PATH)/include/$$folder
    }
}

SOURCES += \
    trojan/src/authenticator.cpp \
    trojan/src/clientsession.cpp \
    trojan/src/config.cpp \
    trojan/src/forwardsession.cpp \
    trojan/src/log.cpp \
    trojan/src/serversession.cpp \
    trojan/src/service.cpp \
    trojan/src/session.cpp \
    trojan/src/socks5address.cpp \
    trojan/src/ssldefaults.cpp \
    trojan/src/sslsession.cpp \
    trojan/src/trojanrequest.cpp \
    trojan/src/udppacket.cpp \
    trojan/src/version.cpp \
    App.cpp \
    main.cpp \
    Window.cpp \
    Button.cpp \
    IconButton.cpp \
    BodyWidget.cpp \
    StackedWidget.cpp \
    ServiceThread.cpp \
    ConfigEditor.cpp \
    AppManager.cpp

HEADERS += \
    trojan/src/authenticator.h \
    trojan/src/clientsession.h \
    trojan/src/config.h \
    trojan/src/forwardsession.h \
    trojan/src/log.h \
    trojan/src/serversession.h \
    trojan/src/service.h \
    trojan/src/session.h \
    trojan/src/ssldefaults.h \
    trojan/src/sslsession.h \
    trojan/src/trojanrequest.h \
    trojan/src/version.h \
    App.h \
    Window.h \
    Button.h \
    Version.h \
    IconButton.h \
    BodyWidget.h \
    StackedWidget.h \
    ServiceThread.h \
    ConfigEditor.h \
    AppManager.h

RESOURCES += \
    img.qrc \
    file.qrc
