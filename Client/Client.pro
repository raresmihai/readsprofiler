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
    global.cpp \
    mainwindow.cpp \
    upload.cpp \
    cautare.cpp \
    detail.cpp \
    recomandare.cpp

HEADERS  += loginwindow.h \
    login.h \
    registerwindow.h \
    register.h \
    global.h \
    mainwindow.h \
    upload.h \
    cautare.h \
    detail.h \
    recomandare.h

FORMS    += loginwindow.ui \
    registerwindow.ui \
    mainwindow.ui

RESOURCES += \
    imagini.qrc
