#include "tktmeshdiscoverhelper.h"
#include <QTcpSocket>
#include "tools.h"
#include "domain/tktmesh.h"
#include <QFutureWatcher>
#include <QtConcurrent>

TKTMesh *TKTMeshDiscoverHelper__asyncScanTKTMesh(const QHostAddress &address, int waitMsec)
{
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost(address, 5200);
    bool ok=tcpSocket.waitForConnected(waitMsec);
    if(!ok)
    {
        return NULL;
    }
    tcpSocket.write(QString("WHO?\n").toLatin1());
    tcpSocket.flush();
    tcpSocket.waitForReadyRead(waitMsec);
    QByteArray data=tcpSocket.readAll();
    if(!data.startsWith("{"))
    {
        return NULL;
    }
    QJsonObject tktMeshJson=Tools::getJsonObjectFromDataArray(data);
    if(tktMeshJson.isEmpty())
    {
        return NULL;
    }
    QString hostID=tktMeshJson.value("host_id").toString();
    if(hostID.isEmpty())
    {
        return NULL;
    }
    TKTMesh *tktMesh=new TKTMesh;
    tktMesh->setHostID(hostID);
    tktMesh->setHostCode(tktMeshJson.value("host_code").toString());
    QJsonObject hostDataJson=tktMeshJson.value("host_data").toObject();
    tktMesh->setHostName(hostDataJson.value("host_name").toString());
    tktMesh->setHostDescription(hostDataJson.value("host_description").toString());
    tktMesh->setHostKey(hostDataJson.value("skey").toString());
    tktMesh->setHostIP(hostDataJson.value("host_ip").toString());
    tktMesh->setHostPeerIP(tcpSocket.peerAddress().toString());
    tktMesh->setHostHardVersion(hostDataJson.value("host_hardver").toString());
    tktMesh->setHostSoftVersion(hostDataJson.value("host_softver").toString());
    tktMesh->setHostRegistered(hostDataJson.value("host_registered").toBool());
    tktMesh->setSystemTime(hostDataJson.value("system_time").toString());
    return tktMesh;
}


TKTMeshDiscoverHelper::TKTMeshDiscoverHelper(QObject *parent) : QObject(parent)
{

}

void TKTMeshDiscoverHelper::scan(const QHostAddress &address1, const QHostAddress &address2, int waitMsec)
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
        QFuture<TKTMesh*> future=QtConcurrent::run(TKTMeshDiscoverHelper__asyncScanTKTMesh, address, waitMsec);
        futureWatcher->setFuture(future);
    }
}

void TKTMeshDiscoverHelper::doAsyncScanTKTMeshTaskFinished()
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
