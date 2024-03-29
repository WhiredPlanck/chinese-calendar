TARGET = chinese-calendar
TEMPLATE = app
DEPENDPATH += $$PWD/
INCLUDEPATH += $$PWD/src
OBJECTS_DIR += build/

CONFIG += debug_and_release

isEmpty(PREFIX) {
    PREFIX = /usr
}

BINDIR = $$PREFIX/bin
DATADIR = $$PREFIX/share
PKGDATADIR = $$DATADIR/chinese-calendar

DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

inst.files += data/KylinLunar.db
inst.path = $$PKGDATADIR/db
inst1.files += data/img/chinesecalendar-64.png
inst1.path = $$DATADIR/pixmaps
inst2.files += data/chinese-calendar.desktop
inst2.path = $$DATADIR/applications
inst3.files += data/chinese-calendar-autostart.desktop
inst3.path = /etc/xdg/autostart
inst4.files += data/chinese-calendar-autostart
inst4.path = $$BINDIR
default_skin.files += data/skin/default/chinesecalendarBGyellow.png \
    data/skin/default/no.png \
    data/skin/default/note.png \
    data/skin/default/ok.png
default_skin.path = $$PKGDATADIR/skin/default
blue_skin.files += data/skin/blue/yijibg.png \
    data/skin/blue/no.png \
    data/skin/blue/note.png \
    data/skin/blue/ok.png \
    data/skin/blue/left-1.png \
    data/skin/blue/left-2.png \
    data/skin/blue/right-1.png \
    data/skin/blue/right-2.png \
        data/skin/blue/normaldate.png \
        data/skin/blue/corner1.png \
        data/skin/blue/corner2.png \
        data/skin/blue/corner3.png \
        data/skin/blue/corner4.png
blue_skin.path = $$PKGDATADIR/skin/blue
black_skin.files += data/skin/black/chinesecalendar.png \
    data/skin/black/no.png \
    data/skin/black/no-hover.png \
    data/skin/black/ok.png \
    data/skin/black/ok-hover.png \
    data/skin/black/note.png \
    data/skin/black/arrow-left.png \
    data/skin/black/arrow-right.png \
    data/skin/black/arrow-down.png
black_skin.path = $$PKGDATADIR/skin/black
qss.files += data/skin/default-skin.qss \
    data/skin/blue-skin.qss \
    data/skin/black.qss
qss.path = $$PKGDATADIR/skin
readme.files += data/skin/README.md
readme.path = $$PKGDATADIR/skin

target.source += $$TARGET
target.path = $$BINDIR

INSTALLS += inst \
    inst1 \
    inst2 \
    inst3 \
    inst4 \
    default_skin \
    blue_skin \
    black_skin \
    qss \
    readme \
    target

#include(src/qtsingleapplication/src/qtsingleapplication.pri)
include(src/singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

QT += core gui\
    sql \
    svg


greaterThan(4, QT_MAJOR_VERSION): QT += widgets


HEADERS += \
    src/dateitem.h \
    src/datemaskwidget.h \
    src/chinesecalendar.h \
    src/ccbo.h \
    src/cnote.h \
    src/aboutdialog.h
FORMS += src/calendar.ui \
    src/dateitem.ui \
    src/aboutdialog.ui
SOURCES += src/main.cpp \
    src/dateitem.cpp \
    src/datemaskwidget.cpp \
    src/chinesecalendar.cpp \
    src/ccbo.cpp \
    src/cnote.cpp \
    src/aboutdialog.cpp
RESOURCES += \
    chinese-calendar.qrc
TRANSLATIONS += data/translate/i18_zh_CN.ts
