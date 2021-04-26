#include "tktmeshapiworker.h"

#include "utils/tools.h"
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QHostAddress>
#include "domain/tktmesh.h"
#include <QDebug>

TKTMeshApiWorker::TKTMeshApiWorker(TKTMesh *tktMesh, QObject *parent) : QObject(parent)
{
    this->m_tktMesh=tktMesh;
    m_heartBeatDeadTimer=NULL;
}

TKTMeshApiWorker::~TKTMeshApiWorker()
{
    m_heartBeatDeadTimer->stop();
}

void TKTMeshApiWorker::initWorker()
{
    if(m_heartBeatDeadTimer==NULL)
    {
        m_heartBeatDeadTimer=new QTimer(this);
        m_heartBeatDeadTimer->setInterval(20*1000); //20秒
        m_heartBeatDeadTimer->setSingleShot(true);
        connect(m_heartBeatDeadTimer, SIGNAL(timeout()), this, SLOT(doHeartBeatDeadTimerout()));
    }
}

void TKTMeshApiWorker::connectHost()
{
    if(m_tktMesh->inCloudConfigMode())
    {
        bool ok=false;
        do{
            m_tcpSocket=new QTcpSocket(this);
            m_tcpSocket->connectToHost(QHostAddress(m_tktMesh->cloudConfigAddress()), m_tktMesh->cloudConfigConfigerPort());
            m_tcpSocket->waitForConnected(3000);
            if(m_tcpSocket->state()!=QTcpSocket::ConnectedState)
            {
                ok=false;
                break;
            }
            QJsonObject json;
            json.insert("command", "connect");
            QJsonObject dataJson;
            dataJson.insert("host_id", m_tktMesh->hostID());
            json.insert("data", dataJson);
            QByteArray data=Tools::getDataArrayOfJsonObject(&json);
            m_tcpSocket->write(data.append('\n'));
            m_tcpSocket->flush();
            m_tcpSocket->waitForReadyRead(3000);
            QByteArray readData=m_tcpSocket->readAll();
            if(readData.isEmpty())
            {
                ok=false;
                break;
            }
            if(readData.trimmed().startsWith("CONNECT_SUCCESS!"))
            {
                ok=true;
                break;
            }
            else
            {
                ok=false;
                break;
            }
        }while(false);

        emit connectHostFinished(ok);

        if(ok)
        {
            connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(doReadyRead()));
            resetHeartBeatDeadTimer();
        }
    }
    else
    {
        bool ok=false;
        do{
            m_tcpSocket=new QTcpSocket(this);
            m_tcpSocket->connectToHost(QHostAddress(m_tktMesh->hostPeerIP()), 5200);
            m_tcpSocket->waitForConnected(2000);
            if(m_tcpSocket->state()!=QTcpSocket::ConnectedState)
            {
                ok=false;
                break;
            }
            QJsonObject json;
            json.insert("topic", "request");
            json.insert("uuid", Tools::getRandomUUID());
            json.insert("command", "connect");
            QByteArray data=Tools::getDataArrayOfJsonObject(&json);
            m_tcpSocket->write(data.append('\n'));
            m_tcpSocket->flush();
            m_tcpSocket->waitForReadyRead(3000);
            QByteArray readData=m_tcpSocket->readAll();
            if(readData.isEmpty())
            {
                ok=false;
                break;
            }
            QJsonObject readJson=Tools::getJsonObjectFromDataArray(readData);
            if(readJson.value("result").toString()=="success")
            {
                ok=true;
                break;
            }
            else
            {
                ok=false;
                break;
            }
        }while(false);

        emit connectHostFinished(ok);

        if(ok)
        {
            connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(doReadyRead()));
            resetHeartBeatDeadTimer();
        }
    }
}

void TKTMeshApiWorker::asyncRequest(QJsonObject *requestJson)
{
    QByteArray data=Tools::getDataArrayOfJsonObject(requestJson);
    qDebug()<<"Write: "<<QString::fromLatin1(data);
    m_tcpSocket->write(data.append('\n'));
    m_tcpSocket->flush();
    delete requestJson;
}

void TKTMeshApiWorker::heartBeat()
{
    if(m_tktMesh->inCloudConfigMode())
    {
        QJsonObject json;
        json.insert("command", "heartbeat");
        QJsonObject dataJson;
        dataJson.insert("host_id", m_tktMesh->hostID());
        dataJson.insert("msg", "OK?");
        json.insert("data", dataJson);
        QByteArray data=Tools::getDataArrayOfJsonObject(&json);
        m_tcpSocket->write(data.append('\n'));
        m_tcpSocket->flush();
    }
    else
    {
        m_tcpSocket->write("OK?\n");
        m_tcpSocket->flush();
    }
}

void TKTMeshApiWorker::doReadyRead()
{
    if(m_tcpSocket==NULL)
    {
        return;
    }
    m_dataBuffer.push_back(m_tcpSocket->readAll());
    int newLineIndex=m_dataBuffer.indexOf('\n');
    while(newLineIndex>=0)
    {
        QByteArray readData=m_dataBuffer.left(newLineIndex).trimmed();
        if(!readData.isEmpty())
        {
            qDebug()<<"Read: "<<readData;
            if(readData=="OK!")
            {
                resetHeartBeatDeadTimer();
            }
            else if(readData.startsWith("{"))
            {
                QJsonObject readJson=Tools::getJsonObjectFromDataArray(readData);
                QJsonObject *responseJson=new QJsonObject(readJson);
                emit asyncResponse(responseJson);
            }
        }
        m_dataBuffer.remove(0, newLineIndex+1);
        newLineIndex=m_dataBuffer.indexOf('\n');
    }
}

void TKTMeshApiWorker::resetHeartBeatDeadTimer()
{
    if(m_heartBeatDeadTimer!=NULL)
    {
        m_heartBeatDeadTimer->stop();
        m_heartBeatDeadTimer->start();
    }
}

void TKTMeshApiWorker::doHeartBeatDeadTimerout()
{
    m_tcpSocket->close();
    emit socketDisconnected();
}
