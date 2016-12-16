#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QTimer>
#include <QTcpServer>
#include "sessionworkerthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    const QStringListModel* getFriendsListModel() const
    {
        return friendsListModel;
    }

private slots:
    void on_listView_friends_doubleClicked(const QModelIndex &index);

public slots:
    void updateFriendsListModel(const QStringList& friendsList){
        friendsListModel->setStringList(friendsList);
    }
    void newConnect();

signals:
    void newSession(QStringList friendsSplited);
    void newSession(SessionWorkerThread *sessionThread);

private:
    Ui::MainWindow *ui;
    QStringListModel* friendsListModel;
    QStringList* loadFriends(const QString friendsListFilename);
    QTcpServer* tcpServer;
};

#endif // MAINWINDOW_H
