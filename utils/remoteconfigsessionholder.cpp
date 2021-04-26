#include "remoteconfigsessionholder.h"
#include "model/meshmodel.h"
#include "domain/mesh.h"
#include "model/tktmeshmodel.h"
#include "domain/tktmesh.h"
#include <QFile>
#include <QApplication>
#include <QDir>
#include "utils/tools.h"
#include "globalsavingbox.h"
#include "model/preparemeshmodel.h"
#include "callback/defaultcallback.h"
#include "utils/tktmeshapiworkercontroller.h"
#include "utils/tktmeshconnectionholder.h"
#include <QJsonArray>

RemoteConfigSessionHolder::RemoteConfigSessionHolder(PrepareMeshModel *prepareMeshModel, QObject *parent) : QObject(parent)
{
    this->m_prepareMeshModel=prepareMeshModel;

    m_keepCommandReportTimer.setInterval(5*1000); //5 seconds
    connect(&m_keepCommandReportTimer, SIGNAL(timeout()),
            this, SLOT(doKeepCommandReportTimerTimeout()));

    connect(m_prepareMeshModel->tktMeshModel()->tktMeshConnectionHolder()->apiWorkerController(), SIGNAL(getReportData(QString,QJsonObject*)),
            this, SLOT(doGetReportData(QString,QJsonObject*)));
}

RemoteConfigSessionHolder::~RemoteConfigSessionHolder()
{
    if(m_keepCommandReportTimer.isActive())
    {
        m_keepCommandReportTimer.stop();
    }
}

void RemoteConfigSessionHolder::startKeepCommandReportTimer()
{
    m_keepCommandReportTimer.start();
    doKeepCommandReportTimerTimeout();
}

void RemoteConfigSessionHolder::stopKeepCommandReportTimer()
{
    m_keepCommandReportTimer.stop();
}

void RemoteConfigSessionHolder::writeCommandToTKTMesh(const QByteArray &cmd)
{
    DefaultCallback *callback=new DefaultCallback(NULL, NULL, m_prepareMeshModel->tktMeshModel()->tktMesh());
    AsyncApiRequestTask *task=new AsyncApiRequestTask(callback);
    task->requestJson()->insert("topic", "request");
    task->requestJson()->insert("command", "write_raw_commands");
    QJsonObject dataJson;
    dataJson.insert("mesh_uuid", m_prepareMeshModel->mesh()->meshUUID());
    QJsonArray commandArray;
    commandArray.push_back(QString::fromLatin1(cmd.toBase64()));
    dataJson.insert("commands", commandArray);
    task->requestJson()->insert("data", dataJson);
    m_prepareMeshModel->tktMeshModel()->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(task);
}

void RemoteConfigSessionHolder::doKeepCommandReportTimerTimeout()
{
    DefaultCallback *callback=new DefaultCallback(NULL, NULL, m_prepareMeshModel->tktMeshModel()->tktMesh());
    AsyncApiRequestTask *task=new AsyncApiRequestTask(callback);
    task->requestJson()->insert("topic", "request");
    if(this->m_prepareMeshModel->tktMeshModel()->tktMesh()->inCloudConfigMode())
    {
        task->requestJson()->insert("command", "keep_cloud_pc_config_command_dataframe_report");
    }
    else
    {
        task->requestJson()->insert("command", "keep_local_pc_config_command_dataframe_report");
    }
    task->requestJson()->insert("data", m_prepareMeshModel->mesh()->meshUUID());
    m_prepareMeshModel->tktMeshModel()->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(task);
}

void RemoteConfigSessionHolder::doGetReportData(const QString &command, QJsonObject *reportJson)
{
    if(command=="raw_commands_report")
    {
        QJsonObject dataJson=reportJson->value("data").toObject();
        QJsonArray commandArray=dataJson.value("commands").toArray();
        for(int i=0; i<commandArray.size(); i++)
        {
            QByteArray dataFrame=QByteArray::fromBase64(commandArray.at(i).toString().toLatin1());
            emit readADataFrame(dataFrame);
        }
    }
}
