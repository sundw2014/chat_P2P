#include "sessionswindow.h"
#include "ui_sessionswindow.h"
#include "sessionworkerthread.h"

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
    (new SessionWorkerThread(friendSplited[2]))->start();
}
