#ifndef SESSIONTAB_H
#define SESSIONTAB_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>

namespace Ui {
class sessionTab;
}

class sessionTab : public QWidget
{
    Q_OBJECT

public:
    explicit sessionTab(QWidget *parent = 0);
    sessionTab(QString friendName, QWidget *parent = 0);
    ~sessionTab();

private:
    Ui::sessionTab *ui;
    QVBoxLayout *sessionContentLayout;
    QString lastmsg; //fuck dirty

public slots:
    void newMsg(QString msg);
    void msgSent(QString msg);
    void updateConnectStatus(QString status);
    void popAddFriendsDialog();
//    void addFriends(QStringList friendsIP, QStringList friendsName);
    void addFriends(QString friendIP);

signals:
    void sendMsg(QString msg);
    void sendBroadcastMsg(QString msg);
};

#endif // SESSIONTAB_H
