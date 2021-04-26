#include "allocswitchkeygroupframe.h"
#include "ui_allocswitchkeygroupframe.h"
#include "domain/sensor.h"
#include "model/meshmodel.h"
#include "model/meshlistmodel.h"
#include "model/sensorsmodel.h"
#include "model/nodegroupsmodel.h"
#include "utils/commandtaskmanager.h"
#include <QVariant>

AllocSwitchKeyGroupFrame::AllocSwitchKeyGroupFrame(MeshModel *meshModel, const QString &sensorId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AllocSwitchKeyGroupFrame)
{
    m_meshModel = meshModel;
    m_sensorId = sensorId;
    ui->setupUi(this);

    m_mapCtrlCmd["AllocSwitchKeyGroupFrame.NoOperation"] = AllocSwitchKeyGroupFrame::NoOperation;
    m_mapCtrlCmd["AllocSwitchKeyGroupFrame.Grouping"] = AllocSwitchKeyGroupFrame::Grouping;

    fillKeyData();
    setUpKeyFrame();
}

AllocSwitchKeyGroupFrame::~AllocSwitchKeyGroupFrame()
{
    delete ui;
}

void AllocSwitchKeyGroupFrame::setUpKeyFrame()
{
    Sensor *sensor = m_meshModel->sensorsModel()->getSensorBySensorId(m_sensorId);

    if (sensor != nullptr && sensor->sensorType() == Sensor::RealSwitchSensor2CH)
    {
        ui->gridLayout->removeWidget(ui->frameKey0);
        ui->gridLayout->removeWidget(ui->frameKey1);
        ui->gridLayout->removeWidget(ui->frameKey2);
        ui->gridLayout->removeWidget(ui->frameKey3);
        ui->gridLayout->removeWidget(ui->frameKey4);
        ui->gridLayout->removeWidget(ui->frameKey5);
        ui->gridLayout->removeWidget(ui->buttonCancel);
        ui->gridLayout->removeWidget(ui->buttonOk);
        ui->gridLayout->removeItem(ui->horSpacerRight);

        ui->frameKey4->setVisible(false);
        ui->frameKey5->setVisible(false);

        ui->gridLayout->addWidget(ui->frameKey0, 0, 1, 1, 1);
        ui->gridLayout->addWidget(ui->frameKey1, 0, 3, 1, 1);
        ui->gridLayout->addWidget(ui->frameKey2, 1, 1, 1, 1);
        ui->gridLayout->addWidget(ui->frameKey3, 1, 3, 1, 1);
        ui->gridLayout->addWidget(ui->buttonOk, 3, 1, 1, 1);
        ui->gridLayout->addWidget(ui->buttonCancel, 3, 3, 1, 1);

        resize(350, 398);
    }
}

void AllocSwitchKeyGroupFrame::fillKeyData()
{
    QList<NodeGroup*> mGroupList =  m_meshModel->nodeGroupsModel()->getAll();

    for (int i = 0;i < mGroupList.size();i++)
    {
        fillGroupingData(mGroupList.at(i)->getName(), mGroupList.at(i)->getId());
    }

    QMap<QString, KeyCmdType>::const_iterator itr = m_mapCtrlCmd.constBegin();
    while (itr != m_mapCtrlCmd.constEnd())
    {
        fillFuncData(tr(itr.key().toStdString().c_str()), itr.value());
        ++itr;
    }

    setDefaultFuncCmbox();
}

void AllocSwitchKeyGroupFrame::fillGroupingData(QString strGroupName, quint8 groupId)
{
    ui->cmbGroup0->addItem(strGroupName, groupId);
    ui->cmbGroup1->addItem(strGroupName, groupId);
    ui->cmbGroup2->addItem(strGroupName, groupId);
    ui->cmbGroup3->addItem(strGroupName, groupId);
    ui->cmbGroup4->addItem(strGroupName, groupId);
    ui->cmbGroup5->addItem(strGroupName, groupId);
}

void AllocSwitchKeyGroupFrame::fillFuncData(QString strFuncName, KeyCmdType cmdCode)
{
    ui->cmbFunc0->addItem(strFuncName, cmdCode);
    ui->cmbFunc1->addItem(strFuncName, cmdCode);
    ui->cmbFunc2->addItem(strFuncName, cmdCode);
    ui->cmbFunc3->addItem(strFuncName, cmdCode);
    ui->cmbFunc4->addItem(strFuncName, cmdCode);
    ui->cmbFunc5->addItem(strFuncName, cmdCode);
}

void AllocSwitchKeyGroupFrame::setDefaultFuncCmbox()
{
    ui->cmbFunc0->setCurrentIndex(ui->cmbFunc0->findData(NoOperation));
    ui->cmbFunc1->setCurrentIndex(ui->cmbFunc1->findData(NoOperation));
    ui->cmbFunc2->setCurrentIndex(ui->cmbFunc2->findData(NoOperation));
    ui->cmbFunc3->setCurrentIndex(ui->cmbFunc3->findData(NoOperation));
    ui->cmbFunc4->setCurrentIndex(ui->cmbFunc4->findData(NoOperation));
    ui->cmbFunc5->setCurrentIndex(ui->cmbFunc5->findData(NoOperation));
}

void AllocSwitchKeyGroupFrame::on_buttonOk_clicked()
{
    quint8 groupId;
    KeyCmdType cmd;

    if (ui->frameKey0->isVisible())
    {
        groupId = ui->cmbGroup0->currentData().toInt()&0xff;
        cmd = static_cast<KeyCmdType>(ui->cmbFunc0->currentData().toInt());

        switch (cmd)
        {
        case Grouping:
            m_meshModel->commandTaskManager()->runSwitchKeyGroupSetup(quint8(0), groupId);
            break;
        default:
            break;
        }
    }
    if (ui->frameKey1->isVisible())
    {
        groupId = ui->cmbGroup1->currentData().toInt()&0xff;
        cmd = static_cast<KeyCmdType>(ui->cmbFunc1->currentData().toInt());

        switch (cmd)
        {
        case Grouping:
            m_meshModel->commandTaskManager()->runSwitchKeyGroupSetup(quint8(1), groupId);
            break;
        default:
            break;
        }
    }
    if (ui->frameKey2->isVisible())
    {
        groupId = ui->cmbGroup2->currentData().toInt()&0xff;
        cmd = static_cast<KeyCmdType>(ui->cmbFunc2->currentData().toInt());

        switch (cmd)
        {
        case Grouping:
            m_meshModel->commandTaskManager()->runSwitchKeyGroupSetup(quint8(2), groupId);
            break;
        default:
            break;
        }
    }
    if (ui->frameKey3->isVisible())
    {
        groupId = ui->cmbGroup3->currentData().toInt()&0xff;
        cmd = static_cast<KeyCmdType>(ui->cmbFunc3->currentData().toInt());

        switch (cmd)
        {
        case Grouping:
            m_meshModel->commandTaskManager()->runSwitchKeyGroupSetup(quint8(3), groupId);
            break;
        default:
            break;
        }
    }
    if (ui->frameKey4->isVisible())
    {
        groupId = ui->cmbGroup4->currentData().toInt()&0xff;
        cmd = static_cast<KeyCmdType>(ui->cmbFunc4->currentData().toInt());

        switch (cmd)
        {
        case Grouping:
            m_meshModel->commandTaskManager()->runSwitchKeyGroupSetup(quint8(4), groupId);
            break;
        default:
            break;
        }
    }
    if (ui->frameKey5->isVisible())
    {
        groupId = ui->cmbGroup5->currentData().toInt()&0xff;
        cmd = static_cast<KeyCmdType>(ui->cmbFunc5->currentData().toInt());

        switch (cmd)
        {
        case Grouping:
            m_meshModel->commandTaskManager()->runSwitchKeyGroupSetup(quint8(5), groupId);
            break;
        default:
            break;
        }
    }

    emit accept();
}

void AllocSwitchKeyGroupFrame::on_buttonCancel_clicked()
{
    emit reject();
}
