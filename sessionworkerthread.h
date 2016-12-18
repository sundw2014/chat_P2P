#ifndef SESSIONWORKERTHREAD_H
#define SESSIONWORKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QTcpSocket>
#include <QHostAddress>
#include "common.h"
#include <QTimer>
#include <QDateTime>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>

const QString magicHead("05c5ce0993f4bb4ea2f06e32f7d7d5e8");
const QString magicTail("3e31402033a9092b63e4b42fe3921879");
const QString magicFileSpliter("65ef05e65fa4cecce8dadaf76800a65e");
const QString filePrefix("data/");

class SessionWorker : public QObject
{
    Q_OBJECT
public:
    SessionWorker(QString peerIP){
//        peerIP = "127.0.0.1";
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
//        qDebug()<<"processMsgQueue()";
        if(QAbstractSocket::ConnectedState != sessionSocket->state()){
            emit connectStatusChanged(QString("disconnected"));
            return;
        }
        else{
            emit connectStatusChanged(QString("connected"));
        }
        static QString receivedData;
        if(!msgToSend.isEmpty())
        {
            QString msg = msgToSend[0];

            sessionSocket->write(wrapMsg(msg).toLocal8Bit());
            if(sessionSocket->waitForBytesWritten(1000)==true){
                msgToSend.pop_front();
//                qDebug()<<"msgSent: "<<msg;
                emit msgSent(msg);
            }
            else{
                ;
            }
        }
//        if(sessionSocket->waitForReadyRead(10) == true){
            receivedData += sessionSocket->readAll();
//            qDebug()<<receivedData;
            QString result = checkOutAvailableMsg(receivedData);
            while(!result.isEmpty()){
                msgReceived.append(result);
                result = checkOutAvailableMsg(receivedData);
            }
//        }
        if(bufStatus == false){
            if(!msgReceived.isEmpty()){
//                qDebug()<<"msg Received: "<<msgReceived[0];
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
    void connectStatusChanged(QString status);

private:
    bool bufStatus = false;
    QStringList msgToSend;
    QStringList msgReceived;
    QTcpSocket *sessionSocket;
    const QString wrapMsg(QString msg){
        if(msg.toStdString().substr(0,7) == std::string("file://"))
        {
            wrapFile(QString::fromStdString(msg.toStdString().substr(7)),msg);
        }
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
                QString msg = QString::fromUtf8(_receivedData.substr(indexStart,indexStop-indexStart).c_str());
                QString result = checkOutFile(msg);
                if(result.isEmpty()){
                    return msg;
                }
                else{
                    return QString("new file, written in " + result);
                }
            }
            return QString();
        }
        else{
            return QString();
        }
    }

    QString checkOutFile(QString &msg){
        QStringList fileSplited = msg.split(magicFileSpliter);
        if(fileSplited.size()!=3){
            return QString();
        }
        QString filename = filePrefix + fileSplited[0] + QString("_") + QString::number(QDateTime::currentMSecsSinceEpoch());
        QByteArray fileContent = fileSplited[1].toLocal8Bit();

        if(fileSplited[2] == checkSum(fileContent)){
            qDebug() << "new file: " << filename;
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            file.write(fileContent);
            file.close();
            qDebug() << "finished write " << filename;
        }
        else{
            qDebug() << "wrong file md5";
        }
        return filename;
    }

    bool wrapFile(QString filename, QString& result){
        QFileInfo fileInfo(filename);
        QFile file(filename);
        file.open(QIODevice::ReadWrite);
        QByteArray content = file.readAll();
        file.close();
        QString checksum = checkSum(content);
        result = fileInfo.completeBaseName() + magicFileSpliter + QString::fromLocal8Bit(content) + magicFileSpliter + checksum;
        return true;
    }

    QString checkSum(const QByteArray& data){
        QCryptographicHash md5Sum(QCryptographicHash::Md5);
        md5Sum.addData(data);
        return md5Sum.result().toHex();
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
