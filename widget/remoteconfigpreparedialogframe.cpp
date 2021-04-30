#include "remoteconfigpreparedialogframe.h"
#include "ui_remoteconfigpreparedialogframe.h"
#include "callback/getmeshdbinfocallback.h"
#include "callback/getmeshdbdatacallback.h"
#include "callback/keepmeshremoteconfigopencallback.h"
#include "model/tktmeshmodel.h"
#include "model/preparemeshmodel.h"
#include "domain/mesh.h"
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"
#include "utils/remoteconfigsessionholder.h"
#include "utils/remoteconfigmanager.h"
#include "globalsavingbox.h"
#include "utils/tools.h"
#include "domain/tktmesh.h"
#include <QFile>

RemoteConfigPrepareDialogFrame::RemoteConfigPrepareDialogFrame(PrepareMeshModel *prepareMeshModel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RemoteConfigPrepareDialogFrame)
{
    ui->setupUi(this);
    this->m_prepareMeshModel=prepareMeshModel;
    ui->frameClose->setVisible(false);

    GetMeshDBInfoCallback *callback=new GetMeshDBInfoCallback(this, this, m_prepareMeshModel);
    connect(callback, SIGNAL(getMeshDBInfoFinished(bool)),
            this, SLOT(doGetMeshDBInfoFinished(bool)));
    AsyncApiRequestTask *task=new AsyncApiRequestTask(callback);
    task->requestJson()->insert("topic", "request");
    task->requestJson()->insert("command", "get_mesh_db_info");
    task->requestJson()->insert("data", m_prepareMeshModel->mesh()->meshUUID());
    m_prepareMeshModel->tktMeshModel()->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(task);

    ui->labelText->setText("获取数据库信息中 ...");
}

RemoteConfigPrepareDialogFrame::~RemoteConfigPrepareDialogFrame()
{
    delete ui;
}

void RemoteConfigPrepareDialogFrame::setStatusText(const QString &text, const QString &color)
{
    ui->labelText->setText(text);
}

void RemoteConfigPrepareDialogFrame::setClosable(bool closable)
{
    if(closable)
    {
        emit dialogClosableChanged(true);
        ui->frameClose->setVisible(true);
    }
    else
    {
        ui->frameClose->setVisible(false);
    }
}

void RemoteConfigPrepareDialogFrame::getMeshDBDataOnece()
{
    quint64 length=(this->dbDataTotal-currentOffset);
    if(length<=0)
    {
        return;
    }
    if(length>(this->m_prepareMeshModel->tktMeshModel()->tktMesh()->dbDataPackageMaxSizeInKB()*1024))
    {
        length=this->m_prepareMeshModel->tktMeshModel()->tktMesh()->dbDataPackageMaxSizeInKB()*1024;
    }

    GetMeshDBDataCallback *callback=new GetMeshDBDataCallback(this, this, m_prepareMeshModel);
    connect(callback, SIGNAL(getMeshDBDataFinished(bool)),
            this, SLOT(doGetMeshDBDataFinished(bool)));
    AsyncApiRequestTask *task=new AsyncApiRequestTask(callback);
    task->requestJson()->insert("topic", "request");
    task->requestJson()->insert("command", "get_mesh_db_data");
    QJsonObject dataJson;
    dataJson.insert("mesh_uuid", this->m_prepareMeshModel->mesh()->meshUUID());
    dataJson.insert("offset", QString::number(this->currentOffset));
    dataJson.insert("length", QString::number(length));
    task->requestJson()->insert("data", dataJson);
    m_prepareMeshModel->tktMeshModel()->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(task);
}

void RemoteConfigPrepareDialogFrame::finished(bool ok, const QString &error)
{
    if(ok)
    {
        this->accept();
    }
    else
    {
        if(error.isEmpty())
        {
            ui->labelText->setText("获取MESH数据库信息失败！");
        }
        else
        {
            ui->labelText->setText(QString("获取MESH数据库信息失败！(%1)").arg(error));
        }
        setClosable(true);
    }
}


void RemoteConfigPrepareDialogFrame::on_buttonClose_clicked()
{
    emit reject();
}

void RemoteConfigPrepareDialogFrame::doGetMeshDBInfoFinished(bool ok)
{
    if(ok)
    {
        GetMeshDBInfoCallback *callBack=static_cast<GetMeshDBInfoCallback*>(sender());
        dbDataTotal=callBack->getTotal();
        dbMd5=callBack->getMd5();
        QTimer::singleShot(1000, this, SLOT(doStartGetMeshDBData()));
        ui->labelText->setText("获取数据库文件中...");
    }
    else
    {
        finished(false);
    }
}

void RemoteConfigPrepareDialogFrame::doGetMeshDBDataFinished(bool ok)
{
    if(ok)
    {
        GetMeshDBDataCallback *callBack=static_cast<GetMeshDBDataCallback*>(sender());
        quint64 offset=callBack->getOffset();
        quint64 length=callBack->getLength();
        QByteArray dbData=callBack->getDbData();
        if(dbData.length()!=length)
        {
            finished(false);
            return;
        }

        QFile file(GlobalSavingBox::getInstance()->getMeshDBFilePath());
        if(file.open(QFile::WriteOnly | QFile::Append))
        {
            file.write(dbData);
            file.flush();
            file.close();
        }
        ui->labelText->setText(QString("已经获取: %1/%2").arg(file.size()).arg(this->dbDataTotal));
        if((offset+length)>=this->dbDataTotal)
        {
            QString calculatedMD5=Tools::getFileMD5(GlobalSavingBox::getInstance()->getMeshDBFilePath());
            if(this->dbMd5.toUpper()==calculatedMD5.toUpper())
            {
                finished(true);
            }
            else
            {
                finished(false, "MD5校验错误");
            }
        }
        else
        {
            currentOffset=offset+length;
            getMeshDBDataOnece();
        }
    }
    else
    {
        finished(false);
    }
}

void RemoteConfigPrepareDialogFrame::doStartGetMeshDBData()
{
    QFile file(GlobalSavingBox::getInstance()->getMeshDBFilePath());
    if(file.exists())
    {
        file.resize(0);
    }
    currentOffset=0;
    this->getMeshDBDataOnece();
}

