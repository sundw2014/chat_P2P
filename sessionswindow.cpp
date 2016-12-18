#include "sessionswindow.h"
#include "ui_sessionswindow.h"
#include "sessionworkerthread.h"
#include <QWidget>
#include "sessiontab.h"

SessionsWindow::SessionsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SessionsWindow)
{
    ui->setupUi(this);
}

SessionsWindow::~SessionsWindow()
{
    delete ui;
}

void SessionsWindow::addNewSession(QStringList friendSplited)
{
//    ui->
    sessionTab *newTab = new sessionTab(friendSplited[1]);
    ui->tabWidget_session->addTab(newTab,friendSplited[1]);

    SessionWorkerThread *sessionThread = new SessionWorkerThread(friendSplited[2]);
    connect(sessionThread->getSessionWorker(),SIGNAL(msgSent(QString)),newTab,SLOT(msgSent(QString)));
    connect(newTab, SIGNAL(sendMsg(QString)), sessionThread->getSessionWorker(), SLOT(addMsgToQueue(QString)));
    connect(sessionThread->getSessionWorker(),SIGNAL(newMsg(QString)),newTab,SLOT(newMsg(QString)));
    connect(sessionThread->getSessionWorker(),SIGNAL(connectStatusChanged(QString)),newTab,SLOT(updateConnectStatus(QString)));
    sessionThread->start();
}

void SessionsWindow::addNewSession(SessionWorkerThread *sessionThread)
{
    QString fake("fake");
    sessionTab *newTab = new sessionTab(fake);
    ui->tabWidget_session->addTab(newTab,fake);

    connect(sessionThread->getSessionWorker(),SIGNAL(msgSent(QString)),newTab,SLOT(msgSent(QString)));
    connect(newTab, SIGNAL(sendMsg(QString)), sessionThread->getSessionWorker(), SLOT(addMsgToQueue(QString)));
    connect(sessionThread->getSessionWorker(),SIGNAL(newMsg(QString)),newTab,SLOT(newMsg(QString)));
    connect(sessionThread->getSessionWorker(),SIGNAL(connectStatusChanged(QString)),newTab,SLOT(updateConnectStatus(QString)));
    sessionThread->start();
}
