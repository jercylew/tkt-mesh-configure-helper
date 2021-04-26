#include "virtualswitchsensoreditdialogframe.h"
#include "ui_virtualswitchsensoreditdialogframe.h"
#include "domain/realswitchsensor.h"
#include "domain/virtualswitchsensor.h"

VirtualSwitchSensorEditDialogFrame::VirtualSwitchSensorEditDialogFrame(MeshModel *meshModel, VirtualSwitchSensor *virtualSwitchSensor, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::VirtualSwitchSensorEditDialogFrame)
{
    ui->setupUi(this);
    this->m_meshModel=meshModel;
    this->m_virtualSwitchSensor=virtualSwitchSensor;

    ui->lineName->setText(virtualSwitchSensor->getName());
    ui->lineBindingSwitchSensorId->setText(virtualSwitchSensor->getBindingSwitchSensorId());
    QList<quint8> logicalGroupIdList=virtualSwitchSensor->getLogicGroupIDList();
    QString text;
    for(int i=0; i<logicalGroupIdList.size(); i++)
    {
        if(i==0)
        {
            text.push_back(QString("%1").arg(logicalGroupIdList.at(i)&0xFF));
        }
        else
        {
            text.push_back(QString(",%1").arg(logicalGroupIdList.at(i)&0xFF));
        }
    }
    ui->lineLogicGroupIDList->setText(text);
    loadControlTypes();
}

VirtualSwitchSensorEditDialogFrame::~VirtualSwitchSensorEditDialogFrame()
{
    delete ui;
}

void VirtualSwitchSensorEditDialogFrame::on_buttonDialogBottomDone_clicked()
{
    this->m_virtualSwitchSensor->setName(ui->lineName->text());
    this->m_virtualSwitchSensor->setBindingSwitchSensorId(ui->lineBindingSwitchSensorId->text());
    QStringList strs=ui->lineLogicGroupIDList->text().split(",");
    QList<quint8> logicalGroupIDList;
    for(int i=0; i<strs.size(); i++)
    {
        QString str=strs.at(i).trimmed();
        if(str.isEmpty())
        {
            continue;
        }
        logicalGroupIDList.push_back(str.toInt());
    }
    this->m_virtualSwitchSensor->setLogicGroupIDList(logicalGroupIDList);
    this->m_virtualSwitchSensor->setChannelControlTypeText(1, ui->comboBox_1->currentData().toString());
    this->m_virtualSwitchSensor->setChannelControlTypeText(2, ui->comboBox_2->currentData().toString());
    this->m_virtualSwitchSensor->setChannelControlTypeText(3, ui->comboBox_3->currentData().toString());
    this->m_virtualSwitchSensor->setChannelControlTypeText(4, ui->comboBox_4->currentData().toString());
    this->m_virtualSwitchSensor->setChannelControlTypeText(5, ui->comboBox_5->currentData().toString());
    this->m_virtualSwitchSensor->setChannelControlTypeText(6, ui->comboBox_6->currentData().toString());

    emit accept();
}

void VirtualSwitchSensorEditDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit reject();
}

void VirtualSwitchSensorEditDialogFrame::loadControlTypes()
{
    QList<QString> controlTypeList=RealSwitchSensor::getWitchControlTypeTextList();
    for(int i=0; i<controlTypeList.size(); i++)
    {
        QString controlType=controlTypeList.at(i);
        QString displayText=tr(QString("SwitchControlType.%1").arg(controlType).toLatin1().data());
        ui->comboBox_1->addItem(displayText, controlType);
        ui->comboBox_2->addItem(displayText, controlType);
        ui->comboBox_3->addItem(displayText, controlType);
        ui->comboBox_4->addItem(displayText, controlType);
        ui->comboBox_5->addItem(displayText, controlType);
        ui->comboBox_6->addItem(displayText, controlType);
    }
    ui->comboBox_1->setCurrentIndex(ui->comboBox_1->findData(this->m_virtualSwitchSensor->getChannelControlTypeText(1)));
    ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData(this->m_virtualSwitchSensor->getChannelControlTypeText(2)));
    ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findData(this->m_virtualSwitchSensor->getChannelControlTypeText(3)));
    ui->comboBox_4->setCurrentIndex(ui->comboBox_4->findData(this->m_virtualSwitchSensor->getChannelControlTypeText(4)));
    ui->comboBox_5->setCurrentIndex(ui->comboBox_5->findData(this->m_virtualSwitchSensor->getChannelControlTypeText(5)));
    ui->comboBox_6->setCurrentIndex(ui->comboBox_6->findData(this->m_virtualSwitchSensor->getChannelControlTypeText(6)));
}
