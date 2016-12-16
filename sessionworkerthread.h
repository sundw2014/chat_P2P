#ifndef SESSIONWORKERTHREAD_H
#define SESSIONWORKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QTcpSocket>
#include <QHostAddress>
#include "common.h"

class SessionWorker : public QObject
{
    Q_OBJECT
public:
    SessionWorker(QString peerIP){
        sessionSocket = new QTcpSocket(this);
        sessionSocket->connectToHost(QHostAddress(peerIP), clientPort);
    }

    SessionWorker(QTcpSocket *_sessionSocket){
        sessionSocket = _sessionSocket;
    }

    ~SessionWorker(){
        if(sessionSocket!=nullptr)
        {
            sessionSocket->close();
            delete sessionSocket;
        }
    }

public slots:
    void addMsgToQueue(QString msg){
        msgToSend.append(msg);
    }

    void processMsgQueue(){
        qDebug()<<"processMsgQueue() stub";
    }

    void setBufStatus(bool _bufStatus){
        bufStatus = _bufStatus;
    }

signals:
    void newMsg(QString msg);
    void msgSent(QString msg);

private:
    bool bufStatus = true;
    QStringList msgToSend;
    QStringList msgReceived;
    QTcpSocket *sessionSocket;
};

class SessionWorkerThread : public QThread
{
    Q_OBJECT
public:
    SessionWorkerThread(QString peerIP); // active session
    SessionWorkerThread(QTcpSocket *sessionSocket); // positive session
    ~SessionWorkerThread();
    const SessionWorker* getSessionWorker(){
        return sessionWorker;
    }

private:
    QString peerIP;
    QTcpSocket *sessionSocket = nullptr;
    SessionWorker *sessionWorker = nullptr;

protected:
    void run();
};

#endif // SESSIONWORKERTHREAD_H
