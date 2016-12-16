#include "logindialog.h"
#include "ui_logindialog.h"
#include <QtNetwork>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include "common.h"

int LoginDialog::realLogin(QString usr, QString passwd, QString serverIP, int serverPort)
{
    QTcpSocket client(this);
    client.connectToHost(QHostAddress(serverIP), serverPort);
    client.write((usr+"_"+passwd).toLocal8Bit().data());
    if(client.waitForBytesWritten(1000)==false){
        return 1;
    }
    QString response;
    if(client.waitForReadyRead(1000) == false){
        return 1;
    }
    QTime t;
    t.start();
    while (t.elapsed() < 1000) {
        response += client.readAll();
        if(response == "lol"){
            // success
            return 0;
        }
        else if(response == "Please send the correct message."){
            // wrong user name or password
            return 2;
        }
    }
    // out of time
    return 1;
}

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->lineEdit_sip->setText(serverIP);
    ui->lineEdit_sport->setText(QString::number(serverPort));
    connect(ui->pushButton_login, &QPushButton::clicked, [=](){
       serverIP = ui->lineEdit_sip->text();
       serverPort = ui->lineEdit_sport->text().toInt();
       int result = realLogin(ui->lineEdit_usr->text(),ui->lineEdit_pwd->text(),serverIP,serverPort);
       if(!result){
           this->accept();
       }
       else{
           QString ErrMsg;
           switch(result){
               case 1:
                    ErrMsg = "connect to server time out";
                    break;
               case 2:
                    ErrMsg = "wrong user name or password";
                    break;
               case 3:
                    ErrMsg = "unknown error";
                    break;
               default:
                    ErrMsg = "unknown error";
                    break;
           }
           QMessageBox::warning(this,"message",ErrMsg,QMessageBox::Yes);
           this->reject();
           this->close();
       }
    });
    connect(ui->pushButton_exit,&QPushButton::clicked,[=](){exit(0);});
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
