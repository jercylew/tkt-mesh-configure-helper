#include "updatethreadworker.h"
#include <QFile>
#include "utils/tools.h"
#include <QThread>
#include <QDebug>

UpdateThreadWorker::UpdateThreadWorker(const QString &hostId, const QString &ip, const QString &updateFilePath, QThread *workerThread, QObject *parent) : QObject(parent)
{
    m_tcpSocket=NULL;
    this->m_hostId=hostId;
    this->m_ip=ip;
    this->m_updateFilePath=updateFilePath;
    this->m_workerThread=workerThread;
    m_writeFileSize=0;
    m_updateFile=NULL;
    m_workIsFinished=false;
}

UpdateThreadWorker::~UpdateThreadWorker()
{
    if(m_tcpSocket!=NULL)
    {
        m_tcpSocket->close();
        m_tcpSocket->deleteLater();
        m_tcpSocket=NULL;
    }
    if(m_updateFile!=NULL)
    {
        m_updateFile->close();
        m_updateFile->deleteLater();
        m_updateFile=NULL;
    }
    m_workerThread->quit();
    m_workerThread->wait();
}

void UpdateThreadWorker::startUpdate()
{
    m_tcpSocket=new QTcpSocket;
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(doTCPSocketConnected()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doTCPSocketDisconnected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(doTCPSocketDisconnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(doTCPSocketReadyRead()));
    connect(m_tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(doTCPSocketBytesWritten(qint64)));
    m_tcpSocket->connectToHost(m_ip, 5199);
    emit updateStarted(m_hostId);
}

void UpdateThreadWorker::doTCPSocketConnected()
{
    QFile file(m_updateFilePath);
    qint64 fileSize=file.size();
    QString md5=Tools::getMD5Text(this->m_hostId);
    QString header=QString("%1,%2\n").arg(md5).arg(fileSize);
    m_tcpSocket->write(header.toLatin1().data());
}

void UpdateThreadWorker::doTCPSocketDisconnected()
{
    if(!m_workIsFinished)
    {
        m_workIsFinished=true;
        emit updateFinished(m_hostId, false);
    }
}

void UpdateThreadWorker::doTCPSocketReadyRead()
{
    m_readDataBuffer.push_back(m_tcpSocket->readAll());
    int newLineIndex=m_readDataBuffer.indexOf('\n');
    while(newLineIndex>=0)
    {
        QByteArray requestData=m_readDataBuffer.left(newLineIndex).trimmed();
        qDebug()<<"Read: "<<requestData;
        if((!requestData.isEmpty()) && requestData.startsWith("{"))
        {
            QJsonObject json=Tools::getJsonObjectFromDataArray(requestData);
            if(json.value("topic").toString()=="progress")
            {
                int progress=json.value("data").toInt();
                emit updateProgressReport(m_hostId, progress);
            }
            else if(json.value("topic").toString()=="result")
            {
                bool success=json.value("data").toBool();
                if(!m_workIsFinished)
                {
                    m_workIsFinished=true;
                    emit updateFinished(m_hostId, success);
                }
            }
        }
        m_readDataBuffer.remove(0, newLineIndex+1);
        newLineIndex=m_readDataBuffer.indexOf('\n');
    }
}

void UpdateThreadWorker::doTCPSocketBytesWritten(qint64 bytes)
{
    qDebug()<<"bytes written, bytes = "<<bytes;
    if(m_updateFile==NULL)
    {
        m_updateFile=new QFile(m_updateFilePath);
        m_updateFile->open(QFile::ReadOnly);
    }
    qint64 remain=(m_updateFile->size()-m_writeFileSize);
    qint64 len=qMin(remain, (qint64)(100*1024));
    qDebug()<<"len = "<<len;
    if(len>0)
    {
        QByteArray data=m_updateFile->read(len);
        qDebug()<<"data size = "<<data.size();
        m_tcpSocket->write(data);
        m_writeFileSize=m_writeFileSize+len;
    }
    else
    {
        m_updateFile->close();
        m_updateFile->deleteLater();
        m_updateFile=NULL;
    }
}

QThread *UpdateThreadWorker::workerThread() const
{
    return m_workerThread;
}
