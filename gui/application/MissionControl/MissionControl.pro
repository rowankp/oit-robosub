QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Source.cpp \
    about/AboutWindow.cpp \
    help/HelpWindow.cpp \
    main/CancelMissionDialog.cpp \
    main/ConfirmMissionDialog.cpp \
    main/MainWindow.cpp \
    main/MissionInvalidDialog.cpp \
    main/UnsupportedDialog.cpp \
    misc/EmitTextButton.cpp \
    subby/SubbyButton.cpp

HEADERS += \
    about/AboutWindow.h \
    help/HelpWindow.h \
    main/CancelMissionDialog.h \
    main/ConfirmMissionDialog.h \
    main/MainWindow.h \
    main/MissionInvalidDialog.h \
    main/UnsupportedDialog.h \
    misc/EmitTextButton.h \
    subby/SubbyButton.h

FORMS += \
    about/AboutWindow.ui \
    help/HelpWindow.ui \
    main/CancelMissionDialog.ui \
    main/ConfirmMissionDialog.ui \
    main/MainWindow.ui \
    main/MissionInvalidDialog.ui \
    main/UnsupportedDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    about/team.png \
    mission/mission.txt \
    mission/tasks-supported.txt \
    subby/subby-icon.png \
    subby/subby-logo.png \
    subby/subby-puns.txt
