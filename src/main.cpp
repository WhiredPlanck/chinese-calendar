/*
 * This file is part of the chinese-calendar project
 * Copyright (C) 2013 National University of Defense Technology(NUDT) & Kylin Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This project is developed by "Ubuntu Kylin Members" team,
 * see <https://launchpad.net/~ubuntukylin~members>
 * Authors: Abel Liu <guiltyritter@gmail.com>
 *                 Shine Huang <hostc@163.com>
 *
 * Modified @ 2017.10.30 14:09:58 CST
 * Jinlj <jincsu@126.com>
 */

#include <iostream>
#include <QtGui>
#include <QCoreApplication>
#include <QTranslator>
#include <QTextCodec>
//#include <QDesktopWidget>
#include <QScreen>
#include <QtSingleApplication>
#include "chinesecalendar.h"

int main(int argc, char *argv[])
{
    //printf("First, you should copy database to `/usr/share/chinese-calendar/db/KylinLunar.db`\n\n");
    std::cout << "First, you should copy database to `/usr/share/chinese-calendar/db/KylinLunar.db`" << std::endl << std::endl;
    //单程序运行处理
    QtSingleApplication app(argc, argv);
    if (app.isRunning())
        return 0;

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//    QApplication app(argc, argv);
#endif

    QTranslator translator;
    QTranslator translatorMenu;

    translator.load(":/data/translate/i18_zh_CN.qm");
    translatorMenu.load(":/data/translate/qt_zh_CN.qm");
    app.installTranslator(&translator);
    app.installTranslator(&translatorMenu);

// load default skin
#ifdef DEBUG
    QFile file(QCoreApplication::applicationDirPath() + "/skin/blue-skin.qss");
#else
    QFile file("/usr/share/chinese-calendar/skin/blue-skin.qss");
#endif
    file.open(QFile::ReadOnly);
    QString stylesheet = QObject::tr(file.readAll());
    app.setStyleSheet(stylesheet);

    app.setQuitOnLastWindowClosed(false);

    ChineseCalendar *window = new ChineseCalendar;
    app.setWindowIcon(window->icon64);
    window->setWindowFlags(Qt::FramelessWindowHint);
    window->resetDateItemColor();

    //QDesktopWidget desktop;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableVirtualGeometry();
    int width = screenRect.width();
    //int height = desktop.screens().size();
    //int width = desktop.screenGeometry().width();
    //int height = desktop.screenGeometry().height();

    window->move(width - window->width() - 5, 30);
    window->hide();
    return app.exec();
}
