#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QString>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    bool logout();

private:
    Ui::LoginDialog *ui;
    int realLogin(QString usr, QString passwd, QString serverIP, int serverPort);
    QString usrName;
};

#endif // LOGINDIALOG_H
