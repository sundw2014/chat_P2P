#ifndef UPDATEFRIENDSLISTTHREAD_H
#define UPDATEFRIENDSLISTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <mainwindow.h>

class UpdateFriendsListThread : public QThread
{
    Q_OBJECT

public:
    UpdateFriendsListThread(){}
    UpdateFriendsListThread(MainWindow *_bindWindow,QString serverIP, int serverPort);
    ~UpdateFriendsListThread();

private:
    QTcpSocket *tcpSocket2Server;
    MainWindow *bindWindow;
    QString checkFriendStatus(QTcpSocket *client, const QString friend_No);
    QStringList getFriendsListWithStatus(QStringList friendsList);

protected:
    void run() {
        while(1) {
            const QStringListModel *friendsListModel = bindWindow->getFriendsListModel();
            QStringList l = getFriendsListWithStatus(friendsListModel->stringList());
            emit gotFriendsList(l);
        }
    }

signals:
    void gotFriendsList(QStringList l);
};

#endif // UPDATEFRIENDSLISTTHREAD_H
