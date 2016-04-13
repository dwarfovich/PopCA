QT += core
QT -= gui

CONFIG += c++14
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS   += -fopenmp

TARGET = PopCA
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    animate.cpp \
    animategenealogy.cpp \
    animateproperties.cpp \
    auxiliary.cpp \
    cell.cpp \
    chemical.cpp \
    environment.cpp \
    globalstatistics.cpp \
    labelledanimate.cpp \
    population.cpp \
    projectparameters.cpp \
    world.cpp \
    worldparameters.cpp

HEADERS += \
    animate.h \
    animategenealogy.h \
    animateproperties.h \
    auxiliary.h \
    cell.h \
    chemical.h \
    environment.h \
    globalstatistics.h \
    labelledanimate.h \
    population.h \
    projectparameters.h \
    world.h \
    worldparameters.h

DISTFILES +=
