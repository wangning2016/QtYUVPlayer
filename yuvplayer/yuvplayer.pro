QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CCFLAG += -lpthread


INCLUDEPATH+=/usr/local/include



LIBS += -L/usr/local/lib -lSDL2

QMAKE_CXXFLAGS += -Wno-unused-function
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-old-style-cast

DESTDIR = ./bin/
MOC_DIR = ./temp/moc
RCC_DIR = ./temp/rcc
UI_DIR = ./temp/ui
OBJECTS_DIR = ./temp/obj
CONFIG += plugin
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cfiletool.cpp \
    cyuvplay.cpp \
    dlgparamsetting.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cfiletool.h \
    comdefine.h \
    cyuvplay.h \
    dlgparamsetting.h \
    mainwindow.h

FORMS += \
    dlgparamsetting.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
