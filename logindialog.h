#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();


private:
    Ui::LoginDialog *ui;
    int realLogin(QString usr, QString passwd, QString serverIP, int serverPort);
};

#endif // LOGINDIALOG_H
