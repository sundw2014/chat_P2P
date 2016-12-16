#include "sessionworkerthread.h"
#include <QTcpSocket>
#include <common.h>
#include <QTimer>

SessionWorkerThread::SessionWorkerThread(QString _peerIP) // active session
{
    peerIP = _peerIP;
}

SessionWorkerThread::SessionWorkerThread(QTcpSocket *_sessionSocket) // positive session
{
    sessionSocket->moveToThread(this);
    sessionSocket = _sessionSocket;
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
    if(sessionSocket!=nullptr){
        sessionWorker = new SessionWorker(sessionSocket);
    }
    else{
        sessionWorker = new SessionWorker(peerIP);
    }
    QTimer t(this);

    connect(&t,SIGNAL(timeout()),sessionWorker,SLOT(processMsgQueue()),Qt::DirectConnection);
    t.start(1000); //process queue per 1000ms

    exec();
}
