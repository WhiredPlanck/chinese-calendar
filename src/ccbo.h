/*
 * This file is part of the chinese-calendar project
 * The code reference to "Chinese Traditional Calendar Library"
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
/*
* Chinese Tradional Calendar Library
* Copyright (C) 2002-2003 Wang Yang
* Copyright (C) 2003      donaldzju@zju.edu.cn
*/

#ifndef CCBO_H
#define CCBO_H

#ifdef QT5XX
#include <QtWidgets>
#endif

#include <math.h>
#include <QByteArray>
#include <QFile>
#include <QString>
#include <QObject>
#include <string.h>
#include <QDate>
#include <QFile>
#include <QtGui>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

const int beginning_year=-849;
const int baseIndex = 0;

struct CCalendar
{
    int month;
    int day;
    int gan;
    int zhi;
    QString cmonth;
    QString cday;
    QString ganzhi;
    QString shengxiao;
    QString sterm;
    QString caltype;
    QString weekday;
    QString zodiac;
    QString holiday;
    QString choliday;
    bool laststerm;
    bool ischoliday;
    QString yi;
    QString ji;
    QString note;
};

class CCBO: public QObject
{
public:
    CCBO(void);
    ~CCBO(void);
    int ctcl_solar_to_lunar(int y,int m,int d,struct CCalendar* ctc);
    bool InitConnection(QString Path);
    int ctcl_savenote(int  y,int m, int d ,QString textedit);
    QString ctcl_displaydata(int y,int m,int d);
private:
    QByteArray lunarstr;
    int maxyear;
    int ctcl_calendar_type(int y, int m, int d, int opt);
    int ctcl_date_to_days(int y,int m,int d);
    int ctcl_days_to_date(int y,int x);
    int days_from_era(int y);
    int ctcl_standard_days(int y,int m,int d);
    double ctcl_julian_day(int y,int m,int d,int h,int min,int sec,int zone);
    int ctcl_day_of_week(int y,int m,int d);
    double ctcl_solar_term(int y,int n,int t);
    double ctcl_tail(double x);
    double ctcl_rem(double x,double w);
    double ctcl_round(double x);
    int ctcl_gan(int x);
    int ctcl_zhi(int x);
    int ctcl_year_ganzhi(int y,int m,int d,int h);
    double ang(double x,double t,double c1,double t0,double t2,double t3);
    int ctcl_lunar_date(int y,int m,int d);
    int ctcl_leap_month(int y);
    int ctcl_lunar_month(int y,int m,int d);
    void logSqlError( const QString& Type, const QString& error);
    QString ctcl_common_query(QString type,QString querystr);


};
#endif
