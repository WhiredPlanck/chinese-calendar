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
 */

#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QWidget>
#include <QDate>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QGraphicsItem>
#include <QtGui>
#include <QMap>
#include <QPixmap>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include "ui_calendar.h"
#include "ccbo.h"
#include "dateitem.h"
#include "cnote.h"
#include "aboutdialog.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

class CCBO;
class ChineseCalendar: public QWidget, private Ui::Calendar
{
    Q_OBJECT
public:
    explicit ChineseCalendar(QWidget *parent = 0);
    virtual ~ChineseCalendar();
    QIcon icon, icon64;
    int clickday;

private:
    void setCalendar(bool flag = true);
    void cleardate();
    void clearbackground();
    void createTray();
    void setItemLayout();
    void resetItemLayout();
    CCalendar fixshengxiao(QDate);
    void initDateItem(QString id, bool show, bool connect);
    Cnote *note;

    void paintEvent(QPaintEvent *);

    bool moveable;
    bool isTall;
    QPoint dragPos;
    QDate selectedDate;
    CCBO *mycalendat;
    QMap<QString, DateItem*> *map;

    QPixmap *bg;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QActionGroup *ag;
    QAction *actionShow, *actionStaysOnTop, *actionStaysOnBottom, *actionStaysNormal, *actionQuit;
    QAction *actionDefault;
    QAction *actionSkin;
    QAction *actionAbout;
    QAction *actionAutoStart;
    QMenu *skinMenu;
    QMenu *stayMenu;

public slots:
    void setMonth(int month);
    void setYear(int year);
    void setTime();
    void resetcalendardate(QString);
    void backtoday();
    void handle_trayIcon_activated(QSystemTrayIcon::ActivationReason reason);
    void showOrHide();
    void savedata(QString);
    void setclickday(QString);
    void resetDateItemColor();
    void showAbout();
    void setAutoStart(bool bFlag);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_TbnextYear_clicked();
    void on_TbnextMonth_clicked();
    void on_TbpreviousMonth_clicked();
    void on_TbpreviousYear_clicked();
    void slotChangeSkin();
    void slotChangeMode();
private:
    void setMode(int nMode, bool bFlag = true);
    void changeSkin(const QString &, bool bFlag = true);
    void readSkinFiles();
    void setDayFocus(QDate &date, bool bFlag = true);

    bool checkAutoStart();
    void readSetting();
    void writeSetting();

    QStringList m_strFileList;
    AboutDialog *aboutDlg;
    int m_nMode;
    QString m_skinName;
};

#endif
