QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += gui \
               data \
               tasks

SOURCES += \
    tasks/alg.cpp \
    controller.cpp \
    data/data.cpp \
    data/models.cpp \
    data/settings.cpp \
    gui/fmattarctorviewer.cpp \
    gui/fmprogressbar.cpp \
    main.cpp \
    gui/fmmainmenu.cpp \
    tasks/maintask.cpp \
    tasks/printtask.cpp

HEADERS += \
    tasks/alg.h \
    data/common.h \
    controller.h \
    data/data.h \
    data/models.h \
    data/settings.h \
    gui/fmattarctorviewer.h \
    gui/fmmainmenu.h \
    gui/fmprogressbar.h \
    tasks/maintask.h \
    tasks/printtask.h

FORMS += \
    gui/fmattarctorviewer.ui \
    gui/fmmainmenu.ui \
    gui/fmprogressbar.ui

TRANSLATIONS += \
    Attractors_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

UI_DIR = .temp/ui
MOC_DIR = .temp/moc
OBJECTS_DIR = .temp/obj

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res_common.qrc
