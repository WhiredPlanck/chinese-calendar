/*!
 * Copyright(C) 2013 mutse
 *
 * @file    aboutdialog.cpp
 * @brief   about Chinese Calendar
 * @author  mutse <yyhoo2.young@gmail.com>
 * @date    Aug 6th, 2013
 */
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->lbHomepage, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));
/*
#ifdef DEBUG
    QFile file(QCoreApplication::applicationDirPath() + "src/qss/aboutus.qss");
#else
    QFile file(":src/qss/aboutus.qss");
#endif
    file.open(QFile::ReadOnly);
    QString stylesheet = QObject::tr(file.readAll());
    this->setStyleSheet(stylesheet);
*/
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::accept()
{
    hide();
}

void AboutDialog::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}
