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
 *                 Shine Huang <shenghuang@ubuntukylin.com>
 */

#include "chinesecalendar.h"
#include <QSettings>

ChineseCalendar::ChineseCalendar(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); m_strFileList.clear();
    icon = QIcon(":/data/img/icon.png");
    icon64 = QIcon(":/data/img/chinesecalendar-64.png");
    mycalendat=new CCBO;
    mycalendat->InitConnection(QDir::currentPath ());
    selectedDate = QDate::currentDate();
	monthCombo->setCurrentIndex(selectedDate.month() - 1);
    YearSelect->setCurrentIndex(selectedDate.year()-1900);
    connect(monthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setMonth(int)));
    connect(monthCombo, SIGNAL(activated(int)), this, SLOT(setMonth(int)));
    connect(YearSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(setYear(int)));
    connect(this->backtotoday, SIGNAL(clicked()), this, SLOT(backtoday()));

    QDateTime dateTime;
    QString currentTime;
    QString week;
    dateTime = QDateTime::currentDateTime();
    if(dateTime.toString("ddd") == "Mon")
        week = "星期一";
    if(dateTime.toString("ddd") == "Tue")
        week = "星期二";
    if(dateTime.toString("ddd") == "Wed")
        week = "星期三";
    if(dateTime.toString("ddd") == "Thu")
        week = "星期四";
    if(dateTime.toString("ddd") == "Fri")
        week = "星期五";
    if(dateTime.toString("ddd") == "Sat")
        week = "星期六";
    if(dateTime.toString("ddd") == "Sun")
        week = "星期日";

    currentTime = dateTime.toString("yyyy-MM-dd");
    QDate day = QDate::currentDate();
    struct CCalendar datebase;
    mycalendat->ctcl_solar_to_lunar(day.year(),day.month(),day.day(),&datebase);

    QString chinesemonth = datebase.cmonth;
    if (chinesemonth == "一月")
        chinesemonth = "正月";
    if (chinesemonth == "十一月")
        chinesemonth = "冬月";
    if (chinesemonth == "十二月")
        chinesemonth = "腊月";

    QString chineseday = QString("农历 %1%2").arg(chinesemonth).arg(datebase.cday);
    QString chineseyear = QString("%1年(%2年)").arg(datebase.ganzhi).arg(datebase.shengxiao);

    QString chineseyi = QString("%1").arg(datebase.yi);
    QStringList chineseyilist = chineseyi.split("、");
    QString chineseji = QString("%2").arg(datebase.ji);
    QStringList chinesejilist = chineseji.split("、");

    QFont font;
    QPalette pa;
    font.setPointSize(48);
    font.setBold(true);
    this->label_10->setFont(font);
    this->label_10->setText(QString::number(day.day()));
    font.setBold(false);
    font.setPointSize(9);
    this->label_12->setFont(font);
    this->label_12->setText(currentTime);
    font.setPointSize(10);
    this->label_13->setFont(font);
    this->label_13->setText(chineseday);
    this->label_14->setFont(font);
    this->label_14->setText(chineseyear);
    this->label_19->setFont(font);
//    font.setPointSize(15);
//    this->label_15->setFont(font);
    font.setPointSize(9);
    font.setFamily("Serif");
    this->label_17->setFont(font);
    this->label_18->setFont(font);

    QString yi=QString("");
    QString ji=QString("");
    if(chineseyilist.count() == 0 )
       yi =QString("");
    if(chineseyilist.count() >= 5)
       yi = QString("").append(chineseyilist.at(0)).append("\n").append(chineseyilist.at(1)).append("\n").append(chineseyilist.at(2)).append("\n").append(chineseyilist.at(3)).append("\n").append(chineseyilist.at(4));
    if(chineseyilist.count() == 4)
       yi = QString("").append(chineseyilist.at(0)).append("\n").append(chineseyilist.at(1)).append("\n").append(chineseyilist.at(2)).append("\n").append(chineseyilist.at(3));
    if (chineseyilist.count() == 3)
       yi = QString("").append(chineseyilist.at(0)).append("\n").append(chineseyilist.at(1)).append("\n").append(chineseyilist.at(2));
    if (chineseyilist.count() == 2)
       yi = QString("").append(chineseyilist.at(0)).append("\n").append(chineseyilist.at(1));
    if (chineseyilist.count() == 1)
       yi = QString("").append(chineseyilist.at(0)).append("\n");
    this->label_17->setText(yi);

    if(chinesejilist.count() == 0 )
       ji =QString("");
    if(chinesejilist.count() >= 5)
       ji = QString("").append(chinesejilist.at(0)).append("\n").append(chinesejilist.at(1)).append("\n").append(chinesejilist.at(2)).append("\n").append(chinesejilist.at(3)).append("\n").append(chinesejilist.at(4));
    if(chinesejilist.count() == 4)
       ji = QString("").append(chinesejilist.at(0)).append("\n").append(chinesejilist.at(1)).append("\n").append(chinesejilist.at(2)).append("\n").append(chinesejilist.at(3));
    if (chinesejilist.count() == 3)
       ji = QString("").append(chinesejilist.at(0)).append("\n").append(chinesejilist.at(1)).append("\n").append(chinesejilist.at(2));
    if (chinesejilist.count() == 2)
       ji = QString("").append(chinesejilist.at(0)).append("\n").append(chinesejilist.at(1));
    if (chinesejilist.count() == 1)
       ji = QString("").append(chinesejilist.at(0)).append("\n");
    this->label_18->setText(ji);

    QString cnote=mycalendat->ctcl_displaydata(day.year(),day.month(),day.day());
    QString haveplan=QString("写便签请双击日期");
    QString noplan=QString("写便签请双击日期");
    int num=cnote.count();
    while ( num > 0 && cnote.at(num-1).isSpace()) {
        num--;
    }
    if(cnote.isEmpty() || num == 0)
        this->label_19->setText(noplan);
    else
        this->label_19->setText(haveplan);

    pa.setColor(QPalette::WindowText,Qt::white);
    this->label->setPalette(pa);
    this->label_2->setPalette(pa);

    pa.setColor(QPalette::WindowText,Qt::black);
    this->label_18->setPalette(pa);

    // some set
    this->TbnextMonth->setFocusPolicy(Qt::NoFocus);
    this->TbnextYear->setFocusPolicy(Qt::NoFocus);
    this->TbpreviousMonth->setFocusPolicy(Qt::NoFocus);
    this->TbpreviousYear->setFocusPolicy(Qt::NoFocus);
    this->backtotoday->setFocusPolicy(Qt::NoFocus);

    // init date item
    map = new QMap<QString, DateItem *>();

    DateItem *dateitem00 = new DateItem(this);
    dateitem00->show();
    map->insert(QString("1-1"), dateitem00 );
    connect(dateitem00, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem00, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem00, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem01 = new DateItem(this);
    dateitem01->show();
    map->insert(QString("1-2"), dateitem01 );
    connect(dateitem01, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem01, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem01, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem02 = new DateItem(this);
    dateitem02->show();
    map->insert(QString("1-3"), dateitem02 );
    connect(dateitem02, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem02, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem02, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem03 = new DateItem(this);
    dateitem03->show();
    map->insert(QString("1-4"), dateitem03 );
    connect(dateitem03, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem03, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem03, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem04 = new DateItem(this);
    dateitem04->show();
    map->insert(QString("1-5"), dateitem04 );
    connect(dateitem04, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem04, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem04, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem05 = new DateItem(this);
    dateitem05->show();
    map->insert(QString("1-6"), dateitem05 );
    connect(dateitem05, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem05, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem05, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem06 = new DateItem(this);
    dateitem06->show();
    map->insert(QString("1-7"), dateitem06 );
    connect(dateitem06, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem06, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem06, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem10 = new DateItem(this);
    dateitem10->show();
    map->insert(QString("2-1"), dateitem10 );
    connect(dateitem10, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem10, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem10, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem11 = new DateItem(this);
    dateitem11->show();
    map->insert(QString("2-2"), dateitem11 );
    connect(dateitem11, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem11, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem11, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem12 = new DateItem(this);
    dateitem12->show();
    map->insert(QString("2-3"), dateitem12 );
    connect(dateitem12, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem12, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem12, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem13 = new DateItem(this);
    dateitem13->show();
    map->insert(QString("2-4"), dateitem13 );
    connect(dateitem13, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem13, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem13, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem14 = new DateItem(this);
    dateitem14->show();
    map->insert(QString("2-5"), dateitem14 );
    connect(dateitem14, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem14, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem14, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem15 = new DateItem(this);
    dateitem15->show();
    map->insert(QString("2-6"), dateitem15 );
    connect(dateitem15, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem15, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem15, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem16 = new DateItem(this);
    dateitem16->show();
    map->insert(QString("2-7"), dateitem16 );
    connect(dateitem16, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem16, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem16, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem20 = new DateItem(this);
    dateitem20->show();
    map->insert(QString("3-1"), dateitem20 );
    connect(dateitem20, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem20, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem20, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem21 = new DateItem(this);
    dateitem21->show();
    map->insert(QString("3-2"), dateitem21 );
    connect(dateitem21, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem21, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem21, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem22 = new DateItem(this);
    dateitem22->show();
    map->insert(QString("3-3"), dateitem22 );
    connect(dateitem22, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem22, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem22, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem23 = new DateItem(this);
    dateitem23->show();
    map->insert(QString("3-4"), dateitem23 );
    connect(dateitem23, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem23, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem23, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem24 = new DateItem(this);
    dateitem24->show();
    map->insert(QString("3-5"), dateitem24 );
    connect(dateitem24, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem24, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem24, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem25 = new DateItem(this);
    dateitem25->show();
    map->insert(QString("3-6"), dateitem25 );
    connect(dateitem25, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem25, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem25, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem26 = new DateItem(this);
    dateitem26->show();
    map->insert(QString("3-7"), dateitem26 );
    connect(dateitem26, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem26, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem26, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem30 = new DateItem(this);
    dateitem30->show();
    map->insert(QString("4-1"), dateitem30 );
    connect(dateitem30, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem30, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem30, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem31 = new DateItem(this);
    dateitem31->show();
    map->insert(QString("4-2"), dateitem31 );
    connect(dateitem31, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem31, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem31, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem32 = new DateItem(this);
    dateitem32->show();
    map->insert(QString("4-3"), dateitem32 );
    connect(dateitem32, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem32, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem32, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem33 = new DateItem(this);
    dateitem33->show();
    map->insert(QString("4-4"), dateitem33 );
    connect(dateitem33, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem33, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem33, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem34 = new DateItem(this);
    dateitem34->show();
    map->insert(QString("4-5"), dateitem34 );
    connect(dateitem34, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem34, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem34, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem35 = new DateItem(this);
    dateitem35->show();
    map->insert(QString("4-6"), dateitem35 );
    connect(dateitem35, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem35, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem35, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem36 = new DateItem(this);
    dateitem36->show();
    map->insert(QString("4-7"), dateitem36 );
    connect(dateitem36, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem36, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem36, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem40 = new DateItem(this);
    dateitem40->show();
    map->insert(QString("5-1"), dateitem40 );
    connect(dateitem40, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem40, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem40, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem41 = new DateItem(this);
    dateitem41->show();
    map->insert(QString("5-2"), dateitem41 );
    connect(dateitem41, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem41, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem41, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem42 = new DateItem(this);
    dateitem42->show();
    map->insert(QString("5-3"), dateitem42 );
    connect(dateitem42, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem42, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem42, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem43 = new DateItem(this);
    dateitem43->show();
    map->insert(QString("5-4"), dateitem43 );
    connect(dateitem43, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem43, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem43, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem44 = new DateItem(this);
    dateitem44->show();
    map->insert(QString("5-5"), dateitem44 );
    connect(dateitem44, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem44, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem44, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem45 = new DateItem(this);
    dateitem45->show();
    map->insert(QString("5-6"), dateitem45 );
    connect(dateitem45, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem45, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem45, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem46 = new DateItem(this);
    dateitem46->show();
    map->insert(QString("5-7"), dateitem46 );
    connect(dateitem46, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem46, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem46, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem50 = new DateItem(this);
    dateitem50->hide();
    map->insert(QString("6-1"), dateitem50 );
    connect(dateitem50, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem50, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem50, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem51 = new DateItem(this);
    dateitem51->hide();
    map->insert(QString("6-2"), dateitem51 );
    connect(dateitem51, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
    connect(dateitem51, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
    connect(dateitem51, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));

    DateItem *dateitem52 = new DateItem(this);
    dateitem52->hide();
    map->insert(QString("6-3"), dateitem52 );

    DateItem *dateitem53 = new DateItem(this);
    dateitem53->hide();
    map->insert(QString("6-4"), dateitem53 );

    DateItem *dateitem54 = new DateItem(this);
    dateitem54->hide();
    map->insert(QString("6-5"), dateitem54 );

    DateItem *dateitem55 = new DateItem(this);
    dateitem55->hide();
    map->insert(QString("6-6"), dateitem55 );

    DateItem *dateitem56 = new DateItem(this);
    dateitem56->hide();
    map->insert(QString("6-7"), dateitem56 );

    setItemLayout();
    setCalendar();

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(setTime()));
    timer->start();

    // mainwindow radius rect
//    QBitmap objBitmap(size());
//    QPainter painter(&objBitmap);
//    painter.fillRect(rect(),Qt::white);
//    painter.setBrush(QColor(0,0,0));
//    painter.drawRoundedRect(this->rect(),10,10);
//    setMask(objBitmap);

    aboutDlg = new AboutDialog();
    aboutDlg->hide();

    this->createTray();

    note = new Cnote;
    //QDesktopWidget desktop;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableVirtualGeometry();
    //int width = desktop.screenGeometry().width();
    int width = screenRect.width();
    note->setGeometry(QRect(width-400,80,196,206));
    connect(note,SIGNAL(save(QString)),this,SLOT(savedata(QString)));
    note->hide();
}

ChineseCalendar::~ChineseCalendar() {
  delete mycalendat;
}

void ChineseCalendar::setclickday(QString day) {
    clickday = day.toInt();
    int year = this->YearSelect->currentText().toInt();
    int month = this->monthCombo->currentText().toInt();
    int today = clickday;
    selectedDate = QDate(year, month, today);
    QString dateplan = QString("%1月%2日计划安排").arg(this->monthCombo->currentText()).arg(day);
    QString data;
    data=mycalendat->ctcl_displaydata(year,month,today);
    note->textedit->setText(data);
    note->datelabel->setText(dateplan);
    note->exec();
}

void ChineseCalendar::savedata(QString data) {
    int year = this->YearSelect->currentText().toInt();
    int month = this->monthCombo->currentText().toInt();
    int day = clickday;
    mycalendat->ctcl_savenote(year,month,day,data);
    note->hide();

    setCalendar();
}


void ChineseCalendar::createTray() {
    this->actionShow = new QAction(tr("隐藏/显示"), this);
    connect(actionShow, SIGNAL(triggered()), this, SLOT(showOrHide()));
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+H"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showOrHide()));

    this->actionAutoStart = new QAction(tr("AutoStart"), this);
    this->actionAutoStart->setCheckable(true);
    connect(actionAutoStart, SIGNAL(triggered(bool)), this, SLOT(setAutoStart(bool)));

    this->actionStaysOnTop = new QAction(tr("总在最前"), this);
    this->actionStaysOnTop->setCheckable(true);
    connect(actionStaysOnTop, SIGNAL(triggered()), this, SLOT(slotChangeMode()));

    this->actionStaysOnBottom = new QAction(tr("总在最后"), this);
    this->actionStaysOnBottom->setCheckable(true);
    connect(actionStaysOnBottom, SIGNAL(triggered()), this, SLOT(slotChangeMode()));

    this->actionStaysNormal = new QAction(tr("普通模式"), this);
    this->actionStaysNormal->setCheckable(true);
    this->actionStaysNormal->setChecked(true);
    connect(actionStaysNormal, SIGNAL(triggered()), this, SLOT(slotChangeMode()));

    // default skin
//    this->actionDefault = new QAction(tr("Default"), this);
//    actionDefault->setCheckable(true);
//    actionDefault->setChecked(true);
//    m_skinName = QString("default");
//    connect(actionDefault, SIGNAL(triggered()), this, SLOT(slotChangeSkin()));

    // blue skin
    this->actionSkin = new QAction(tr("Blue"), this);
    actionSkin->setCheckable(true);
    connect(actionSkin, SIGNAL(triggered()), this, SLOT(slotChangeSkin()));

    this->skinMenu = new QMenu(tr("换肤"), this);
//    this->skinMenu->addAction(actionDefault);
    this->skinMenu->addAction(actionSkin);

    QActionGroup *actSkinGrp = new QActionGroup(this);
//    actSkinGrp->addAction(actionDefault);
    actSkinGrp->addAction(actionSkin);

    readSetting();
//    actionDefault->setChecked(m_skinName == "default");
    actionSkin->setChecked(m_skinName == "blue");

    // read user customizated skin files
    readSkinFiles();

    foreach (QString strSkin, m_strFileList) {
        QAction *actionNew = new QAction(strSkin, this);
        actionNew->setCheckable(true);
        actionNew->setChecked(m_skinName == strSkin);
        actSkinGrp->addAction(actionNew);
        this->skinMenu->addAction(actionNew);
        connect(actionNew, SIGNAL(triggered()), this, SLOT(slotChangeSkin()));
    }

    this->ag = new QActionGroup(this);
    this->ag->addAction(actionStaysOnTop);
    this->ag->addAction(actionStaysOnBottom);
    this->ag->addAction(actionStaysNormal);

    this->stayMenu = new QMenu(tr("Stay Mode"), this);
    this->stayMenu->addAction(actionStaysOnTop);
    this->stayMenu->addAction(actionStaysOnBottom);
    this->stayMenu->addAction(actionStaysNormal);

    this->actionAbout = new QAction(tr("关于本软件"), this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    this->actionQuit = new QAction(tr("E&xit"), this);
    this->actionQuit->setShortcut(tr("Ctrl+Q"));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    this->trayMenu = new QMenu(this);
    this->trayMenu->addAction(actionShow);
    this->trayMenu->addSeparator();
    this->trayMenu->addAction(actionAutoStart);
    this->trayMenu->addSeparator();
    this->trayMenu->addMenu(stayMenu);
    this->trayMenu->addSeparator();
    this->trayMenu->addMenu(skinMenu);
    this->trayMenu->addSeparator();
    this->trayMenu->addAction(actionAbout);
    this->trayMenu->addSeparator();
    this->trayMenu->addAction(actionQuit);

    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(icon);
    this->trayIcon->setContextMenu(trayMenu);
    connect(this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handle_trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    this->trayIcon->show();
}

void ChineseCalendar::paintEvent(QPaintEvent *) {
//    QStyleOption opt;
//    opt.init(this);
//    QPainter painter(this);
//    painter.drawPixmap(0,0,width(),height(),*bg);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

////    painter.setPen(Qt::gray);
//    painter.setPen(QColor(255,121,3,100));

//    if(!isTall)
//    {
//    //row
//        painter.drawLine(5,108,410,108);
//        painter.drawLine(5,147,410,147);
//        painter.drawLine(5,186,410,186);
//        painter.drawLine(5,225,410,225);
//        painter.drawLine(5,264,410,264);
////        painter.drawLine(5,303,410,303);
//    }
//    else
//    {
//        painter.drawLine(5,116,410,116);
//        painter.drawLine(5,163,410,163);
//        painter.drawLine(5,210,410,210);
//        painter.drawLine(5,257,410,257);
////        painter.drawLine(5,304,410,304);
//    }
//    //column
//    painter.drawLine(60,69,60,303);
//    painter.drawLine(115,69,115,303);
//    painter.drawLine(170,69,170,303);
//    painter.drawLine(225,69,225,303);
//    painter.drawLine(280,69,280,303);
//    painter.drawLine(335,69,335,303);
}

void ChineseCalendar::setItemLayout() {
    if(isTall == false) {
        map->value("1-1")->resizeItem(49,50);
        map->value("1-2")->resizeItem(49,50);
        map->value("1-3")->resizeItem(49,50);
        map->value("1-4")->resizeItem(49,50);
        map->value("1-5")->resizeItem(49,50);
        map->value("1-6")->resizeItem(49,50);
        map->value("1-7")->resizeItem(49,50);
        map->value("2-1")->resizeItem(49,50);
        map->value("2-2")->resizeItem(49,50);
        map->value("2-3")->resizeItem(49,50);
        map->value("2-4")->resizeItem(49,50);
        map->value("2-5")->resizeItem(49,50);
        map->value("2-6")->resizeItem(49,50);
        map->value("2-7")->resizeItem(49,50);
        map->value("3-1")->resizeItem(49,50);
        map->value("3-2")->resizeItem(49,50);
        map->value("3-3")->resizeItem(49,50);
        map->value("3-4")->resizeItem(49,50);
        map->value("3-5")->resizeItem(49,50);
        map->value("3-6")->resizeItem(49,50);
        map->value("3-7")->resizeItem(49,50);
        map->value("4-1")->resizeItem(49,50);
        map->value("4-2")->resizeItem(49,50);
        map->value("4-3")->resizeItem(49,50);
        map->value("4-4")->resizeItem(49,50);
        map->value("4-5")->resizeItem(49,50);
        map->value("4-6")->resizeItem(49,50);
        map->value("4-7")->resizeItem(49,50);
        map->value("5-1")->resizeItem(49,50);
        map->value("5-2")->resizeItem(49,50);
        map->value("5-3")->resizeItem(49,50);
        map->value("5-4")->resizeItem(49,50);
        map->value("5-5")->resizeItem(49,50);
        map->value("5-6")->resizeItem(49,50);
        map->value("5-7")->resizeItem(49,50);
        map->value("6-1")->resizeItem(49,50);
        map->value("6-2")->resizeItem(49,50);
        map->value("6-3")->resizeItem(49,50);
        map->value("6-4")->resizeItem(49,50);
        map->value("6-5")->resizeItem(49,50);
        map->value("6-6")->resizeItem(49,50);
        map->value("6-7")->resizeItem(49,50);
        isTall = true;
    }
    map->value("1-1")->move(0,71);
    map->value("1-2")->move(50,71);
    map->value("1-3")->move(100,71);
    map->value("1-4")->move(150,71);
    map->value("1-5")->move(200,71);
    map->value("1-6")->move(250,71);
    map->value("1-7")->move(300,71);

    map->value("2-1")->move(0,122);
    map->value("2-2")->move(50,122);
    map->value("2-3")->move(100,122);
    map->value("2-4")->move(150,122);
    map->value("2-5")->move(200,122);
    map->value("2-6")->move(250,122);
    map->value("2-7")->move(300,122);

    map->value("3-1")->move(0,173);
    map->value("3-2")->move(50,173);
    map->value("3-3")->move(100,173);
    map->value("3-4")->move(150,173);
    map->value("3-5")->move(200,173);
    map->value("3-6")->move(250,173);
    map->value("3-7")->move(300,173);

    map->value("4-1")->move(0,224);
    map->value("4-2")->move(50,224);
    map->value("4-3")->move(100,224);
    map->value("4-4")->move(150,224);
    map->value("4-5")->move(200,224);
    map->value("4-6")->move(250,224);
    map->value("4-7")->move(300,224);

    map->value("5-1")->move(0,275);
    map->value("5-2")->move(50,275);
    map->value("5-3")->move(100,275);
    map->value("5-4")->move(150,275);
    map->value("5-5")->move(200,275);
    map->value("5-6")->move(250,275);
    map->value("5-7")->move(300,275);

    map->value("6-1")->move(0,326);
    map->value("6-2")->move(50,326);
    map->value("6-3")->move(100,326);
    map->value("6-4")->move(150,326);
    map->value("6-5")->move(200,326);
    map->value("6-6")->move(250,326);
    map->value("6-7")->move(300,326);
}

void ChineseCalendar::resetItemLayout() {
    map->value("1-1")->resizeItem(49,41);
    map->value("1-2")->resizeItem(49,41);
    map->value("1-3")->resizeItem(49,41);
    map->value("1-4")->resizeItem(49,41);
    map->value("1-5")->resizeItem(49,41);
    map->value("1-6")->resizeItem(49,41);
    map->value("1-7")->resizeItem(49,41);
    map->value("2-1")->resizeItem(49,41);
    map->value("2-2")->resizeItem(49,41);
    map->value("2-3")->resizeItem(49,41);
    map->value("2-4")->resizeItem(49,41);
    map->value("2-5")->resizeItem(49,41);
    map->value("2-6")->resizeItem(49,41);
    map->value("2-7")->resizeItem(49,41);
    map->value("3-1")->resizeItem(49,41);
    map->value("3-2")->resizeItem(49,41);
    map->value("3-3")->resizeItem(49,41);
    map->value("3-4")->resizeItem(49,41);
    map->value("3-5")->resizeItem(49,41);
    map->value("3-6")->resizeItem(49,41);
    map->value("3-7")->resizeItem(49,41);
    map->value("4-1")->resizeItem(49,41);
    map->value("4-2")->resizeItem(49,41);
    map->value("4-3")->resizeItem(49,41);
    map->value("4-4")->resizeItem(49,41);
    map->value("4-5")->resizeItem(49,41);
    map->value("4-6")->resizeItem(49,41);
    map->value("4-7")->resizeItem(49,41);
    map->value("5-1")->resizeItem(49,41);
    map->value("5-2")->resizeItem(49,41);
    map->value("5-3")->resizeItem(49,41);
    map->value("5-4")->resizeItem(49,41);
    map->value("5-5")->resizeItem(49,41);
    map->value("5-6")->resizeItem(49,41);
    map->value("5-7")->resizeItem(49,41);
    map->value("6-1")->resizeItem(49,41);
    map->value("6-2")->resizeItem(49,41);
    map->value("6-3")->resizeItem(49,41);
    map->value("6-4")->resizeItem(49,41);
    map->value("6-5")->resizeItem(49,41);
    map->value("6-6")->resizeItem(49,41);
    map->value("6-7")->resizeItem(49,41);
    isTall = false;

    map->value("1-1")->move(0,71);
    map->value("1-2")->move(50,71);
    map->value("1-3")->move(100,71);
    map->value("1-4")->move(150,71);
    map->value("1-5")->move(200,71);
    map->value("1-6")->move(250,71);
    map->value("1-7")->move(300,71);

    map->value("2-1")->move(0,113);
    map->value("2-2")->move(50,113);
    map->value("2-3")->move(100,113);
    map->value("2-4")->move(150,113);
    map->value("2-5")->move(200,113);
    map->value("2-6")->move(250,113);
    map->value("2-7")->move(300,113);

    map->value("3-1")->move(0,155);
    map->value("3-2")->move(50,155);
    map->value("3-3")->move(100,155);
    map->value("3-4")->move(150,155);
    map->value("3-5")->move(200,155);
    map->value("3-6")->move(250,155);
    map->value("3-7")->move(300,155);

    map->value("4-1")->move(0,197);
    map->value("4-2")->move(50,197);
    map->value("4-3")->move(100,197);
    map->value("4-4")->move(150,197);
    map->value("4-5")->move(200,197);
    map->value("4-6")->move(250,197);
    map->value("4-7")->move(300,197);

    map->value("5-1")->move(0,239);
    map->value("5-2")->move(50,239);
    map->value("5-3")->move(100,239);
    map->value("5-4")->move(150,239);
    map->value("5-5")->move(200,239);
    map->value("5-6")->move(250,239);
    map->value("5-7")->move(300,239);

    map->value("6-1")->move(0,281);
    map->value("6-2")->move(50,281);
    map->value("6-3")->move(100,281);
    map->value("6-4")->move(150,281);
    map->value("6-5")->move(200,281);
    map->value("6-6")->move(250,281);
    map->value("6-7")->move(300,281);
}

void ChineseCalendar::setTime() {
    QDateTime dateTime = QDateTime::currentDateTime();
    this->label_15->setText(dateTime.toString("hh:mm:ss"));
}

void ChineseCalendar::resetcalendardate(QString day) {
    QDate date(this->YearSelect->currentText().toInt(),this->monthCombo->currentText().toInt(),day.toInt());
    clearbackground();
    selectedDate = date;
    QString weekday;
    if(date.toString("ddd") == "Mon")
        weekday = "星期一";
    if(date.toString("ddd") == "Tue")
        weekday = "星期二";
    if(date.toString("ddd") == "Wed")
        weekday = "星期三";
    if(date.toString("ddd") == "Thu")
        weekday = "星期四";
    if(date.toString("ddd") == "Fri")
        weekday = "星期五";
    if(date.toString("ddd") == "Sat")
        weekday = "星期六";
    if(date.toString("ddd") == "Sun")
        weekday = "星期日";
    QString selecttime = date.toString("yyyy-MM-dd");
    struct CCalendar d;
    mycalendat->ctcl_solar_to_lunar(date.year(),date.month(),date.day(),&d);
    QString selectmonth = d.cmonth;
    if (selectmonth == "一月")
        selectmonth = "正月";
    if (selectmonth == "十一月")
        selectmonth = "冬月";
    if (selectmonth == "十二月")
        selectmonth = "腊月";
    struct CCalendar f = fixshengxiao(date);
    QString selectday = QString("农历 %1%2").arg(selectmonth).arg(d.cday);
    QString selectyear = QString("%1年(%2年)").arg(f.ganzhi).arg(f.shengxiao);
    this->label_10->setText(day);
    this->label_12->setText(selecttime);
    this->label_13->setText(selectday);
    this->label_14->setText(selectyear);

    QString cnote=QString("");
    QString haveplan=QString("写便签请双击日期");
    QString noplan=QString("写便签请双击日期");
    cnote=mycalendat->ctcl_displaydata(date.year(),date.month(),date.day());
    int num=cnote.count();
    while( num > 0 && cnote.at(num-1).isSpace()) {
        num--;
    }
    if(cnote.isEmpty() || num == 0)
        this->label_19->setText(noplan);
    else
        this->label_19->setText(haveplan);

    QString yi = QString("");
    QString ji = QString("");
    QString selectyi = QString("%1").arg(f.yi);
    QStringList selectyilist = selectyi.split("、");
    if(selectyilist.count() == 0 )
       yi =QString("");
    if(selectyilist.count() >= 5)
       yi = QString("").append(selectyilist.at(0)).append("\n").append(selectyilist.at(1)).append("\n").append(selectyilist.at(2)).append("\n").append(selectyilist.at(3)).append("\n").append(selectyilist.at(4));
    if(selectyilist.count() == 4)
       yi = QString("").append(selectyilist.at(0)).append("\n").append(selectyilist.at(1)).append("\n").append(selectyilist.at(2)).append("\n").append(selectyilist.at(3));
    if (selectyilist.count() == 3)
       yi = QString("").append(selectyilist.at(0)).append("\n").append(selectyilist.at(1)).append("\n").append(selectyilist.at(2));
    if (selectyilist.count() == 2)
       yi = QString("").append(selectyilist.at(0)).append("\n").append(selectyilist.at(1));
    if (selectyilist.count() == 1)
       yi = QString("").append(selectyilist.at(0)).append("\n");

    QString selectji = QString("%2").arg(f.ji);
    QStringList selectjilist = selectji.split("、");
    if(selectjilist.count() == 0 )
       ji =QString("");
    if(selectjilist.count() >= 5)
       ji = QString("").append(selectjilist.at(0)).append("\n").append(selectjilist.at(1)).append("\n").append(selectjilist.at(2)).append("\n").append(selectjilist.at(3)).append("\n").append(selectjilist.at(4));
    if(selectjilist.count() == 4)
       ji = QString("").append(selectjilist.at(0)).append("\n").append(selectjilist.at(1)).append("\n").append(selectjilist.at(2)).append("\n").append(selectjilist.at(3));
    if (selectjilist.count() == 3)
       ji = QString("").append(selectjilist.at(0)).append("\n").append(selectjilist.at(1)).append("\n").append(selectjilist.at(2));
    if (selectjilist.count() == 2)
       ji = QString("").append(selectjilist.at(0)).append("\n").append(selectjilist.at(1));
    if (selectjilist.count() == 1)
       ji = QString("").append(selectjilist.at(0)).append("\n");
    this->label_17->setText(yi);
    this->label_18->setText(ji);
}

CCalendar ChineseCalendar::fixshengxiao(QDate date) {
    QDate fixdate = date.addDays(1);
    struct CCalendar fix;
    mycalendat->ctcl_solar_to_lunar(fixdate.year(),fixdate.month(),fixdate.day(),&fix);
    if (fix.sterm == "立春") {
        fixdate = fixdate.addDays(-2);
        mycalendat->ctcl_solar_to_lunar(fixdate.year(),fixdate.month(),fixdate.day(),&fix);
        return fix;
    } else {
        fixdate = fixdate.addDays(-1);
        mycalendat->ctcl_solar_to_lunar(fixdate.year(),fixdate.month(),fixdate.day(),&fix);
        return fix;
    }
}

void ChineseCalendar::backtoday() {
    QDate today = QDate::currentDate();
    YearSelect->setCurrentIndex(today.year() - 1900);
    monthCombo->setCurrentIndex(today.month() - 1);
    selectedDate = today;
    setCalendar();
}

void ChineseCalendar::setCalendar(bool flag) {
    struct CCalendar c;
    cleardate();
    QDate date(selectedDate.year(), selectedDate.month(), 1);
    while (date.month() == selectedDate.month()) {
        int weekDay = date.dayOfWeek();
        mycalendat->ctcl_solar_to_lunar(selectedDate.year(),selectedDate.month(),date.day(),&c);
        QString mycday;
        mycday=c.cday;
        if (c.sterm.at(0) >=QChar('0') && c.sterm.at(0) <= QChar('9'))
          mycday=c.cday;
        else
        {
          if(c.laststerm)
              mycday=c.cday;
          else
              mycday=c.sterm;
        }
        if (c.ischoliday)
            mycday=c.choliday;
        int day = date.day();
        if (weekDay == 7)
            weekDay = 0;
        int row = (day + (6 - weekDay) + 7 -1)/7;
        QString site = QString("%1-%2").arg(row).arg(weekDay+1);
        map->value(site)->setday(date.day());
        map->value(site)->setcday(mycday);

        QString cnote = mycalendat->ctcl_displaydata(date.year(),date.month(),date.day());
        int num=cnote.count();
        if(cnote.isEmpty() || num == 0)
            map->value(site)->setNoteCorner(false);
        else
            map->value(site)->setNoteCorner(true);

        if(weekDay == 6 || weekDay == 0)
            map->value(site)->setcolor();
         date = date.addDays(1);
    }

    if (!map->value("6-1")->day().isNull()) {
        map->value("6-1")->show();
        map->value("6-2")->show();
        map->value("6-3")->show();
        map->value("6-4")->show();
        map->value("6-5")->show();
        map->value("6-6")->show();
        map->value("6-7")->show();
        resetItemLayout();
    } 

    QDate today = QDate::currentDate();
    if(today.year() == selectedDate.year() && today.month() == selectedDate.month() && today.day() == selectedDate.day()) {
        setDayFocus(today);
    }

    setDayFocus(selectedDate, false); // set the selected date in focus
}

void ChineseCalendar::clearbackground() {
    map->value("1-1")->clearstyle();
    map->value("1-2")->clearstyle();
    map->value("1-3")->clearstyle();
    map->value("1-4")->clearstyle();
    map->value("1-5")->clearstyle();
    map->value("1-6")->clearstyle();
    map->value("1-7")->clearstyle();
    map->value("2-1")->clearstyle();
    map->value("2-2")->clearstyle();
    map->value("2-3")->clearstyle();
    map->value("2-4")->clearstyle();
    map->value("2-5")->clearstyle();
    map->value("2-6")->clearstyle();
    map->value("2-7")->clearstyle();
    map->value("3-1")->clearstyle();
    map->value("3-2")->clearstyle();
    map->value("3-3")->clearstyle();
    map->value("3-4")->clearstyle();
    map->value("3-5")->clearstyle();
    map->value("3-6")->clearstyle();
    map->value("3-7")->clearstyle();
    map->value("4-1")->clearstyle();
    map->value("4-2")->clearstyle();
    map->value("4-3")->clearstyle();
    map->value("4-4")->clearstyle();
    map->value("4-5")->clearstyle();
    map->value("4-6")->clearstyle();
    map->value("4-7")->clearstyle();
    map->value("5-1")->clearstyle();
    map->value("5-2")->clearstyle();
    map->value("5-3")->clearstyle();
    map->value("5-4")->clearstyle();
    map->value("5-5")->clearstyle();
    map->value("5-6")->clearstyle();
    map->value("5-7")->clearstyle();
    map->value("6-1")->clearstyle();
    map->value("6-2")->clearstyle();
    map->value("6-3")->clearstyle();
    map->value("6-4")->clearstyle();
    map->value("6-5")->clearstyle();
    map->value("6-6")->clearstyle();
    map->value("6-7")->clearstyle();
}

void ChineseCalendar::cleardate() {
    map->value("1-1")->clear();
    map->value("1-2")->clear();
    map->value("1-3")->clear();
    map->value("1-4")->clear();
    map->value("1-5")->clear();
    map->value("1-6")->clear();
    map->value("1-7")->clear();
    map->value("2-1")->clear();
    map->value("2-2")->clear();
    map->value("2-3")->clear();
    map->value("2-4")->clear();
    map->value("2-5")->clear();
    map->value("2-6")->clear();
    map->value("2-7")->clear();
    map->value("3-1")->clear();
    map->value("3-2")->clear();
    map->value("3-3")->clear();
    map->value("3-4")->clear();
    map->value("3-5")->clear();
    map->value("3-6")->clear();
    map->value("3-7")->clear();
    map->value("4-1")->clear();
    map->value("4-2")->clear();
    map->value("4-3")->clear();
    map->value("4-4")->clear();
    map->value("4-5")->clear();
    map->value("4-6")->clear();
    map->value("4-7")->clear();
    map->value("5-1")->clear();
    map->value("5-2")->clear();
    map->value("5-3")->clear();
    map->value("5-4")->clear();
    map->value("5-5")->clear();
    map->value("5-6")->clear();
    map->value("5-7")->clear();
    map->value("6-1")->clear();
    map->value("6-2")->clear();
    map->value("6-3")->clear();
    map->value("6-4")->clear();
    map->value("6-5")->clear();
    map->value("6-6")->clear();
    map->value("6-7")->clear();
    map->value("6-1")->hide();
    map->value("6-2")->hide();
    map->value("6-3")->hide();
    map->value("6-4")->hide();
    map->value("6-5")->hide();
    map->value("6-6")->hide();
    map->value("6-7")->hide();
    setItemLayout();
}

void ChineseCalendar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (map->value("6-1")->isHidden()) {
            if (event->x()>5 && event->x()<390 && event->y()>69 && event->y()<304)
                moveable = false;
            else moveable = true;
        } else {
            if (event->x()>5 && event->x()<390 && event->y()>69 && event->y()<304)
                moveable = false ;
            else moveable = true;
        }
        dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }

}

void ChineseCalendar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && moveable == true){
        if (map->value("6-1")->isHidden()) {
            if (event->x()>5 && event->x()<390 && event->y()>69 && event->y()<304)
                return ;
            else move(event->globalPos() - dragPos);
        } else {
            if (event->x()>5 && event->x()<390 && event->y()>69 && event->y()<304)
                return ;
            else move(event->globalPos() - dragPos);
        }
        setWindowOpacity(0.5);
    }
    event->accept();

}

void ChineseCalendar::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setWindowOpacity(1);
    }
    event->accept();
}

void ChineseCalendar::setMonth(int month) {
    int day=0;
    int newmonth = month+1;
    if (newmonth<=0) {
        newmonth = 12;
        monthCombo->setCurrentIndex(11);
    }
    if (newmonth>12) {
        newmonth = 1;
    }
    if (newmonth ==1 || newmonth ==3 || newmonth == 5 || newmonth == 7 || newmonth  == 8 || newmonth == 10 || newmonth ==12) {
        day =31;
    } else if (newmonth ==4 || newmonth==6 || newmonth ==9 || newmonth ==11) {
       day =30;
    }
   if (selectedDate.year()%4 ==0 && selectedDate.year()%100!=0) {
            day = 29;
    } else if (selectedDate.year()%400 ==0) {
            day = 29;
    } else {
            day = 28;
    }
    selectedDate = QDate(selectedDate.year(),newmonth,day);
    setCalendar();
}

void ChineseCalendar::setYear(int year) {
    selectedDate = QDate(year+1900, selectedDate.month(), selectedDate.day());
    setCalendar();
}

void ChineseCalendar::on_TbpreviousYear_clicked() {
    if (YearSelect->currentIndex() != 0)
        YearSelect->setCurrentIndex(YearSelect->currentIndex()-1);
}

void ChineseCalendar::on_TbpreviousMonth_clicked() {
    if (monthCombo->currentIndex() == 0 && YearSelect->currentIndex() != 0) {
        YearSelect->setCurrentIndex(YearSelect->currentIndex()-1);
        monthCombo->setCurrentIndex(11);
    } else {
        monthCombo->setCurrentIndex(monthCombo->currentIndex()-1);
    }
}

void ChineseCalendar::on_TbnextMonth_clicked()
{
    if (monthCombo->currentIndex() == 11 && YearSelect->currentIndex() != 150) {
        YearSelect->setCurrentIndex(YearSelect->currentIndex()+1);
        monthCombo->setCurrentIndex(0);
    } else {
        monthCombo->setCurrentIndex(monthCombo->currentIndex()+1);
    }
}

void ChineseCalendar::on_TbnextYear_clicked()
{
    if(YearSelect->currentIndex() != 150)
        YearSelect->setCurrentIndex(YearSelect->currentIndex()+1);
}

void ChineseCalendar::handle_trayIcon_activated(QSystemTrayIcon::ActivationReason reason) {
    switch(reason) {
        case QSystemTrayIcon::Trigger:
            this->showOrHide();
            break;
        default:
            ;
    }
}

void ChineseCalendar::showOrHide() {
    if(this->isHidden()) {
        this->show();
    } else
        this->hide();
}

void ChineseCalendar::setMode(int nMode, bool bFlag) {
    switch (nMode) {
    case 1: // stays on top
        this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
        break;
    case 2: // stays on bottom
        this->setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        break;
    case 3: // stays normal
        this->setWindowFlags(Qt::FramelessWindowHint);
        break;
    default:
        break;
    }

    if (bFlag)
        this->show();
}

void ChineseCalendar::slotChangeSkin() {
    QAction *action = qobject_cast<QAction *>(sender());
    QString strSkin = action->text();

    if (!strSkin.isEmpty()) {
        if (strSkin.toLower() == "default" || strSkin.toLower() == "blue") {
            strSkin = strSkin.toLower();
            changeSkin(strSkin);
        } else {
            changeSkin(strSkin, false);
        }
    }

    m_skinName = strSkin;

    setCalendar();

    writeSetting();
}

void ChineseCalendar::slotChangeMode() {
    m_nMode = 0;
    if (actionStaysOnTop->isChecked())
        m_nMode = 1;
    if (actionStaysOnBottom->isChecked())
        m_nMode = 2;
    if (actionStaysNormal->isChecked())
        m_nMode = 3;

    setMode(m_nMode);

    writeSetting();
}

void ChineseCalendar::changeSkin(const QString &skin, bool bFlag) {
    QString strFilename("");

    if (bFlag) {
#ifdef DEBUG
        strFilename = QCoreApplication::applicationDirPath() + QString("/skin/%1-skin.qss").arg(skin);
#else
        strFilename = QString("/usr/share/chinese-calendar/skin/%1-skin.qss").arg(skin);
#endif
    } else {
        strFilename =  QDir::homePath() + QString("/.local/share/chinese-calendar/skin/%1.qss").arg(skin);
    }

    QFile qss(strFilename);
    if (qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
}

void ChineseCalendar::readSkinFiles() {
    QString systemdirectory;
    systemdirectory = QDir::homePath() + "/.local/share/chinese-calendar/skin/";

    QDir dir(systemdirectory);
    foreach(QFileInfo info, dir.entryInfoList()) {
        if (info.isFile() && info.suffix() == "qss") {
            m_strFileList << info.baseName();
        }
    }
}

void ChineseCalendar::resetDateItemColor() {
    setCalendar(true);
}

void ChineseCalendar::setDayFocus(QDate &date, bool bFlag) {
    int d = date.day();
    int w = date.dayOfWeek();
    if (w == 7)
        w = 0;
    int r = (d + (6 - w) + 7 -1)/7;

    QString add = QString("%1-%2").arg(r).arg(w+1);
    if (bFlag) {
        resetcalendardate(QString::number(date.day()));
    }

    map->value(add)->setToday();
}

void ChineseCalendar::showAbout() {
    aboutDlg->show();
}

void ChineseCalendar::setAutoStart(bool bFlag) {
    /* 设置是否开机自启动
     * 当该目录下没有desktop文件时开机启动,有则不开机启动
     * 因为系统安装时没有该用户目录,所以第一次是默认开机启动
     */
    if (!bFlag) {
        // 需要将desktop文件拷贝至~/.config/autostart目录下
        QString strDesktop = "/usr/share/applications/chinese-calendar.desktop";
        QString strCmd = "cp " + strDesktop + " ~/.local/share/chinese-calendar";
        const char *cmd = strCmd.toUtf8().constData();
        system(cmd);
    } else {
        QString filename = QDir::homePath() + "/.local/share/chinese-calendar/chinese-calendar.desktop";
        QFile file(filename);

        // if chinese-calendar.desktop is exist, then remove it.
        if (file.exists()) {
            file.remove();
        }
    }

//    writeSetting();
}

bool ChineseCalendar::checkAutoStart() {
    QString filename = QDir::homePath() + "/.local/share/chinese-calendar/chinese-calendar.desktop";
    QFile file(filename);
    if (file.exists()) {
        return false;
    } else {
        return true;
    }
}

void ChineseCalendar::readSetting() {
    QSettings settings("chinese-calendar", "chinese-calendar");

    settings.beginGroup("Config");

    // 读取自启动标志
    bool bAutoStart = this->checkAutoStart();
//    bool bAutoStart = settings.value("autostart", false).toBool();

    // 读取显示模式
    m_nMode = settings.value("mode", 3).toInt();

    // 读取皮肤名
//    m_skinName = settings.value("skin", "default").toString();
    m_skinName = settings.value("skin", "blue").toString();

    settings.endGroup();

    actionAutoStart->setChecked(bAutoStart);

    switch (m_nMode) {
    case 1:
        actionStaysOnTop->setChecked(true);
        actionStaysOnBottom->setChecked(false);
        actionStaysNormal->setChecked(false);
        break;
    case 2:
        actionStaysOnTop->setChecked(false);
        actionStaysOnBottom->setChecked(true);
        actionStaysNormal->setChecked(false);
        break;
    case 3:
        actionStaysOnTop->setChecked(false);
        actionStaysOnBottom->setChecked(false);
        actionStaysNormal->setChecked(true);
        break;
    default:
        ;
    }

    setMode(m_nMode, false);

    if (m_skinName == "default" || m_skinName == "blue") {
        changeSkin(m_skinName);
    } else {
        changeSkin(m_skinName, false);
    }
    setCalendar();
}

void ChineseCalendar::writeSetting() {
    QSettings settings("chinese-calendar", "chinese-calendar");

    settings.beginGroup("Config");
    settings.setValue("autostart", actionAutoStart->isChecked());
    settings.setValue("mode", m_nMode);
    settings.setValue("skin", m_skinName);

    settings.endGroup();
}
