/*!
 * Copyright(C) 2013 mutse
 *
 * @file    aboutdialog.h
 * @brief   about Chinese Calendar
 * @author  mutse <yyhoo2.young@gmail.com>
 * @date    Aug 6th, 2013
 */
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

public slots:
    void accept();
    void openUrl(QString url);
    
private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
