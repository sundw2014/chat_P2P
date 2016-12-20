#include "sessiontab.h"
#include "ui_sessiontab.h"
#include <QVBoxLayout>
#include <QLabel>
#include <common.h>

sessionTab::sessionTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sessionTab)
{
    ui->setupUi(this);
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
        emit sendMsg(ui->lineEdit_textToSend->text());
        ui->lineEdit_textToSend->clear();
    });
}

sessionTab::~sessionTab()
{
    delete ui;
}

void sessionTab::msgSent(QString msg){
    QLabel *label = new QLabel();
    label->setText(msg);
    // label->
    sessionContentLayout->addWidget(new QLabel(/*QString("you say: ") +*/ msg));
}

void sessionTab::newMsg(QString msg){
    QLabel *label = new QLabel();
    label->setText(msg);
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
