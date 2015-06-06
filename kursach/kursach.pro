#-------------------------------------------------
#
# Project created by QtCreator 2015-05-04T11:11:12
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = kursach
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    workwithstring.cpp \
    connectionsettings.cpp \
    readsettings.cpp \
    user.cpp \
    connectclient.cpp \
    share.cpp

HEADERS  += client.h \
    workwithstring.h \
    connectionsettings.h \
    user.h \
    ReadSettings.h \
    connectclient.h \
    share.h

FORMS    += client.ui \
    connectionsettings.ui \
    user.ui \
    share.ui
