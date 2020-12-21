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

#include "ccbo.h"

CCBO::CCBO(void){}
CCBO::~CCBO(void){}

int CCBO::ctcl_calendar_type(int y, int m, int d, int opt)
{
  int days_of_month[13]={0,31,28,31,30,31,30,31,30,30,31,30,31};
  if(opt==1){
    if(y>1582 || (y==1582 && m>10) || (y==1582 && m==10 && d>14) ){
      if( (y%400==0) || (y%4==0 && y%100!=0) )days_of_month[2]++;
      if(m>0 && m<=12 && d>0 && d<=days_of_month[m])
    return(1);
      else
    return(-1);
    }else if(y==1582 && m==10 && d>=5 && d<=14){
      return(-1);
    }else{
      if(y%4==0)days_of_month[2]++;
      if(m>0 && m<=12 && d>0 && d<=days_of_month[m])
    return(0);
      else
    return(-1);
    }
  }

  if(opt==2)
    return(1);
  return(0);
}

int CCBO::ctcl_date_to_days(int y,int m,int d)
{
  int days=0,i;
  int type=ctcl_calendar_type(y,m,d,1);
  int dm[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};//Days of each month
  if(type!=0){
    if( (y%100!=0 && y%4==0) || (y%400==0) )
      dm[2]+=1;
  }else{
    if(y%4==0)
      dm[2]+=1;
  }
  for(i=0;i<=m-1;i++){
    days+=dm[i];
  }
  days+=d;
  if(y==1582){
    if(type==1)
      days-=10;
    if(type==-1)
      days=-1;
  }
  return days;
}

int CCBO::ctcl_days_to_date(int y,int x)
{
  int m=1;
  int i;
  for(i=1;i<=12;i++){
    int d=ctcl_date_to_days(y,i+1,1)-ctcl_date_to_days(y,i,1);
    if(x<=d || i==12){
      m=i;
      break;
    }else{
       x-=d;
    }
  }
  return 100*m+x;
}

int CCBO::days_from_era(int y)
{
  int days=(y-1)*365 + (y-1)/4;
  if(y>1582)
    days+= -((y-1)/100) + (y-1)/400;
  return days;
}

int CCBO::ctcl_standard_days(int y,int m,int d)
{
  int days=(y-1)*365 + (y-1)/4 + ctcl_date_to_days(y,m,d) - 2;
  if(y>1582)
    days+= -((y-1)/100) + ((y-1)/400) + 2;
  return days;
}

double CCBO::ctcl_julian_day(int y,int m,int d,int h,int min,int sec,int zone)
{
  int type=ctcl_calendar_type(y,m,d,1);
  double jt=(h+(min+sec/60.0)/60.0)/24.0-0.5-zone/24.0;
  double jd=(type!=0) ? (ctcl_standard_days(y,m,d)+1721425+jt) :
    (ctcl_standard_days(y,m,d)+1721425+jt);
  return jd;
}

int CCBO::ctcl_day_of_week(int y,int m,int d)
{
  return ctcl_standard_days(y,m,d)%7;
}

double CCBO::ctcl_solar_term(int y,int n,int t)
{
  double jd = y * (365.2423112 - 6.4e-14*(y-100)*(y-100) - 3.047e-8*(y-100))
    + 15.218427 * n + 1721050.71301;
  double zeta = 3.0e-4 * y - 0.372781384 - 0.2617913325 * n;
  double yd = ( 1.945 * sin(zeta) - 0.01206 * sin(2*zeta) )
    * ( 1.048994 - 2.583e-5 *y );
  double sd =-18e-4*sin(2.313908653*y-0.439822951-3.0443*n);
  return (t==1)?(jd+yd+sd-ctcl_standard_days(y,1,0)-1721425)
    :(jd-ctcl_standard_days(y,1,0)-1721425);
}

double CCBO::ctcl_tail(double x)
{
  return x-floor(x);
}

double CCBO::ctcl_rem(double x,double w)
{
  return ctcl_tail(x/w)*w;
}

double CCBO::ctcl_round(double x){
  return floor(x+0.5);
}

int CCBO::ctcl_gan(int x)
{
  return x%10;
}

int CCBO::ctcl_zhi(int x)
{
  return x%12;
}

int CCBO::ctcl_year_ganzhi(int y,int m,int d,int h)
{
  if ( (ctcl_date_to_days(y,m,d)+h/24.0)
       < ctcl_solar_term(y,3,1)-1.0 )
    y-=1;
  return ctcl_round( ctcl_rem(y-3.0,60.0) );
}

double CCBO::ang(double x,double t,double c1,double t0,double t2,double t3)
{
  return ctcl_tail(c1*x)*2*M_PI+t0-t2*t*t-t3*t*t*t;
}

int CCBO::ctcl_lunar_date(int y,int m,int d)
{
  int lunar_date=-1;
  const double rpi=180/M_PI;
  const double zone=8.0;
  double t=(y-1899.5)/100.0;
  double ms=floor((y-1900)*12.3685);
  double f0=ang(ms,t,0,0.75933,2.172e-4,1.55e-7)
    +0.53058868*ms-8.37e-4*t+zone/24.0+0.5;
  double fc=0.1734-3.93e-4*t;
  double j0=693595+29*ms;
  double aa0=ang(ms,t,0.08084821133,359.2242/rpi,
         0.0000333/rpi,0.00000347/rpi);
  double ab0=ang(ms,t,7.171366127999999e-2,306.0253/rpi,
         -0.0107306/rpi,-0.00001236/rpi);
  double ac0=ang(ms,t,0.08519585128,21.2964/rpi,
         0.0016528/rpi,0.00000239/rpi);
  int i;
  for(i=-1;i<=13;i+=1){
    double aa=aa0+0.507984293*i;
    double ab=ab0+6.73377553*i;
    double ac=ac0+6.818486628*i;
    double f1=f0+1.53058868*i+fc*sin(aa)-0.4068*sin(ab)+0.0021*sin(2*aa)
      +0.0161*sin(2*ab)+0.0104*sin(2*ac)-0.0074*sin(aa-ab)-0.0051*sin(aa+ab);
    double j=j0+28*i+f1;
    int diff=ctcl_standard_days(y,m,d)-floor(j);
    if( diff>=0 && diff<=29 )
      lunar_date=diff+1;
  }
  return(lunar_date);
}

int CCBO::ctcl_leap_month(int y)
{
    char leap=lunarstr.mid(y-beginning_year,2).at(0);
    if(leap=='a')
        leap='0'+10;
    if(leap=='b')
        leap='0'+11;
    if(leap=='c')
        leap='0'+12;
    return leap-'0';
}

int CCBO::ctcl_lunar_month(int y,int m,int d)
{
  double lunar_date=ctcl_lunar_date(y,m,d);
  int lunar_days=lunar_date-floor(lunar_date/100)*100;
  int leap_num=0;
  int i;
  int non_leap;
  int lunar_month;

  for(i=-849;i<=y;i++){
    if(ctcl_leap_month(i)!=0)
      leap_num++;
  }

  non_leap=ctcl_round((ctcl_standard_days(y,m,d)
          -ctcl_standard_days(-849,1,21)
          -lunar_days)/29.530588)-leap_num;

  if(y<=240) non_leap++;
  if(y<=237) non_leap--;
  if(y<24) non_leap++;
  if(y<9) non_leap--;
  if(y<=-255) non_leap++;
  if(y<=-256) non_leap+=2;
  if(y<=-722) non_leap++;

  lunar_month=ctcl_round(ctcl_rem(non_leap-3.0,12.0)+1.0);
  if(lunar_month==ctcl_leap_month(y-1) && m==1 && d<lunar_days)
    lunar_month*=-1;
  else if(lunar_month==ctcl_leap_month(y))
    lunar_month*=-1;
  else if(lunar_month<ctcl_leap_month(y) || ( m<lunar_month && ctcl_leap_month(y) ) )
    lunar_month++;
  else lunar_month=ctcl_round(ctcl_rem(lunar_month-1,12)+1);

  return lunar_month;
}
QString CCBO::ctcl_displaydata(int y, int m, int d)
{
    QString QueryStr;
    QString data;
    QueryStr=QString("select note from cnote where y=%1 and m=%2 and d=%3").arg(y).arg(m).arg(d);
    data=ctcl_common_query("cnote",QueryStr);
    return data;
}

int CCBO::ctcl_savenote(int  y,int m, int d ,QString textedit)
{
    QString QueryStr;
    QString InsertStr;
    QString UpdateStr;
    QString note;
    QString insert;
    QueryStr=QString("select note from cnote where y=%1 and m=%2 and d=%3").arg(y).arg(m).arg(d);
    note=ctcl_common_query("cnote",QueryStr);
    if(textedit.isEmpty()&&note.isEmpty())
        return 0;
    if(note.isEmpty())
    {   InsertStr=QString("INSERT INTO cnote(y,m,d,note) VALUES(%1,%2,%3,'%4');").arg(y).arg(m).arg(d).arg(textedit);
        ctcl_common_query("cnote",InsertStr);
    }else{
        UpdateStr=QString("UPDATE cnote SET note = '%4' where y=%1 and m=%2 and d=%3").arg(y).arg(m).arg(d).arg(textedit);
        ctcl_common_query("cnote",UpdateStr);
   }
}

int CCBO::ctcl_solar_to_lunar(int y,int m,int d,struct CCalendar* ctc)
{
  int r,i;
  char buf[10];
       ::qsnprintf(buf, 10, "%02d%02d", m,d);

#if QT_VERSION < 0x050000
  QString MD = QString::fromAscii(buf);
#else
  QString MD = QString::fromLatin1(buf);
#endif

  QString QueryStr;

  QueryStr=QString("select Ji from yiji where Year=%1 and Month=%2 and Day=%3").arg(y).arg(m).arg(d);
  ctc->ji=ctcl_common_query("yiji",QueryStr);
  QueryStr=QString("select Yi from yiji where Year=%1 and Month=%2 and Day=%3").arg(y).arg(m).arg(d);
  ctc->yi=ctcl_common_query("yiji",QueryStr);


  QueryStr=QString("select VALUE from holiday where DATA='"+MD+"'");
  ctc->holiday=ctcl_common_query("holiday",QueryStr);

  int type=ctcl_calendar_type(y,m,d,1);
  QueryStr=QString("select VALUE from caltype where ID=%1").arg(type);
  ctc->caltype=ctcl_common_query("caltype",QueryStr);

  if(type==-1)return -1;

  QueryStr=QString("select VALUE from weekday where ID=%1").arg(ctcl_day_of_week(y,m,d));
  ctc->weekday=ctcl_common_query("weekday",QueryStr);

  QueryStr=QString("select VALUE from zodiac where DATEB<=%1 and DATEE>=%2").arg(MD.toInt()).arg(MD.toInt());

  ctc->zodiac=ctcl_common_query("zodiac",QueryStr);

  if(ctc->zodiac.isEmpty())
  {
      QueryStr=QString("select VALUE from zodiac where DATEB<=%1 and %2<=1231").arg(MD.toInt()).arg(MD.toInt());
      ctc->zodiac=ctcl_common_query("zodiac",QueryStr);
      if(ctc->zodiac.isEmpty())
      {
          QueryStr=QString("select VALUE from zodiac where DATEB>=101 and  DATEE<=119");
          ctc->zodiac=ctcl_common_query("zodiac",QueryStr);
      }
  }
  ctc->gan=ctcl_gan(ctcl_year_ganzhi(y,m,d,12));
  ctc->zhi=ctcl_zhi(ctcl_year_ganzhi(y,m,d,12));


  QueryStr=QString("select VALUE from tiangan where ID=%1").arg(ctc->gan);
  ctc->ganzhi=ctcl_common_query("gan",QueryStr);
  QueryStr=QString("select VALUE from dizhi where ID=%1").arg(ctc->zhi);
  ctc->ganzhi.append(ctcl_common_query("zhi",QueryStr));

  QueryStr=QString("select VALUE from shengxiao where ID=%1").arg(ctc->zhi);
  ctc->shengxiao=ctcl_common_query("shengxiao",QueryStr);

  for(i=0;i<24;i++){
    r=ctcl_days_to_date(y,ctcl_solar_term(y,i+1,1));
    if(r>=m*100+d){
       QueryStr=QString("select VALUE from sterm where ID=%1").arg(i);
       ctc->sterm=ctcl_common_query("sterm",QueryStr);
     if(r!=m*100+d){
        ctc->sterm.insert(0,QString("%1%2").arg(r/10%10).arg(r%10)+QString::fromUtf8("\346\227\245"));
      }
     ctc->laststerm = false;
      break;
    }else if(i==23){
      QueryStr=QString("select VALUE from sterm where ID=0");
      ctc->sterm=ctcl_common_query("sterm",QueryStr);
      r=ctcl_days_to_date(y+1,ctcl_solar_term(y+1,1,1));
      ctc->laststerm = true;
    }
  }

  if(y>=maxyear){
    ctc->day=0;
    ctc->month=0;
    ctc->cday=tr("unknown");
    ctc->cmonth=tr("no data");
    return 0;
  }

 ctc->day=ctcl_lunar_date(y,m,d);
  if(ctc->day<=10){
   QueryStr=QString("select VALUE from cnumber where ID=%1").arg(ctc->day);
   ctc->cday=QString::fromUtf8("\345\210\235")+ctcl_common_query("day",QueryStr);
  }else if(ctc->day<20){
   QueryStr=QString("select VALUE from cnumber where ID=%1").arg(ctc->day%10);
   ctc->cday=QString::fromUtf8("\345\215\201")+ctcl_common_query("day",QueryStr);
  }else if(ctc->day==20){
    ctc->cday=QString::fromUtf8("\344\272\214\345\215\201");
  }else if(ctc->day<30){
  QueryStr=QString("select VALUE from cnumber where ID=%1").arg(ctc->day%10);
  ctc->cday=QString::fromUtf8("\345\273\277")+ctcl_common_query("day",QueryStr);
  }else if(ctc->day==30){
    ctc->cday=QString::fromUtf8("\344\270\211\345\215\201");
  }

  ctc->month=ctcl_lunar_month(y,m,d);
  if(ctc->month==-12){
    ctc->cmonth=QString::fromUtf8("\351\227\260\345\215\201\344\272\214\346\234\210");
  }else if(ctc->month==-11){
    ctc->cmonth=QString::fromUtf8("\351\227\260\345\215\201\344\270\200\346\234\210");
  }else if(ctc->month==-1){
    ctc->cmonth=QString::fromUtf8("\351\227\260\346\255\243\346\234\210");
  }else if(ctc->month<0){
    ctc->cmonth=QString::fromUtf8("\351\227\260");
    QueryStr=QString("select VALUE from cnumber where ID=%1").arg(-ctc->month);
    ctc->cmonth.append(ctcl_common_query("month",QueryStr));
    ctc->cmonth.append(QString::fromUtf8("\346\234\210"));
  }else if(ctc->month==13){
    ctc->cmonth=QString::fromUtf8("\346\255\243\346\234\210");
  }else{
    QueryStr=QString("select VALUE from cnumber where ID=%1").arg(ctc->month);
    ctc->cmonth=ctcl_common_query("month",QueryStr);
    ctc->cmonth.append(QString::fromUtf8("\346\234\210"));
  }
  memset(buf,0x00,sizeof(buf));
  if(qAbs(ctc->month)<13)
   ::qsnprintf(buf, 10, "%02d%02d", qAbs(ctc->month),ctc->day);
  else if(qAbs(ctc->month)==13)
   ::qsnprintf(buf, 10, "%02d%02d", 1,ctc->day);

#if QT_VERSION < 0x050000
  MD = QString::fromAscii(buf);
#else
  MD = QString::fromLatin1(buf);
#endif

   if(ctc->month>0){
    QueryStr=QString("select VALUE from choliday where DATA='"+MD+"'");
    ctc->choliday=ctcl_common_query("choliday",QueryStr);
    //logSqlError("choliday",QueryStr);
    }
   if(ctc->month>0&&!(ctc->choliday.isEmpty()))
       ctc->ischoliday=true;
   else
       ctc->ischoliday=false;
  return 1;
}

bool CCBO::InitConnection(QString Path)
{
    QString QueryStr;
    QString systempath,systemdirectory;
    QStringList env = QProcess::systemEnvironment().filter(QRegExp("^XDG_DATA_HOME="));
    if (env.length() == 0) {
        systemdirectory = QDir::homePath() + "/.local/share";
    } else {
        systemdirectory = env.at(0);
        systemdirectory = systemdirectory.mid(systemdirectory.indexOf("=") + 1);
        if (systemdirectory.isEmpty()) {
            systemdirectory = QDir::homePath() + "/.local/share";
        }
    }

    systemdirectory = QString("%1/chinese-calendar").arg(systemdirectory);
    systempath = QString("%1/KylinLunar.db").arg(systemdirectory);

    if(!QFile::exists(systempath))
    {
        QDir d;
        if (d.mkpath(systemdirectory) == false) {
            return false;
        }
        if (QFile::copy("/usr/share/chinese-calendar/db/KylinLunar.db", systempath) == false) {
            return false;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(systempath);

    if (!db.open()) {
            return false;
    }

    QSqlQuery query;
    query.exec("select * from lunar where MaxYear=2100");
    query.last();
    maxyear=query.value(0).toInt();
    lunarstr=query.value(1).toByteArray ();

    return true;
}

void CCBO::logSqlError(const QString& Type, const QString& error)
{
    QFile f("chinesecalendar.log" );
    if ( !f.open(QIODevice::ReadWrite | QIODevice::Append ) )
        return;
    QTextStream t( &f );
    t << "Error time: " << QDateTime::currentDateTime().toString() << "\n";
    t << Type << "\n";
    t << error << "\n";
    f.close();
}

QString CCBO::ctcl_common_query(QString type,QString querystr)
{
    QSqlQuery query;
    query.prepare(querystr);
    if(!query.exec())
      logSqlError(type,query.lastError().text());
    if (query.last()) {
        return query.value(0).toString();
    } else {
        return QString();
    }
}

