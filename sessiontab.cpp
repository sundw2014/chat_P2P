#include "sessiontab.h"
#include "ui_sessiontab.h"
#include <QVBoxLayout>
#include <QLabel>
#include <common.h>
#include <QPushButton>
#include <sessionworkerthread.h>
#include <selectfriendsdialog.h>
#include <QFileDialog>

sessionTab::sessionTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sessionTab)
{
    ui->setupUi(this);
    ui->label_friendName->setText(QString("Group"));
    sessionContentLayout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(sessionContentLayout);
    connect(ui->pushButton_send,&QPushButton::clicked,this,[=](){
        lastmsg=QString();
        emit sendBroadcastMsg(myName + QString(" say: ") + ui->lineEdit_textToSend->text());
        ui->lineEdit_textToSend->clear();
    });

    QPushButton *addFriendToGroup = new QPushButton(QString("add friends"));
    ui->horizontalLayout_misc->addWidget(addFriendToGroup);
    connect(addFriendToGroup,SIGNAL(clicked()),this,SLOT(popAddFriendsDialog()));

    connect(ui->pushButton_selectFile,&QPushButton::clicked,this,[=](){
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle("select file");
        fileDialog->setDirectory(".");
        if(fileDialog->exec() == QDialog::Accepted) {
            QString path = fileDialog->selectedFiles()[0];
            ui->lineEdit_textToSend->clear();
            ui->lineEdit_textToSend->setText(QString("file://" + path));
        }
    });
}

sessionTab::sessionTab(QString friendName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sessionTab)
{
    ui->setupUi(this);
    ui->label_friendName->setText(friendName);
    sessionContentLayout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(sessionContentLayout);
    connect(ui->pushButton_send,&QPushButton::clicked,this,[=](){
        lastmsg=QString();
        emit sendMsg(myName + QString(" say: ") + ui->lineEdit_textToSend->text());
        ui->lineEdit_textToSend->clear();
    });
    connect(ui->pushButton_selectFile,&QPushButton::clicked,this,[=](){
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle("select file");
        fileDialog->setDirectory(".");
        if(fileDialog->exec() == QDialog::Accepted) {
            QString path = fileDialog->selectedFiles()[0];
            ui->lineEdit_textToSend->clear();
            ui->lineEdit_textToSend->setText(QString("file://" + path));
        }
    });
}

sessionTab::~sessionTab()
{
    delete ui;
}

void sessionTab::msgSent(QString msg){
    if(lastmsg!=msg && msg.mid(0,myName.size()) == myName){
        sessionContentLayout->addWidget(new QLabel(/*QString("you say: ") +*/ msg));
        lastmsg=msg;
    }
}

void sessionTab::newMsg(QString msg){
    if(msg.mid(0,myName.size()) == myName){

    }
    else{
        sessionContentLayout->addWidget(new QLabel(/*QString("peer say:") +*/ msg));
    }
}

void sessionTab::updateConnectStatus(QString status)
{
    ui->label_connectStatus->setText(status);
}

void sessionTab::popAddFriendsDialog()
{
    SelectFriendsDialog *selectFriend = new SelectFriendsDialog();
    connect(selectFriend,SIGNAL(exportFriendsIP(QString)),this,SLOT(addFriends(QString)));
    selectFriend->exec();
}

void sessionTab::addFriends(QString friendIP)
{
//    for (QString &friendIP:friendsIP){
        SessionWorkerThread *sessionThread = new SessionWorkerThread(friendIP);
        connect(this,SIGNAL(sendBroadcastMsg(QString)),sessionThread->getSessionWorker(),SLOT(addMsgToQueue(QString)));
        connect(sessionThread->getSessionWorker(),SIGNAL(newMsg(QString)),this,SLOT(sendBroadcastMsgSLOT(QString)));
        connect(sessionThread->getSessionWorker(),SIGNAL(newMsg(QString)),this,SLOT(newMsg(QString)));
        connect(sessionThread->getSessionWorker(),SIGNAL(msgSent(QString)),this,SLOT(msgSent(QString)));
        sessionThread->start();
//    }
}

void sessionTab::sendBroadcastMsgSLOT(QString msg)
{
    emit sendBroadcastMsg(msg);
}
