QT += core
QT -= gui
QT += sql

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

QTPLUGIN += qsqlmysql
TEMPLATE = app

SOURCES += main.cpp \
    login.cpp \
    register.cpp \
    recomandare.cpp \
    cautare.cpp \
    continut.cpp \
    upload.cpp \
    voteaza.cpp \
    detalii.cpp

HEADERS += \
    login.h \
    register.h \
    recomandare.h \
    cautare.h \
    continut.h \
    upload.h \
    voteaza.h \
    detalii.h

