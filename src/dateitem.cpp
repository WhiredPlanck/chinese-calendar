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

#include "dateitem.h"
//extern Cnote *note;

DateItem::DateItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateItem)
{
    ui->setupUi(this);
    QFont font ;
    font.setPointSize(15);
    QFont font1;
    font1.setPointSize(10);
    ui->solarDay->setFont(font);
    ui->lunarDay->setFont(font1);
    ui->noteCorner->hide();

    connect(ui->datemaskwidget, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->datemaskwidget, SIGNAL(clickday()), this, SLOT(clickday()));

    ui->datemaskwidget->installEventFilter(this);

    this->isFocus = false;
//    this->oriItemStyle = this->styleSheet();
//    this->oriDayStyle = ui->label->styleSheet();
//    this->oriCDayStyle = ui->lunarDay->styleSheet();
    this->oriItemStyle = "QWidget{background-image: url('/usr/share/chinese-calendar/skin/blue/normaldate.png');}";
    this->oriDayStyle = "QLabel{font-size:16px;color: #666666;}";
    this->oriCDayStyle = "QLabel{font-size:13px;color: #666666;}";
//    this->clickiItemStyle = "QWidget{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(238, 93, 0, 255), stop:1 rgba(255, 121, 2, 200));}";
//    this->clickDayStyle = "QLabel{color:rgb(255,255,255);}";
//    this->clickCDayStyle = "QLabel{color:rgb(255,255,255);}";
//    this->hoverItemStyle = "QWidget{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(238, 93, 0, 155), stop:1 rgba(255, 121, 2, 100));}";
//    this->hoverDayStyle = "QLabel{color:rgb(255,255,255);}";
//    this->hoverCDayStyle = "QLabel{color:rgb(255,255,255);}";
    this->clickiItemStyle = "QWidget{background-color: #EF6362;}";
    this->clickDayStyle = "QLabel{font-size:18px;color: #FFFFFF;}";
    this->clickCDayStyle = "QLabel{color: #FFFFFF;}";
    this->hoverItemStyle = "QWidget{background-color: #FFD4D4;}";
    this->hoverDayStyle = "QLabel{font-size:18px;color: #666666;}";
    this->hoverCDayStyle = "QLabel{color: #666666;}";
}

DateItem::~DateItem() {
    delete ui;
}

bool DateItem::eventFilter(QObject *o, QEvent *ev) {
    if (o == ui->datemaskwidget) {
        if (ev->type() == QEvent::Enter) {
            this->enter();
        } else if (ev->type() == QEvent::Leave) {
            this->leave();
        } else {
            return QObject::eventFilter(o,ev);
        }
        return true;
    }
}

void DateItem::resizeItem(int x, int y) {
    this->resize(x,y);
    ui->datemaskwidget->resize(x,y);
    if ( y == 50 ){
        ui->solarDay->resize(49,27);
        ui->solarDay->move(0,2);
        ui->lunarDay->resize(49,20);
        ui->lunarDay->move(0,27);
    }
    if ( y == 41 ) {
        ui->solarDay->resize(49,22);
        ui->solarDay->move(0,0);
        ui->lunarDay->resize(49,17);
        ui->lunarDay->move(0,22);
    }
}

void DateItem::enter() {
    if (!ui->solarDay->text().isNull()) {
        if (this->isFocus) {
            this->ui->bg->setStyleSheet(this->clickiItemStyle);
            ui->solarDay->setStyleSheet(this->clickDayStyle);
            ui->lunarDay->setStyleSheet(this->clickCDayStyle);
        } else {
            this->ui->bg->setStyleSheet(this->hoverItemStyle);
            ui->solarDay->setStyleSheet(this->hoverDayStyle);
            ui->lunarDay->setStyleSheet(this->hoverCDayStyle);
        }
    }
}

void DateItem::leave() {
    if (this->isFocus) {
        this->ui->bg->setStyleSheet(this->clickiItemStyle);
        ui->solarDay->setStyleSheet(this->clickDayStyle);
        ui->lunarDay->setStyleSheet(this->clickCDayStyle);
    } else {
        this->ui->bg->setStyleSheet(this->oriItemStyle);
        ui->solarDay->setStyleSheet(this->oriDayStyle);
        ui->lunarDay->setStyleSheet(this->oriCDayStyle);
    }
//    emit resetColor();
}


void DateItem::click() {
    if (!ui->solarDay->text().isNull()) {
        emit clicked(ui->solarDay->text());
        this->ui->bg->setStyleSheet(this->clickiItemStyle);
        ui->solarDay->setStyleSheet(this->clickDayStyle);
        ui->lunarDay->setStyleSheet(this->clickCDayStyle);
        this->isFocus = true;
//        emit resetColor();
    }
}

void DateItem::clickday() {
    if (!ui->solarDay->text().isNull()) {
        emit day(ui->solarDay->text());
//        note->show();
    }
}

void DateItem::setday(int day) {
    ui->solarDay->setText(QString::number(day));
}

void DateItem::setcday(QString cday) {
    ui->lunarDay->setText(cday);
}

void DateItem::setNoteCorner(bool flag) {
    if (flag) {
        ui->noteCorner->show();
    } else {
        ui->noteCorner->hide();
    }
}

QString DateItem::day() {
    return ui->solarDay->text();
}

QString DateItem::cday() {
    return ui->lunarDay->text();
}

void DateItem::clear() {
    ui->solarDay->clear();
    ui->lunarDay->clear();
    ui->noteCorner->hide();
    this->clearstyle();
}

void DateItem::clearstyle() {
    this->ui->bg->setStyleSheet(this->oriItemStyle);
    ui->solarDay->setStyleSheet(this->oriDayStyle);
    ui->lunarDay->setStyleSheet(this->oriCDayStyle);
    this->isFocus = false;
}

void DateItem::setcolor() {
    QPalette pa;
//    pa.setColor(QPalette::WindowText,Qt::red);
//    pa.setColor(QPalette::WindowText,QColor(102, 102, 102));
    pa.setColor(QPalette::WindowText,QColor(215, 25, 24));
    ui->solarDay->setPalette(pa);
    ui->lunarDay->setPalette(pa);
}

void DateItem::setToday() {
    this->ui->bg->setStyleSheet(this->clickiItemStyle);
    ui->solarDay->setStyleSheet(this->clickDayStyle);
    ui->lunarDay->setStyleSheet(this->clickCDayStyle);
    this->isFocus = true;
}
