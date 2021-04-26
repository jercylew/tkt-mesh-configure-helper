#include "tktmeshupdatediscoverhelper.h"

#include <QTcpSocket>
#include "tools.h"
#include "domain/tktmesh.h"
#include <QFutureWatcher>
#include <QtConcurrent>

TKTMesh *TKTMeshUpdateDiscoverHelper__asyncScanTKTMesh(const QHostAddress &address, int waitMsec)
{
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost(address, 5199);
    bool ok=tcpSocket.waitForConnected(waitMsec);
    if(!ok)
    {
        return NULL;
    }
    tcpSocket.waitForReadyRead(waitMsec);
    QByteArray data=tcpSocket.readAll();
    qDebug()<<"READ: "<<data;
    if(!data.startsWith("{"))
    {
        return NULL;
    }
    QJsonObject tktMeshJson=Tools::getJsonObjectFromDataArray(data);
    if(tktMeshJson.isEmpty())
    {
        return NULL;
    }
    if(tktMeshJson.value("topic").toString()!="description")
    {
        return NULL;
    }
    QJsonObject dataObj=tktMeshJson.value("data").toObject();
    if(dataObj.isEmpty())
    {
        return NULL;
    }
    QString hostID=dataObj.value("host_id").toString();
    if(hostID.isEmpty())
    {
        return NULL;
    }
    TKTMesh *tktMesh=new TKTMesh;
    tktMesh->setHostID(hostID);
    tktMesh->setPlatform(dataObj.value("platform").toString());
    tktMesh->setHostName(dataObj.value("host_name").toString());
    tktMesh->setUpdateDescription(dataObj.value("update_description").toString());
    tktMesh->setHostPeerIP(tcpSocket.peerAddress().toString());
    tcpSocket.close();
    return tktMesh;
}


TKTMeshUpdateDiscoverHelper::TKTMeshUpdateDiscoverHelper(QObject *parent) : QObject(parent)
{

}

void TKTMeshUpdateDiscoverHelper::scan(const QHostAddress &address1, const QHostAddress &address2, int waitMsec)
{
    quint32 ip1=address1.toIPv4Address();
    quint32 ip2=address2.toIPv4Address();
    scanTotal=(ip2-ip1+1);
    scanFinished=0;
    for(quint32 ip=ip1; ip<=ip2; ip++)
    {
        QHostAddress address(ip);
        QFutureWatcher<TKTMesh*> *futureWatcher=new QFutureWatcher<TKTMesh*>;
        connect(futureWatcher, SIGNAL(finished()),
                this, SLOT(doAsyncScanTKTMeshTaskFinished()));
        QFuture<TKTMesh*> future=QtConcurrent::run(TKTMeshUpdateDiscoverHelper__asyncScanTKTMesh, address, waitMsec);
        futureWatcher->setFuture(future);
    }
}

void TKTMeshUpdateDiscoverHelper::doAsyncScanTKTMeshTaskFinished()
{
    QFutureWatcher<TKTMesh*> *futureWatcher=static_cast<QFutureWatcher<TKTMesh*>*>(sender());
    if(futureWatcher)
    {
        TKTMesh *tktMesh=futureWatcher->result();
        if(tktMesh!=NULL)
        {
            emit discovered(tktMesh);
        }
        delete futureWatcher;
    }
    scanFinished++;
    emit scanProgress(scanTotal, scanFinished);
}
