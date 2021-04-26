#include "virtualmotionsensoreditdialogframe.h"
#include "ui_virtualmotionsensoreditdialogframe.h"
#include "model/sensorsmodel.h"
#include "model/meshmodel.h"
#include "domain/virtualmotionsensor.h"

VirtualMotionSensorEditDialogFrame::VirtualMotionSensorEditDialogFrame(MeshModel *meshModel, VirtualMotionSensor *virtualMotionSensor, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::VirtualMotionSensorEditDialogFrame)
{
    ui->setupUi(this);
    this->m_meshModel=meshModel;
    this->m_virtualMotionSensor=virtualMotionSensor;

    ui->listAllSensor->setSelectionMode(QListWidget::ExtendedSelection);
    ui->lineName->setText(virtualMotionSensor->getName());

    QStringList strList=virtualMotionSensor->bindingRealMotionSensorIdList();
    QSet<QString> selectedIdSet;
    for(int i=0; i<strList.size(); i++)
    {
        QString sensorId=strList.at(i);
        if(sensorId.isEmpty())
        {
            continue;
        }
        selectedIdSet.insert(sensorId);
        selectedIdList.push_back(sensorId);
    }

    QList<Sensor*> virtualSensorList=m_meshModel->sensorsModel()->allRealMotionSensor();
    for(int i=0; i<virtualSensorList.size(); i++)
    {
        Sensor *sensor=virtualSensorList.at(i);
        if(!selectedIdSet.contains(sensor->sensorId()))
        {
            canSelectedSensorIdList.push_back(sensor->sensorId());
        }
    }

    updateListWidget();
}

VirtualMotionSensorEditDialogFrame::~VirtualMotionSensorEditDialogFrame()
{
    delete ui;
}

void VirtualMotionSensorEditDialogFrame::on_buttonDialogBottomDone_clicked()
{
    m_virtualMotionSensor->setBindingRealMotionSensorIdList(this->selectedIdList);
    m_virtualMotionSensor->setName(ui->lineName->text().trimmed());
    emit accept();
}

void VirtualMotionSensorEditDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit reject();
}

void VirtualMotionSensorEditDialogFrame::on_buttonAddBindingSensor_clicked()
{
     QList<QListWidgetItem*> itemList=ui->listAllSensor->selectedItems();
     for(int i=0; i<itemList.size(); i++)
     {
         QString sensorId=itemList.at(i)->data(Qt::UserRole).toString();
         canSelectedSensorIdList.removeAll(sensorId);
         selectedIdList.push_back(sensorId);
     }
     updateListWidget();
}

void VirtualMotionSensorEditDialogFrame::on_buttonRemoveBindingSensor_clicked()
{
    QList<QListWidgetItem*> itemList=ui->listBindingSensor->selectedItems();
    for(int i=0; i<itemList.size(); i++)
    {
        QString sensorId=itemList.at(i)->data(Qt::UserRole).toString();
        selectedIdList.removeAll(sensorId);
        canSelectedSensorIdList.push_back(sensorId);
    }
    updateListWidget();
}

void VirtualMotionSensorEditDialogFrame::updateListWidget()
{
    ui->listAllSensor->clear();
    ui->listBindingSensor->clear();
    for(int i=0; i<canSelectedSensorIdList.size(); i++)
    {
        QListWidgetItem *item=new QListWidgetItem;
        QString sensorId=canSelectedSensorIdList.at(i);
        item->setText(QString("[%1]%2").arg(sensorId).arg(m_meshModel->sensorsModel()->getNodeName(sensorId)));
        item->setData(Qt::UserRole, sensorId);
        ui->listAllSensor->addItem(item);
    }
    for(int i=0; i<selectedIdList.size(); i++)
    {
        QListWidgetItem *item=new QListWidgetItem;
        QString sensorId=selectedIdList.at(i);
        item->setText(QString("[%1]%2").arg(sensorId).arg(m_meshModel->sensorsModel()->getNodeName(sensorId)));
        item->setData(Qt::UserRole, sensorId);
        ui->listBindingSensor->addItem(item);
    }
}

