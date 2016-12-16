#ifndef SESSIONWORKERTHREAD_H
#define SESSIONWORKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QTcpSocket>
#include <QHostAddress>
#include "common.h"
#include <QTimer>

const QString magicHead("05c5ce0993f4bb4ea2f06e32f7d7d5e8");
const QString magicTail("3e31402033a9092b63e4b42fe3921879");

class SessionWorker : public QObject
{
    Q_OBJECT
public:
    SessionWorker(QString peerIP){
        peerIP = "127.0.0.1";
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

    void fake()
    {
        addMsgToQueue(QString("hello"));
    }

    void processMsgQueue(){
//        qDebug()<<"processMsgQueue() stub";
        static QString receivedData;
        if(!msgToSend.isEmpty())
        {
            QString msg = msgToSend[0];

            sessionSocket->write(wrapMsg(msg).toLocal8Bit());
            if(sessionSocket->waitForBytesWritten(1000)==true){
                msgToSend.pop_front();
                qDebug()<<"msgSent: "<<msg;
                emit msgSent(msg);
            }
            else{
                ;
            }
        }
        if(sessionSocket->waitForReadyRead(10) == true){
            receivedData += sessionSocket->readAll();
            QString result = checkOutAvailableMsg(receivedData);
            while(!result.isEmpty()){
                msgReceived.append(result);
                result = checkOutAvailableMsg(receivedData);
            }
        }
        if(bufStatus == false){
            if(!msgReceived.isEmpty()){
                qDebug()<<"msg Received: "<<msgReceived[0];
                emit newMsg(msgReceived[0]);
                msgReceived.pop_front();
            }
        }
    }

    void setBufStatus(bool _bufStatus){
        bufStatus = _bufStatus;
    }

signals:
    void newMsg(QString msg);
    void msgSent(QString msg);

private:
    bool bufStatus = false;
    QStringList msgToSend;
    QStringList msgReceived;
    QTcpSocket *sessionSocket;
    const QString wrapMsg(QString msg){
        return (magicHead + msg + magicTail);
    }
    const QString checkOutAvailableMsg(QString &receivedData){
        std::string _receivedData = receivedData.toStdString();
        size_t indexStart = _receivedData.find(magicHead.toStdString());
        if(indexStart!=std::string::npos){
            indexStart += magicHead.toStdString().length();
            size_t indexStop = _receivedData.find(magicTail.toStdString());
            if(indexStop!=std::string::npos){
                receivedData = QString::fromUtf8(\
                            _receivedData.substr(indexStop + magicTail.toStdString().length())\
                            .c_str());
                return QString::fromUtf8(_receivedData.substr(indexStart,indexStop-indexStart).c_str());
            }
            return QString();
        }
        else{
            return QString();
        }
    }
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
