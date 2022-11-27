#include "cnote.h"
#include <QtGui>

Cnote::Cnote(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);
    //pix.load(":/img/note.png",0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
//    resize(224,252);
    resize(196,206);
    textedit = new QTextEdit(this); textedit->setObjectName("textedit");
    ok = new QPushButton(this); ok->setObjectName("ok");
    cancel = new QPushButton(this);cancel->setObjectName("cancel");
    datelabel = new QLabel(this);datelabel->setObjectName("datelabel");
    clearplan = new QPushButton(this);clearplan->setObjectName("clearplan");
    textedit->resize(186,140);
    textedit->move(5,35);
    ok->resize(50,20);
    ok->move(140,180);
    ok->setText("保存");
    cancel->resize(50,20);
    cancel->move(89,180);
    cancel->setText("取消");
    clearplan->resize(50,20);
    clearplan->setText("清空");
    clearplan->move(6,180);
    datelabel->resize(130,20);
    datelabel->move(33,5);
    ok->setFocusPolicy(Qt::NoFocus);
    cancel->setFocusPolicy(Qt::NoFocus);
    clearplan->setFocusPolicy(Qt::NoFocus);

    headerw = new QWidget(this);
    headerw->setGeometry(0,0,196,30);
    headerw->setStyleSheet("QWidget{background-color:#0F84BC;}");

    ok->raise();
    cancel->raise();
    clearplan->raise();
    datelabel->raise();
    //ok->setStyleSheet("QPushButton{background-color:transparent;}");
    //cancel->setStyleSheet("QPushButton{background-color:transparent;border:0 solid}");
    //datelabel->setStyleSheet("QLabel{background-color:transparent;border:0 solid;}");
    //clearplan->setStyleSheet("QPushButton{background-color:transparent;}");
    //ok->setIcon(QIcon(":/img/ok.png"));
    //cancel->setIcon(QIcon(":/img/no.png"));
    //textedit->setStyleSheet("QTextEdit{background-color:transparent;border:0 solid;}");
    connect(this->ok,SIGNAL(clicked()),this,SLOT(savenote()));
    connect(this->cancel,SIGNAL(clicked()),this,SLOT(cancelnote()));
    connect(this->clearplan,SIGNAL(clicked()),this,SLOT(clean()));

}

Cnote::~Cnote() {

}

void Cnote::clean() {
    this->textedit->clear();
}

void Cnote::savenote() {
    emit save(this->textedit->toPlainText());

    this->hide();
    this->textedit->clear();
}

void Cnote::cancelnote() {
    this->hide();
    this->textedit->clear();
}

void Cnote::paintEvent(QPaintEvent *) {
//    QPainter painter(this);
    //painter.drawPixmap(0,0,pix.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
//    QStyleOption opt;
//    opt.init(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void  Cnote::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalPos() - dragPos);
    setWindowOpacity(0.5);
}

void  Cnote::mousePressEvent(QMouseEvent *event) {
    dragPos = event->globalPos() - frameGeometry().topLeft();
}

void  Cnote::mouseReleaseEvent(QMouseEvent *event) {
    setWindowOpacity(1.0);
}
