#include "renameonedialogframe.h"
#include "ui_renameonedialogframe.h"
#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "model/sensorsmodel.h"
#include "model/nodegroupsmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"

RenameOneDialogFrame::RenameOneDialogFrame(MeshModel *meshModel, QString id, const QString &type, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RenameOneDialogFrame),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    this->id=id;
    this->type=type;
    if(type=="light")
    {
        ui->lineNewName->setText(m_meshModel->executersModel()->getNodeName(id));
    }
    else if(type=="sensor")
    {
        ui->lineNewName->setText(m_meshModel->sensorsModel()->getNodeName(id));
    }
    else if(type=="group")
    {
        NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(id.toInt()));
        if(group!=NULL)
        {
            ui->lineNewName->setText(group->getName());
        }
    }
    ui->lineNewName->setFocus();

    ui->buttonDialogDone->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogCancel->setCursor(Qt::PointingHandCursor);
}

RenameOneDialogFrame::~RenameOneDialogFrame()
{
    delete ui;
}

void RenameOneDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void RenameOneDialogFrame::on_buttonDialogDone_clicked()
{
    if(type=="light")
    {
        Executer *executer=m_meshModel->executersModel()->getExecuter(this->id);
        if(executer==NULL)
        {
            return;
        }
        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        nameMap->insert(id, QPair<QString, QString>(executer->typeText(), ui->lineNewName->text()));
        m_meshModel->dbManager()->dbCaller()->setExecuterNodesName(nameMap);
    }
    else if(type=="sensor")
    {
        Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(this->id);
        if(sensor==NULL)
        {
            return;
        }
        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        nameMap->insert(id, QPair<QString, QString>(sensor->typeText(), ui->lineNewName->text()));
        m_meshModel->dbManager()->dbCaller()->setSensorNodesName(nameMap);
    }
    else if(type=="group")
    {
        QMap<quint8, QString> *nameMap=new QMap<quint8, QString>;
        nameMap->insert((quint8)(id.toInt()), ui->lineNewName->text());
        m_meshModel->dbManager()->dbCaller()->setNodeGroupsName(nameMap);
    }
    emit accept();
}

void RenameOneDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}

void RenameOneDialogFrame::on_lineNewName_returnPressed()
{
    ui->buttonDialogDone->click();
}
