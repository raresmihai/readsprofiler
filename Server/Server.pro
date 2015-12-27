QT += core
QT -= gui
QT += sql

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

QTPLUGIN += qsqlmysql
TEMPLATE = app

SOURCES += main.cpp

