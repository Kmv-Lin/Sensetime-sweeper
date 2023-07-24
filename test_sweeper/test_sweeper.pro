#-------------------------------------------------
#
# Project created by QtCreator 2023-03-16T10:44:40
#
#-------------------------------------------------
LIBS     += -L/opt_WQ/WQ_qtenv/Qt-4.8.5/lib/  -lQtqjson
QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sweeper
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-psabi
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    mission_dialog.cpp \
    settingwindow.cpp \
    selfcheck_window.cpp \
    networkdialog.cpp \
    diskspacedialog.cpp \
    volumedialog.cpp \
    systemupdatedialog.cpp \
    missionsetdialog.cpp \
    versiondialog.cpp \
    debugdialog.cpp \
    weiqianfunctions.cpp \
    serialportreadthread.cpp \
    serialport.cpp \
    com.cpp \
    startingwindow.cpp \
    history_mission.cpp \
    com_mission.cpp \
    today_mission.cpp \
    com_setting.cpp

HEADERS  += mainwindow.h \
    mission_dialog.h \
    settingwindow.h \
    selfcheck_window.h \
    networkdialog.h \
    diskspacedialog.h \
    volumedialog.h \
    systemupdatedialog.h \
    missionsetdialog.h \
    versiondialog.h \
    debugdialog.h \
    weiqianfunctions.h \
    serialportreadthread.h \
    serialport.h \
    com.h \
    startingwindow.h \
    history_mission.h \
    com_mission.h \
    today_mission.h \
    com_setting.h

FORMS    += mainwindow.ui \
    mission_dialog.ui \
    settingwindow.ui \
    selfcheck_window.ui \
    networkdialog.ui \
    diskspacedialog.ui \
    volumedialog.ui \
    systemupdatedialog.ui \
    missionsetdialog.ui \
    versiondialog.ui \
    debugdialog.ui \
    startingwindow.ui \
    history_mission.ui \
    today_mission.ui

target.path = /usrfs/panel
INSTALLS += target

include(thread/thread.pri)
include(inputmethod/inputmethod.pri)
include(popwindow/popwindow.pri)

RESOURCES += \
    picture.qrc

DISTFILES +=
