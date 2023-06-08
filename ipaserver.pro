QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

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

INCLUDEPATH += system

SOURCES += \
    appdata.cpp \
    apphome.cpp \
    appmain.cpp \
    filebrower.cpp \
    fileplayer.cpp \
    global.cpp \
    homepage.cpp \
    main.cpp \
    mylistwidget.cpp \
    screensaver.cpp \
    swipeselector.cpp \
    system/gpio.cpp \
    system/system.cpp \
    system/utils.cpp \
    zoneselector.cpp

HEADERS += \
    appdata.h \
    apphome.h \
    appmain.h \
    filebrower.h \
    fileplayer.h \
    global.h \
    homepage.h \
    mylistwidget.h \
    screensaver.h \
    swipeselector.h \
    system/gpio.h \
    system/system.h \
    system/utils.h \
    zoneselector.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
