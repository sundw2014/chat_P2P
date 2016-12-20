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
#include <QByteArrayMatcher>

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
        static QByteArray receivedData;
        if(!msgToSend.isEmpty())
        {
            QString msg = msgToSend[0];

            sessionSocket->write(wrapMsg(msg));
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
    const QByteArray wrapMsg(QString msg){
        // send a file
        QByteArray result;
        size_t index = msg.toStdString().find("file://");
        if(index!=std::string::npos){
            wrapFile(QString::fromStdString(msg.toStdString().substr(index+7)),result);
        }
        else{
            result = msg.toUtf8();
        }
        return (magicHead.toUtf8() + result + magicTail.toUtf8());
    }

    const QString checkOutAvailableMsg(QByteArray &receivedData){
        QByteArrayMatcher matcher_magicHead(magicHead.toUtf8());
        QByteArrayMatcher matcher_magicTail(magicTail.toUtf8());

        int indexStart = matcher_magicHead.indexIn(receivedData);
        if(indexStart!=-1){
            indexStart += matcher_magicHead.pattern().size();
            int indexStop = matcher_magicTail.indexIn(receivedData);
            if(indexStop!=-1){
                QByteArray msg = receivedData.mid(indexStart,indexStop-indexStart);
                receivedData = receivedData.mid(indexStop+matcher_magicTail.pattern().size());
                QString result = checkOutFile(msg);
                if(result=="no file"){
                    return QString::fromUtf8(msg);
                }
                else if(result=="wrong file md5"){
                    return QString("new file, but wrong md5 sum");
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

    QString checkOutFile(QByteArray &msg){
        QByteArrayMatcher matcher_magicFileSpliter(magicFileSpliter.toUtf8());
        int index1 = -1, index2 = -1;
        if((index1 = matcher_magicFileSpliter.indexIn(msg))==-1){
            return QString("no file");
        }
        if((index2 = matcher_magicFileSpliter.indexIn(msg,index1+1))==-1){
            return QString("no file");
        }
        QByteArray fileSplited1 = msg.mid(0,index1);
        QByteArray fileSplited2 = msg.mid(index1+matcher_magicFileSpliter.pattern().size(),index2-(index1+matcher_magicFileSpliter.pattern().size()));
        QByteArray fileSplited3 = msg.mid(index2+matcher_magicFileSpliter.pattern().size());

        QString filename = filePrefix + QString::fromUtf8(fileSplited1) + QString("_") + QString::number(QDateTime::currentMSecsSinceEpoch());
        QByteArray fileContent = fileSplited2;

        if(fileSplited3 == checkSum(fileContent)){
            qDebug() << "new file: " << filename;
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            file.write(fileContent);
            file.close();
            qDebug() << "finished write " << filename;
        }
        else{
            qDebug() << "wrong file md5";
            return QString("wrong file md5");
        }
        return filename;
    }

    bool wrapFile(QString filename, QByteArray& result){
//        filename = QString("/home/sundw/friends.txt");
        QFileInfo fileInfo(filename);
        while(filename.size() != 0){
            if(filename[filename.size()-1] == ' ' || filename[filename.size()-1] == '\r' || filename[filename.size()-1] == '\n')
                filename = QString::fromStdString(filename.toStdString().substr(0,filename.size()-1));
            else
                break;
        }
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return false;
//        file.open(QIODevice::ReadWrite);
        QByteArray content = file.readAll();
//        qDebug() << file.errorString() << "," << file.size();
        file.close();
        QByteArray checksum = checkSum(content);
        result = fileInfo.completeBaseName().toUtf8() + magicFileSpliter.toUtf8() + content + magicFileSpliter.toUtf8() + checksum;
        return true;
    }

    QByteArray checkSum(const QByteArray& data){
        QCryptographicHash md5Sum(QCryptographicHash::Md5);
        md5Sum.addData(data);
        return md5Sum.result();
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
