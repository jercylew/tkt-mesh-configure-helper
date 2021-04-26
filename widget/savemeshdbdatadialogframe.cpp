#include "savemeshdbdatadialogframe.h"
#include "ui_savemeshdbdatadialogframe.h"

#include "utils/asyncapirequesttask.h"
#include "callback/setmeshdbdatacallback.h"

#include "model/meshmodel.h"
#include "model/preparemeshmodel.h"
#include "domain/mesh.h"

#include <QFile>
#include "globalsavingbox.h"
#include "utils/tools.h"
#include "utils/remoteconfigsessionholder.h"
#include "utils/remoteconfigmanager.h"
#include "model/tktmeshmodel.h"
#include "utils/tktmeshapiworkercontroller.h"
#include "utils/tktmeshconnectionholder.h"
#include "domain/tktmesh.h"

SaveMeshDBDataDialogFrame::SaveMeshDBDataDialogFrame(PrepareMeshModel *prepareMeshModel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SaveMeshDBDataDialogFrame)
{
    ui->setupUi(this);
    this->currentOffset=0;
    this->m_prepareMeshModel=prepareMeshModel;

    QFile file(GlobalSavingBox::getInstance()->getMeshDBFilePath());
    this->dbFileTotal=file.size();
    if(this->dbFileTotal<=0)
    {
        finished(false, "数据库文件为空!");
        return;
    }
    if(!file.open(QFile::ReadOnly))
    {
        finished(false, "数据库文件无法打开!");
        return;
    }
    dbData=file.readAll();
    file.close();
    if(dbData.size()!=this->dbFileTotal)
    {
        finished(false, "读取数据库文件出错!");
        return;
    }

    this->dbFileMD5=Tools::getFileMD5(GlobalSavingBox::getInstance()->getMeshDBFilePath());
    ui->frameClose->setVisible(false);

    ui->labelText->setText("正在保存...");
    setMeshDBDataOnece();
}

SaveMeshDBDataDialogFrame::~SaveMeshDBDataDialogFrame()
{
    delete ui;
}

void SaveMeshDBDataDialogFrame::setStatusText(const QString &text, const QString &color)
{
    ui->labelText->setText(text);
}

void SaveMeshDBDataDialogFrame::on_buttonClose_clicked()
{
    emit accept();
}

void SaveMeshDBDataDialogFrame::doSetMeshDBDataFinished(bool ok)
{
    if(ok)
    {
        SetMeshDBDataCallback *callback=static_cast<SetMeshDBDataCallback*>(sender());
        this->currentOffset=callback->getWrite();
        if(this->currentOffset>=this->dbFileTotal)
        {
            if(callback->getMd5Ok())
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
            ui->labelText->setText(QString("已保存 %1/%2").arg(this->currentOffset).arg(this->dbFileTotal));
            setMeshDBDataOnece();
        }
    }
    else
    {
        finished(false, "发送保存Mesh数据库文件失败");
    }
}

void SaveMeshDBDataDialogFrame::setMeshDBDataOnece()
{
    quint64 length=this->dbFileTotal-this->currentOffset;
    if(length<=0)
    {
        return;
    }
    if(length>(this->m_prepareMeshModel->tktMeshModel()->tktMesh()->dbDataPackageMaxSizeInKB()*1024))
    {
        length=this->m_prepareMeshModel->tktMeshModel()->tktMesh()->dbDataPackageMaxSizeInKB()*1024;
    }

    SetMeshDBDataCallback *callback=new SetMeshDBDataCallback(this, this, m_prepareMeshModel);
    connect(callback, SIGNAL(setMeshDBDataFinished(bool)),
            this, SLOT(doSetMeshDBDataFinished(bool)));
    AsyncApiRequestTask *task=new AsyncApiRequestTask(callback);
    task->requestJson()->insert("topic", "request");
    task->requestJson()->insert("command", "set_mesh_db_data");
    QJsonObject dataJson;
    dataJson.insert("mesh_uuid", m_prepareMeshModel->mesh()->meshUUID());
    dataJson.insert("total", QString::number(this->dbFileTotal));
    dataJson.insert("md5", this->dbFileMD5);
    dataJson.insert("offset", QString::number(this->currentOffset));
    dataJson.insert("length", QString::number(length));

    QByteArray partData=dbData.mid(this->currentOffset, length);
    dataJson.insert("db_data", QString::fromLatin1(partData.toBase64()));

    task->requestJson()->insert("data", dataJson);
    m_prepareMeshModel->tktMeshModel()->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(task);
}

void SaveMeshDBDataDialogFrame::finished(bool ok, const QString &error)
{
    if(ok)
    {
        ui->labelText->setText("保存成功！");
    }
    else
    {
        if(error.isEmpty())
        {
            ui->labelText->setText("保存失败!");
        }
        else
        {
            ui->labelText->setText(QString("保存失败!（%1）").arg(error));
        }
    }
    ui->frameClose->setVisible(true);
}
