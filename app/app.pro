#-------------------------------------------------
#
# Project created by QtCreator 2015-10-25T20:37:09
#
#-------------------------------------------------

QT += core gui multimedia multimediawidgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleCamViewer
TEMPLATE = app

mac* | linux*{
    CONFIG(release, debug|release):CONFIG += Release
    CONFIG(debug, debug|release):CONFIG += Debug
}

Release:OUTPUT_DIR=release
Debug:OUTPUT_DIR=debug

DESTDIR  = ../bin/$${OUTPUT_DIR}
OBJECTS_DIR = ../build/$${OUTPUT_DIR}
MOC_DIR     = ../build/$${OUTPUT_DIR}
UI_DIR      = ../build/$${OUTPUT_DIR}
RCC_DIR     = ../build/$${OUTPUT_DIR}

INCLUDEPATH += \
    ../qtutils/ \
    ../cpputils/ \
    ../github-releases-autoupdater/src/


SOURCES += \
    main.cpp\
    cmainwindow.cpp \
    settings/csettingspagegeneral.cpp \
    settings/csettingspagecamera.cpp \
    ccameraslist.cpp \
    updater/cupdaterdialog.cpp

HEADERS += \
    cmainwindow.h \
    settings/csettingspagegeneral.h \
    settings/csettingspagecamera.h \
    settings/settings.h \
    ccameraslist.h \
    updater/cupdaterdialog.h \
    version.h

FORMS += \
    cmainwindow.ui \
    settings/csettingspagegeneral.ui \
    settings/csettingspagecamera.ui \
    ccameraslist.ui \
    updater/cupdaterdialog.ui

RESOURCES += \
    resources/resources.qrc

win*{
    RC_FILE = resources/simplecamviewer.rc

    QMAKE_CXXFLAGS += /MP /wd4251
    QMAKE_CXXFLAGS_WARN_ON = /W4
    DEFINES += WIN32_LEAN_AND_MEAN NOMINMAX
}

LIBS += -L../bin/$${OUTPUT_DIR} -lqtutils -lcpputils -lautoupdater
