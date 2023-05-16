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

    QString chineseday = QString("农历 %1%2").arg(chinesemonth, datebase.cday);
    QString chineseyear = QString("%1年(%2年)").arg(datebase.ganzhi, datebase.shengxiao);

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

    auto yi = datebase.yi.split("、").mid(0, 5).join("\n");
    auto ji = datebase.ji.split("、").mid(0, 5).join("\n");

    this->label_17->setText(yi);
    this->label_18->setText(ji);

    QString cnote=mycalendat->ctcl_displaydata(day.year(),day.month(),day.day());
    QString haveplan=QString("写便签请双击日期");
    QString noplan=QString("写便签请双击日期");
    auto isCnoteBlank = cnote.isEmpty() || std::all_of(cnote.cbegin(), cnote.cend(), [](const QChar ch) {
                            return ch.isSpace();
                    });
    if(isCnoteBlank)
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

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            auto id = QString("%1-%2").arg(i + 1).arg(j + 1);
            if (i < 5) {
                initDateItem(id, true, true);
            } else {
                if (j < 2) {
                    initDateItem(id, false, true);
                } else {
                    initDateItem(id, false, false);
                }
            }
        }
    }

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
    QString dateplan = QString("%1月%2日计划安排").arg(this->monthCombo->currentText(), day);
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
        for (auto item : qAsConst(*map)) {
            item->resizeItem(49, 50);
        }
        isTall = true;
    }

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            map->value(QString("%1-%2").arg(i + 1).arg(j + 1))->move(j * 50, 71 + i  * 51);
        }
    }
}

void ChineseCalendar::resetItemLayout() {
    for (auto item : qAsConst(*map)) {
        item->resizeItem(49, 41);
    }
    isTall = false;

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            map->value(QString("%1-%2").arg(i + 1).arg(j + 1))->move(j * 50, 71 + i  * 42);
        }
    }
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
    QString selectday = QString("农历 %1%2").arg(selectmonth, d.cday);
    QString selectyear = QString("%1年(%2年)").arg(f.ganzhi, f.shengxiao);
    this->label_10->setText(day);
    this->label_12->setText(selecttime);
    this->label_13->setText(selectday);
    this->label_14->setText(selectyear);

    QString cnote=QString("");
    QString haveplan=QString("写便签请双击日期");
    QString noplan=QString("写便签请双击日期");
    cnote=mycalendat->ctcl_displaydata(date.year(),date.month(),date.day());
    auto isCnoteBlank = cnote.isEmpty() || std::all_of(cnote.cbegin(), cnote.cend(), [](const QChar ch) {
                            return ch.isSpace();
                        });
    if(isCnoteBlank)
        this->label_19->setText(noplan);
    else
        this->label_19->setText(haveplan);

    auto yi = f.yi.split("、").mid(0, 5).join("\n");
    auto ji = f.ji.split("、").mid(0, 5).join("\n");

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

void ChineseCalendar::initDateItem(QString id, bool show, bool connect) {
    auto *dateItem = new DateItem(this);
    show ? dateItem->show() : dateItem->hide();
    map->insert(id, dateItem);
    if (connect) {
        QObject::connect(dateItem, SIGNAL(clicked(QString)), this, SLOT(resetcalendardate(QString)));
        QObject::connect(dateItem, SIGNAL(day(QString)), this, SLOT(setclickday(QString)));
        QObject::connect(dateItem, SIGNAL(resetColor()), this, SLOT(resetDateItemColor()));
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
         map->value(site)->setNoteCorner(!cnote.isEmpty());

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
    for (auto item : qAsConst(*map)) {
        item->clearstyle();
    }
}

void ChineseCalendar::cleardate() {
    for (auto item : qAsConst(*map)) {
        item->clear();
    }
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
    QFile autoStarFile(QDir::homePath() + "/.local/share/chinese-calendar/chinese-calendar.desktop");
    return !autoStarFile.exists();
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

    actionStaysOnTop->setChecked(m_nMode == 1);
    actionStaysOnBottom->setChecked(m_nMode == 2);
    actionStaysNormal->setChecked(m_nMode == 3);

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
