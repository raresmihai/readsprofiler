#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T14:50:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        loginwindow.cpp \
    login.cpp \
    registerwindow.cpp \
    register.cpp \
    global.cpp

HEADERS  += loginwindow.h \
    login.h \
    registerwindow.h \
    register.h \
    global.h

FORMS    += loginwindow.ui \
    registerwindow.ui
