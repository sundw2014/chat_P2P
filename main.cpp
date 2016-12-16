#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"
#include <QString>
#include <QTimer>
#include <QThread>
#include "sessionswindow.h"

QString serverIP = "166.111.140.14";
int serverPort = 8000;
int clientPort = 10086;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog loginDlg;
    int result = QDialog::Rejected;
    while(result!=QDialog::Accepted)
    {
        result = loginDlg.exec();
    }

    MainWindow w_FriendsList;
    w_FriendsList.show();

    SessionsWindow w_Sessions;
    w_Sessions.show();

    QObject::connect(&w_FriendsList,SIGNAL(newSession(QStringList)),&w_Sessions,SLOT(addNewSession(QStringList)));
    QObject::connect(&w_FriendsList,SIGNAL(newSession(SessionWorkerThread*)),&w_Sessions,SLOT(addNewSession(SessionWorkerThread*)));
//    QThread* updateFriendsThread = new QThread(nullptr);
//    QTimer *updateFriendsStatusTimer = new QTimer(nullptr);
//    updateFriendsStatusTimer->setInterval(10000);
//    updateFriendsStatusTimer->moveToThread(updateFriendsThread);
//    QObject::connect(updateFriendsStatusTimer, SIGNAL(timeout()), &w_FriendsList, SLOT(updateFriendsStatus()),Qt::DirectConnection);
//    QObject::connect(updateFriendsThread, SIGNAL(started()), updateFriendsStatusTimer, SLOT(start()));
//    updateFriendsThread->start();
//    updateFriendsStatusTimer->start(10000);
    return a.exec();
}
