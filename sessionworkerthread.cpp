#include "sessionworkerthread.h"
#include <QTcpSocket>
#include <common.h>
#include <QTimer>

SessionWorkerThread::SessionWorkerThread(QString _peerIP) // active session
{
    peerIP = _peerIP;
    sessionWorker = new SessionWorker(peerIP);
    sessionWorker->moveToThread(this);
}

SessionWorkerThread::SessionWorkerThread(QTcpSocket *_sessionSocket) // positive session
{
    _sessionSocket->setParent(nullptr);
    _sessionSocket->moveToThread(this);
    sessionSocket = _sessionSocket;
    sessionWorker = new SessionWorker(sessionSocket);
    sessionWorker->moveToThread(this);
}

SessionWorkerThread::~SessionWorkerThread()
{
    if(sessionWorker!=nullptr)
    {
        delete sessionWorker;
    }
}

void SessionWorkerThread::run()
{

    QTimer t(nullptr);
    t.moveToThread(this);
//    QTimer t2(nullptr);
//    t2.moveToThread(this);
//    connect(&t2,SIGNAL(timeout()),sessionWorker,SLOT(fake()));
//    t2.start(1000);

    connect(&t,SIGNAL(timeout()),sessionWorker,SLOT(processMsgQueue()),Qt::DirectConnection);
    t.start(1000); //process queue per 1000ms

    exec();
}
