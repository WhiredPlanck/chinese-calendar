TEMPLATE = app
TARGET +=
DEPENDPATH += $$PWD/
INCLUDEPATH += $$PWD/

inst.files += KylinLunar.db
inst.path = /usr/share/chinese-calendar/db
inst1.files += img/chinesecalendar-64.png
inst1.path = /usr/share/pixmaps
inst2.files += chinese-calendar.desktop
inst2.path = /usr/share/applications
inst3.files += chinese-calendar-autostart.desktop
inst3.path = /etc/xdg/autostart
inst4.files += chinese-calendar-autostart
inst4.path = /usr/bin
default_skin.files += skin/default/chinesecalendarBGyellow.png \
    skin/default/no.png \
    skin/default/note.png \
    skin/default/ok.png
default_skin.path = /usr/share/chinese-calendar/skin/default
blue_skin.files += skin/blue/yijibg.png \
    skin/blue/no.png \
    skin/blue/note.png \
    skin/blue/ok.png \
    skin/blue/left-1.png \
    skin/blue/left-2.png \
    skin/blue/right-1.png \
    skin/blue/right-2.png \
        skin/blue/normaldate.png \
        skin/blue/corner1.png \
        skin/blue/corner2.png \
        skin/blue/corner3.png \
        skin/blue/corner4.png
blue_skin.path = /usr/share/chinese-calendar/skin/blue
black_skin.files += skin/black/chinesecalendar.png \
    skin/black/no.png \
    skin/black/no-hover.png \
    skin/black/ok.png \
    skin/black/ok-hover.png \
    skin/black/note.png \
    skin/black/arrow-left.png \
    skin/black/arrow-right.png \
    skin/black/arrow-down.png
black_skin.path = /usr/share/chinese-calendar/skin/black
qss.files += skin/default-skin.qss \
    skin/blue-skin.qss \
    skin/black.qss
qss.path = /usr/share/chinese-calendar/skin
readme.files += skin/README.md
readme.path = /usr/share/chinese-calendar/skin

target.source += $$TARGET
target.path = /usr/bin
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

include(qtsingleapplication/src/qtsingleapplication.pri)

QT += core gui\
    sql \
    svg


greaterThan(4, QT_MAJOR_VERSION): QT += widgets


HEADERS += \
    dateitem.h \
    datemaskwidget.h \
    chinesecalendar.h \
    ccbo.h \
    cnote.h \
    aboutdialog.h
FORMS += calendar.ui \
    dateitem.ui \
    aboutdialog.ui
SOURCES += main.cpp \
    dateitem.cpp \
    datemaskwidget.cpp \
    chinesecalendar.cpp \
    ccbo.cpp \
    cnote.cpp \
    aboutdialog.cpp
RESOURCES += \
    chinese-calendar.qrc
TRANSLATIONS += translate/i18_zh_CN.ts
