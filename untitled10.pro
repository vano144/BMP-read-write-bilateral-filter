QT += core
QT += gui

CONFIG += c++11

TARGET = untitled10
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    bmpreader.cpp \
    bmpimage.cpp

HEADERS += \
    main.h \
    bmpreader.h \
    bmpimage.h
