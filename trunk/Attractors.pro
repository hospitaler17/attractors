QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alg.cpp \
    builder.cpp \
    fmprogressbar.cpp \
    main.cpp \
    fmmainmenu.cpp

HEADERS += \
    alg.h \
    builder.h \
    common.h \
    fmmainmenu.h \
    fmprogressbar.h

FORMS += \
    fmmainmenu.ui \
    fmprogressbar.ui

TRANSLATIONS += \
    Attractors_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

UI_DIR = .temp
MOC_DIR = .temp
OBJECTS_DIR = .temp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
