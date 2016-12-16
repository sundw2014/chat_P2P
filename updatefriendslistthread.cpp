#include "updatefriendslistthread.h"
#include "mainwindow.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringListModel>
#include "common.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QTime>

QString timeOut("time out");
QString offline("offline");
QString wrongFriendNo("wrong friend No");

UpdateFriendsListThread::UpdateFriendsListThread(MainWindow *_bindWindow,QString _serverIP, int _serverPort)
{
    serverIP = _serverIP;
    serverPort = _serverPort;

    bindWindow = _bindWindow;
    connect(this,SIGNAL(gotFriendsList(QStringList)),bindWindow,SLOT(updateFriendsListModel(QStringList)));
}

UpdateFriendsListThread::~UpdateFriendsListThread()
{
    if(tcpSocket2Server!=nullptr){
        delete tcpSocket2Server;
    }
}

QString UpdateFriendsListThread::checkFriendStatus(QTcpSocket *client, const QString friend_No)
{
    client->flush();
    client->write((QString('q')+friend_No).toLocal8Bit().data());
    if(client->waitForBytesWritten(100)==false){
        return timeOut;
    }
    QString response;
    if(client->waitForReadyRead(1000) == false){
        return timeOut;
    }
    QTime t;
    t.start();
    while (t.elapsed() < 1000) {
        response += client->readAll();
        if(response == "n"){
            // offline
            return offline;
        }
        else if(response == "Incorrect No."){
            // wrong friend No
            return wrongFriendNo;
        }
        else if(QHostAddress().setAddress(response)){
            // valid IP
            return response;
        }
    }
    // out of time
    return timeOut;
}

QStringList UpdateFriendsListThread::getFriendsListWithStatus(QStringList friendsList)
{
    for(int i=0;i<friendsList.size();i++)
    {
        QString _friend = friendsList[i];
        QStringList _friendSplited = _friend.split('\t');
        QString friend_No = _friendSplited[0];
        QString result = checkFriendStatus(tcpSocket2Server, friend_No);
//        qDebug()<<friend_No<<" is "<<result;
        if(result == offline)
        {
            _friend = _friendSplited[0] + "\t" + _friendSplited[1];
        }
        else if(result == timeOut)
        {

        }
        else if(result == wrongFriendNo)
        {

        }
        else{
            _friend = _friendSplited[0] + "\t" + _friendSplited[1] + "\t" + result;
        }
        friendsList[i] = _friend;
    }
    return friendsList;
}
