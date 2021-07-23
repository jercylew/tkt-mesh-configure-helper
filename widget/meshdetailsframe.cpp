#include "meshdetailsframe.h"
#include "ui_meshdetailsframe.h"
#include "domain/mesh.h"
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDateTime>
#include "utils/tools.h"
#include <QMenu>
#include <QAction>
#include "commandmanager.h"
#include "utils/nofocuslineitemdelegate.h"
#include <QModelIndexList>
#include "widget/luminairedimmingdialog.h"
#include "widget/grouppropertiesdialog.h"
#include "widget/addnodegroupdialogframe.h"
#include "widget/uvsterilizerconfdialog.h"
#include <QScrollBar>
#include "widget/timelinecontrolitemseditdialogframe.h"
#include "dialogbuilder.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include "model/executerlistmodel.h"
#include "model/sensorlistmodel.h"
#include "model/meshmodel.h"
#include "model/executerlistviewmodel.h"
#include "model/sensorlistviewmodel.h"
#include "model/nodegrouplistviewmodel.h"
#include "model/nodegrouplistmodel.h"
#include "model/nodegroupsmodel.h"
#include <QHBoxLayout>
#include "utils/commandtaskmanager.h"
#include "utils/timelinecontrolmanager.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include "utils/meshsavingbox.h"
#include <QWheelEvent>
#include "virtualmotionsensoreditdialogframe.h"
#include "domain/virtualmotionsensor.h"
#include "widget/sensornumberdisplayframe.h"
#include "domain/virtualswitchsensor.h"
#include "domain/mesh.h"
#include "domain/temperaturehumiditysensor.h"
#include "delegate/nodegrouplistviewitemdelegate.h"
#include "domain/stresssensor.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "parser/sensorparser.h"
#include "objectfactory.h"
#include "domain/warninglight.h"
#include "domain/onechannelluminaire.h"
#include "domain/onechannelrelay.h"
#include "domain/drycontactrelay.h"
#include "domain/adboard.h"
#include "domain/refrgtemperaturehumiditysensor.h"
#include "domain/uvlightsensor.h"
#include "domain/handwashingsensor.h"
#include "domain/uvsterilizer.h"
#include "domain/ozonesensor.h"
#include "domain/formaldehydesensor.h"
#include "domain/flammablegassensor.h"
#include "domain/alarmloudspeaker.h"
#include "dialogbuilder.h"
#include "parsermanager.h"

#define MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE   8

MeshDetailsFrame::MeshDetailsFrame(MeshModel *meshModel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MeshDetailsFrame)
{
    ui->setupUi(this);

    m_mapExecuterType[ADBoard::staticTypeText()] = NodeGroup::ADBoardGroup;
    m_mapExecuterType[OneChannelLuminaire::staticTypeText()] = NodeGroup::LuminaireGroup;
    m_mapExecuterType[WarningLight::staticTypeText()] = NodeGroup::LuminaireGroup;
    m_mapExecuterType[OneChannelRelay::staticTypeText()] = NodeGroup::RelayGroup;
    m_mapExecuterType[DryContactRelay::staticTypeText()] = NodeGroup::RelayGroup;
    m_mapExecuterType[AlarmLoudspeaker::staticTypeText()] = NodeGroup::RelayGroup;

    m_meshModel=meshModel;
    m_sensorNumberCurrentPage=0;
    ui->labelMeshName->setText(m_meshModel->mesh()->meshName());

    ui->buttonMeshClose->setCursor(Qt::PointingHandCursor);
    ui->buttonMeshSave->setCursor(Qt::PointingHandCursor);

    ui->listExecuterBody->installEventFilter(this);
    ui->listSensor->installEventFilter(this);

    initMenus();

    m_executerListViewModel=new ExecuterListViewModel(m_meshModel, m_meshModel->executersModel(), this);
    ui->listExecuterBody->setModel(m_executerListViewModel);
    connect(ui->listExecuterBody->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(doLumListSelectionChanged(QItemSelection,QItemSelection)));
    connect(m_executerListViewModel->executerListModel(), SIGNAL(modelChanged()),
            ui->listExecuterBody, SLOT(reset()));
    connect(m_executerListViewModel->executerListModel(), SIGNAL(dataChanged()),
            ui->listExecuterBody, SLOT(update()));
    connect(m_executerListViewModel->executerListModel(), SIGNAL(modelChanged()),
            this, SLOT(doUpdateLumNumber()));

    ui->listExecuterBody->setViewMode(QListView::IconMode);
    ui->listExecuterBody->setResizeMode(QListView::Adjust);
    ui->listExecuterBody->setSelectionMode(QListView::ExtendedSelection);
    ui->listExecuterBody->setSpacing(10);
    ui->listExecuterBody->setWordWrap(true);

    m_sensorListViewModel=new SensorListViewModel(m_meshModel, m_meshModel->sensorsModel(), this);
    ui->listSensor->setModel(m_sensorListViewModel);
    connect(m_sensorListViewModel->sensorListModel(), SIGNAL(modelChanged()),
            ui->listSensor, SLOT(reset()));
    connect(m_sensorListViewModel->sensorListModel(), SIGNAL(dataChanged()),
            ui->listSensor, SLOT(update()));
    ui->listSensor->setViewMode(QListView::IconMode);
    ui->listSensor->setResizeMode(QListView::Adjust);
    ui->listSensor->setSelectionMode(QListView::ExtendedSelection);
    ui->listSensor->setSpacing(10);
    ui->listSensor->setWordWrap(true);

    ui->listNodeGroups->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listNodeGroups->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->listNodeGroups->setSelectionMode(QListView::SingleSelection);
    ui->listNodeGroups->setSpacing(0);

    m_nodeGroupListViewModel=new NodeGroupListViewModel(m_meshModel, m_meshModel->nodeGroupsModel(), this);
    m_nodeGroupListViewItemDelegate=new NodeGroupListViewItemDelegate(m_meshModel, this);
    ui->listNodeGroups->setItemDelegate(m_nodeGroupListViewItemDelegate);
    ui->listNodeGroups->setModel(m_nodeGroupListViewModel);
    ui->listNodeGroups->setAlternatingRowColors(true);

    connect(m_nodeGroupListViewModel->nodeGroupListModel(), SIGNAL(modelChanged()),
            ui->listNodeGroups, SLOT(reset()));
    connect(m_nodeGroupListViewModel->nodeGroupListModel(), SIGNAL(dataChanged()),
            ui->listNodeGroups, SLOT(update()));

    connect(m_meshModel->nodeGroupsModel(), SIGNAL(groupActionTriggered(bool,quint8)),
            this, SLOT(doGroupActionTriggered(bool,quint8)));

    connect(m_meshModel->nodeGroupsModel(), SIGNAL(groupModelChanged()),
            this, SLOT(doNodeGroupsModelChanged()));
    connect(m_meshModel->sensorsModel(), SIGNAL(sensorsNumChanged(Sensor::SensorType,int)),
            this, SLOT(doSensorNumChanged(Sensor::SensorType,int)));

    ui->buttonGroupsMappingSetup->setVisible(false);  //暂时隐藏


    connect(m_meshModel->sensorsModel(), SIGNAL(updateAllSensorNumberDisplay()), this, SLOT(doUpdateAllSensorNumberDisplay()));

    ui->buttonSensorNumberScrollUp->setCursor(Qt::PointingHandCursor);
    ui->buttonSensorNumberScrollDown->setCursor(Qt::PointingHandCursor);
    ui->buttonSensorNumberScrollUp->setToolTip(tr("Global.PageUp"));
    ui->buttonSensorNumberScrollDown->setToolTip(tr("Global.PageDown"));
    initSensorNumberDisplay();

    m_executerListViewModel->executerListModel()->refreshList();
    m_sensorListViewModel->sensorListModel()->refreshList();
    m_nodeGroupListViewModel->nodeGroupListModel()->refreshList();

    updateLumGroupCombox();
    doUpdateLumNumber();

    updateSensorNumberDisplay();

    ui->buttonExecutersMenu->setCursor(Qt::PointingHandCursor);
    ui->buttonSensorsMenu->setCursor(Qt::PointingHandCursor);
}

MeshDetailsFrame::~MeshDetailsFrame()
{
    delete ui;
}

void MeshDetailsFrame::on_buttonMeshClose_clicked()
{
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Quit"), tr("Are you sure to quit mesh?"));
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    emit backToTKTMeshPage();
}

void MeshDetailsFrame::on_buttonMeshSave_clicked()
{
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Save"), tr("Are you sure to save mesh?"));
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    QDialog *saveDialog=DialogBuilder::getInstance()->buildSaveMeshDBDataDialog(m_meshModel->prepareMeshModel());
    saveDialog->exec();
}

bool MeshDetailsFrame::eventFilter(QObject *watched, QEvent *event)
{
   if(watched==ui->listExecuterBody && event->type()==QEvent::Wheel)
    {
        if(QApplication::queryKeyboardModifiers() & Qt::ControlModifier)
        {
            QWheelEvent *wheelEv=static_cast<QWheelEvent *>(event);
            if(wheelEv->delta()>0)
            {
                m_executerListViewModel->executerListModel()->scaleUp();
            }
            else
            {
                m_executerListViewModel->executerListModel()->scaleDown();
            }
        }
    }
    else if(watched==ui->listSensor && event->type()==QEvent::Wheel)
    {
        if(QApplication::queryKeyboardModifiers() & Qt::ControlModifier)
        {
            QWheelEvent *wheelEv=static_cast<QWheelEvent *>(event);
            if(wheelEv->delta()>0)
            {
                m_sensorListViewModel->sensorListModel()->scaleUp();
            }
            else
            {
                m_sensorListViewModel->sensorListModel()->scaleDown();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MeshDetailsFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        menuOrder->setTitle(tr("MeshDetailsFrame.Order"));
        actionAllDimming->setText(tr("MeshDetailsFrame.AllDimming"));
        actionSpecialControl->setText(tr("Special Control"));
        actionAllOff->setText(tr("MeshDetailsFrame.AllOff"));
        actionAllOn->setText(tr("MeshDetailsFrame.AllOn"));
        actionDimming->setText(tr("MeshDetailsFrame.Dimming"));
        actionOff->setText(tr("MeshDetailsFrame.Off"));
        actionOn->setText(tr("MeshDetailsFrame.On"));
        actionRename->setText(tr("MeshDetailsFrame.Rename"));
        actionDeleteConfigData->setText(tr("Delete Configuration Data"));
        actionScheduleControlSettings->setText(tr("MeshDetailsFrame.ScheduleControlSettings"));
        actionAdd->setText(tr("Global.Add"));
        actionDelete->setText(tr("Global.Remove"));
        menuScheduleType->setTitle(tr("Global.ScheduleType"));
        actionScheduleByDate->setText(tr("Global.ScheduleByDate"));
        actionScheduleByOnDuty->setText(tr("Global.ScheduleByOnDuty"));
        actionScheduleByOffDuty->setText(tr("Global.ScheduleByOffDuty"));
        actionScheduleOff->setText(tr("Global.ScheduleOff"));
        actionAddVirtualMotionSensor->setText(tr("Global.AddVirtualMotionSensor"));
        actionAddVirtualSwitchSensor->setText(tr("Global.AddVirtualSwitchSensor"));
        actionEditVirtualSensor->setText(tr("Global.Edit"));
        actionDeleteVirtualSensor->setText(tr("Global.Remove"));
        actionEnableCameraAP->setText(tr("Global.EnableCameraAP"));
        actionOrderByID->setText(tr("MeshDetailsFrame.OrderByID"));
        actionOrderByName->setText(tr("MeshDetailsFrame.OrderByName"));
        actionOrderByOnlineStatus->setText(tr("MeshDetailsFrame.OrderByOnlineStatus"));
    }
}

void MeshDetailsFrame::initMenus()
{
    actionRename=new QAction(tr("MainWidget.Rename"));
    connect(actionRename, SIGNAL(triggered(bool)), this, SLOT(doActionRename()));
    actionDeleteConfigData=new QAction(tr("Delete Configuration Data"));
    connect(actionDeleteConfigData, SIGNAL(triggered(bool)), this, SLOT(doActionDeleteConfigData()));

    menuExecuters=new QMenu(ui->listExecuterBody);
    actionOn=new QAction(tr("MeshDetailsFrame.On"));
    connect(actionOn, SIGNAL(triggered(bool)), this, SLOT(doActionOn()));
    actionOff=new QAction(tr("MeshDetailsFrame.Off"));
    connect(actionOff, SIGNAL(triggered(bool)), this, SLOT(doActionOff()));
    actionAllOn=new QAction(tr("MeshDetailsFrame.AllOn"));
    connect(actionAllOn, SIGNAL(triggered(bool)), this, SLOT(doActionAllOn()));
    actionAllOff=new QAction(tr("MeshDetailsFrame.AllOff"));
    connect(actionAllOff, SIGNAL(triggered(bool)), this, SLOT(doActionAllOff()));
    actionDimming=new QAction(tr("MeshDetailsFrame.Dimming"));
    connect(actionDimming, SIGNAL(triggered(bool)), this, SLOT(doActionDimming()));
    actionAllDimming=new QAction(tr("MeshDetailsFrame.AllDimming"));
    connect(actionAllDimming, SIGNAL(triggered(bool)), this, SLOT(doActionAllDimming()));
    actionSpecialControl=new QAction(tr("Special Control"));
    connect(actionSpecialControl, SIGNAL(triggered(bool)), this, SLOT(doActionSpecialControl()));
    actionStartSterilize=new QAction(tr("Start Sterilizing"));
    connect(actionStartSterilize, SIGNAL(triggered(bool)), this, SLOT(doActionStartSterilizing()));
    actionWarningLightAlarmOn=new QAction(tr("Start Alarm"));
    connect(actionWarningLightAlarmOn, SIGNAL(triggered(bool)), this, SLOT(doActionWarningLighAlarmOn()));
    actionWarningLightAlarmOff=new QAction(tr("Stop Alarm"));
    connect(actionWarningLightAlarmOff, SIGNAL(triggered(bool)), this, SLOT(doActionWarningLighAlarmOff()));

    menuGrouping=new QMenu(tr("MeshDetailsFrame.Grouping"));

    menuOrder=new QMenu(tr("MeshDetailsFrame.Order"));
    menuExecuters->addMenu(menuOrder);
    actionOrderByID=new QAction(tr("MeshDetailsFrame.OrderByID"));
    actionOrderByID->setCheckable(true);
    connect(actionOrderByID, SIGNAL(triggered(bool)), this, SLOT(doActionOrderByID()));
    menuOrder->addAction(actionOrderByID);
    actionOrderByName=new QAction(tr("MeshDetailsFrame.OrderByName"));
    actionOrderByName->setCheckable(true);
    connect(actionOrderByName, SIGNAL(triggered(bool)), this, SLOT(doActionOrderByName()));
    menuOrder->addAction(actionOrderByName);
    actionOrderByOnlineStatus=new QAction(tr("MeshDetailsFrame.OrderByOnlineStatus"));
    actionOrderByOnlineStatus->setCheckable(true);
    connect(actionOrderByOnlineStatus, SIGNAL(triggered(bool)), this, SLOT(doActionOrderByOnlineStatus()));
    menuOrder->addAction(actionOrderByOnlineStatus);

    actionFindLum=new QAction(tr("MeshDetailsFrame.FindLum"));
    connect(actionFindLum, SIGNAL(triggered(bool)), this, SLOT(doActionFindLum()));
    menuExecuters->addAction(actionFindLum);

    menuSensors=new QMenu(ui->listSensor);
    menuSensors->addAction(actionRename);
    menuSensors->addMenu(menuOrder);
    actionAddVirtualMotionSensor=new QAction(tr("Global.AddVirtualMotionSensor"));
    connect(actionAddVirtualMotionSensor, SIGNAL(triggered(bool)), this, SLOT(doActionAddVirtualMotionSensorTriggered()));
    menuSensors->addAction(actionAddVirtualMotionSensor);
    actionAddVirtualSwitchSensor=new QAction(tr("Global.AddVirtualSwitchSensor"));
    connect(actionAddVirtualSwitchSensor, SIGNAL(triggered(bool)), this, SLOT(doActionAddVirtualSwitchSensorTriggered()));
    menuSensors->addAction(actionAddVirtualSwitchSensor);
    actionEditVirtualSensor=new QAction(tr("Global.Edit"));
    connect(actionEditVirtualSensor, SIGNAL(triggered(bool)), this, SLOT(doActionEditVirtualSensorTriggered()));
    menuSensors->addAction(actionEditVirtualSensor);
    actionDeleteVirtualSensor=new QAction(tr("Global.Remove"));
    connect(actionDeleteVirtualSensor, SIGNAL(triggered(bool)), this, SLOT(doActionDeleteVirtualSensorTriggered()));
    menuSensors->addAction(actionDeleteVirtualSensor);
    actionEnableCameraAP=new QAction(tr("Global.EnableCameraAP"));
    actionEnableCameraAP->setCheckable(true);
    connect(actionEnableCameraAP, SIGNAL(triggered(bool)), this, SLOT(doActionEnableCameraAPTriggered(bool)));
    menuSensors->addAction(actionEnableCameraAP);
    menuSensors->addAction(actionDeleteConfigData);
    actionAllocSwitchKeyGroup=new QAction(tr("Global.AllocSwitchKeyGroup"));
    connect(actionAllocSwitchKeyGroup, SIGNAL(triggered(bool)), this, SLOT(doActionAllocSwitchKeyGroup()));
    menuSensors->addAction(actionAllocSwitchKeyGroup);

    menuGroups=new QMenu(ui->listNodeGroups);
    actionAdd=new QAction(tr("Global.Add"));
    connect(actionAdd, SIGNAL(triggered(bool)), ui->buttonGroupMappingAdd, SLOT(click()));
    menuGroups->addAction(actionAdd);
    actionDelete=new QAction(tr("Global.Remove"));
    connect(actionDelete, SIGNAL(triggered(bool)), ui->buttonGroupMappingDelete, SLOT(click()));
    menuGroups->addAction(actionDelete);
    menuGroups->addAction(actionRename);
    menuGroups->addAction(actionDimming);
    menuGroups->addAction(actionSpecialControl);
    menuGroups->addAction(actionOn);
    menuGroups->addAction(actionOff);
    actionScheduleControlSettings=new QAction(tr("MeshDetailsFrame.ScheduleControlSettings"));
    connect(actionScheduleControlSettings, SIGNAL(triggered(bool)), this, SLOT(doActionScheduleControlSettings()));
    menuGroups->addAction(actionScheduleControlSettings);
    menuScheduleType=new QMenu(menuGroups);
    menuScheduleType->setTitle(tr("Global.ScheduleType"));
    menuGroups->addMenu(menuScheduleType);
    actionScheduleByDate=new QAction(tr("Global.ScheduleByDate"));
    actionScheduleByDate->setCheckable(true);
    connect(actionScheduleByDate, SIGNAL(triggered(bool)), this, SLOT(doActionScheduleByDateTriggered()));
    menuScheduleType->addAction(actionScheduleByDate);
    actionScheduleByOnDuty=new QAction(tr("Global.ScheduleByOnDuty"));
    actionScheduleByOnDuty->setCheckable(true);
    connect(actionScheduleByOnDuty, SIGNAL(triggered(bool)), this, SLOT(doActionScheduleByOnDutyTriggered()));
    menuScheduleType->addAction(actionScheduleByOnDuty);
    actionScheduleByOffDuty=new QAction(tr("Global.ScheduleByOffDuty"));
    actionScheduleByOffDuty->setCheckable(true);
    connect(actionScheduleByOffDuty, SIGNAL(triggered(bool)), this, SLOT(doActionScheduleByOffDutyTriggered()));
    menuScheduleType->addAction(actionScheduleByOffDuty);
    actionScheduleOff=new QAction(tr("Global.ScheduleOff"));
    actionScheduleOff->setCheckable(true);
    connect(actionScheduleOff, SIGNAL(triggered(bool)), this, SLOT(doActionScheduleOffTriggered()));
    menuScheduleType->addAction(actionScheduleOff);
    menuGroups->addMenu(menuOrder);

    m_menuOfExecutersCorner=new QMenu(ui->buttonExecutersMenu);
    m_actionShowOnlineExecuters=new QAction(tr("Show Only Online Executers"));
    m_actionShowOnlineExecuters->setCheckable(true);
    connect(m_actionShowOnlineExecuters, SIGNAL(triggered(bool)), this, SLOT(doActionShowOnlineExecutersTriggered(bool)));
    m_menuOfExecutersCorner->addAction(m_actionShowOnlineExecuters);
    m_actionShowAvailableExecuters=new QAction(tr("Show Only Available Executers"));
    m_actionShowAvailableExecuters->setCheckable(true);
    connect(m_actionShowAvailableExecuters, SIGNAL(triggered(bool)), this, SLOT(doActionShowAvailableExecutersTriggered(bool)));
    m_menuOfExecutersCorner->addAction(m_actionShowAvailableExecuters);

    m_menuOfSensorsCorner=new QMenu(ui->buttonSensorsMenu);
    m_actionShowAvailableSensors=new QAction(tr("Show Only Available Sensors"));
    m_actionShowAvailableSensors->setCheckable(true);
    connect(m_actionShowAvailableSensors, SIGNAL(triggered(bool)), this, SLOT(doActionShowAvailableSensorsTriggered(bool)));
    m_menuOfSensorsCorner->addAction(m_actionShowAvailableSensors);
}

void MeshDetailsFrame::initSensorNumberDisplay()
{
    SensorNumberDisplayFrame *luxSensorNumberDisplayFrame=new SensorNumberDisplayFrame("lux_sensor", "Global.LuxSensor");
    luxSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::LuxSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::LuxSensor, luxSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(luxSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *realMotionSensorNumberDisplayFrame=new SensorNumberDisplayFrame("real_motion_sensor", "Global.RealMotionSensor");
    realMotionSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::RealMotionSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::RealMotionSensor, realMotionSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(realMotionSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *virtualMotionSensorNumberDisplayFrame=new SensorNumberDisplayFrame("virtual_motion_sensor", "Global.VirtualMotionSensor");
    virtualMotionSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::VirtualMotionSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::VirtualMotionSensor, virtualMotionSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(virtualMotionSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *switchSensorNumberDisplayFrame=new SensorNumberDisplayFrame("real_switch_sensor", "Global.RealSwitchSensor");
    switchSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::RealSwitchSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::RealSwitchSensor, switchSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(switchSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *switchSensor2CHNumberDisplayFrame=new SensorNumberDisplayFrame("real_switch_sensor_2ch", "Global.RealSwitchSensor2CH");
    switchSensor2CHNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::RealSwitchSensor2CH));
    m_sensorNumberDisplayFrameMap.insert(Sensor::RealSwitchSensor2CH, switchSensor2CHNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(switchSensor2CHNumberDisplayFrame);

    SensorNumberDisplayFrame *virtualSwitchSensorNumberDisplayFrame=new SensorNumberDisplayFrame("virtual_switch_sensor", "Global.VirtualSwitchSensor");
    virtualSwitchSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::VirtualSwitchSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::VirtualSwitchSensor, virtualSwitchSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(virtualSwitchSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *virtualSwitchSensor2CHNumberDisplayFrame=new SensorNumberDisplayFrame("virtual_switch_sensor_2ch", "Global.VirtualSwitchSensor2CH");
    virtualSwitchSensor2CHNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::VirtualSwitchSensor2CH));
    m_sensorNumberDisplayFrameMap.insert(Sensor::VirtualSwitchSensor2CH, virtualSwitchSensor2CHNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(virtualSwitchSensor2CHNumberDisplayFrame);

    SensorNumberDisplayFrame *pmSensorNumberDisplayFrame=new SensorNumberDisplayFrame("pm_sensor", "Global.PMSensor");
    pmSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::PMSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::PMSensor, pmSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(pmSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *co1SensorNumberDisplayFrame=new SensorNumberDisplayFrame("co1_sensor", "Global.CO1Sensor");
    co1SensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::CO1Sensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::CO1Sensor, co1SensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(co1SensorNumberDisplayFrame);

    SensorNumberDisplayFrame *co2SensorNumberDisplayFrame=new SensorNumberDisplayFrame("co2_sensor", "Global.CO2Sensor");
    co2SensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::CO2Sensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::CO2Sensor, co2SensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(co2SensorNumberDisplayFrame);

    SensorNumberDisplayFrame *realMotionAndLuxCameraSensorNumberDisplayFrame=new SensorNumberDisplayFrame("real_motion_and_lux_camera_sensor", "Global.RealMotionAndLuxCameraSensor");
    realMotionAndLuxCameraSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::RealMotionAndLuxCameraSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::RealMotionAndLuxCameraSensor, realMotionAndLuxCameraSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(realMotionAndLuxCameraSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *tempHumSensorNumberDisplayFrame=new SensorNumberDisplayFrame(TemperatureHumiditySensor::staticTypeText(), "Global.TemperatureHumiditySensor");
    tempHumSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::TemperatureHumiditySensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::TemperatureHumiditySensor, tempHumSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(tempHumSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *stressSensorNumberDisplayFrame=new SensorNumberDisplayFrame(StressSensor::staticTypeText(), "Global.StressSensor");
    stressSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::StressSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::StressSensor, stressSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(stressSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *refrgTempHumSensorNumberDisplayFrame=new SensorNumberDisplayFrame(RefrgTemperatureHumiditySensor::staticTypeText(), "Global.RefrgTemperatureHumiditySensor");
    refrgTempHumSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::RefrgTemperatureHumiditySensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::RefrgTemperatureHumiditySensor, refrgTempHumSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(refrgTempHumSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *uvLightSensorNumberDisplayFrame=new SensorNumberDisplayFrame(UVLightSensor::staticTypeText(), "Global.UVLightSensor");
    uvLightSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::UVLightSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::UVLightSensor, uvLightSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(uvLightSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *handWashingSensorNumberDisplayFrame=new SensorNumberDisplayFrame(HandWashingSensor::staticTypeText(), "Global.HandWashingSensor");
    handWashingSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::HandWashingSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::HandWashingSensor, handWashingSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(handWashingSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *formaldehydeSensorNumberDisplayFrame=new SensorNumberDisplayFrame(FormaldehydeSensor::staticTypeText(), "Global.FormaldehydeSensor");
    formaldehydeSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::FormaldehydeSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::FormaldehydeSensor, formaldehydeSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(formaldehydeSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *ozoneSensorNumberDisplayFrame=new SensorNumberDisplayFrame(OzoneSensor::staticTypeText(), "Global.OzoneSensor");
    ozoneSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::OzoneSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::OzoneSensor, ozoneSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(ozoneSensorNumberDisplayFrame);

    SensorNumberDisplayFrame *flammableGasSensorNumberDisplayFrame=new SensorNumberDisplayFrame(FlammableGasSensor::staticTypeText(), "Global.FlammableGasSensor");
    flammableGasSensorNumberDisplayFrame->setNumber(m_meshModel->sensorsModel()->sensorNumber(Sensor::FlammableGasSensor));
    m_sensorNumberDisplayFrameMap.insert(Sensor::FlammableGasSensor, flammableGasSensorNumberDisplayFrame);
    m_allSensorNumberDisplayFrameList.push_back(flammableGasSensorNumberDisplayFrame);

    QHBoxLayout *layout=new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    ui->frameSensorNumberContainer->setStyleSheet("border:none;");
    ui->frameSensorNumberContainer->setLayout(layout);
    QSpacerItem *item=new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addItem(item);
}

void MeshDetailsFrame::updateLumGroupCombox()
{
    ui->combLumGroup->clear();
    QList<NodeGroup*> nodeGroups=m_meshModel->nodeGroupsModel()->getNodeGroupList();
    int len=nodeGroups.size();
    ui->combLumGroup->addItem(tr("MeshDetailsFrame.AllGroups"), 0);
    for(int i=0; i<len; i++)
    {
        ui->combLumGroup->addItem(nodeGroups.at(i)->getName(), nodeGroups.at(i)->getId()&0xFF);
    }
    ui->combLumGroup->addItem(tr("MeshDetailsFrame.Ungrouped"), 255);

    quint8 groupId=m_meshModel->savingBox()->runtimeParameterBox()->executerListViewCurrentGroupId();
    if(groupId==0 || groupId==255)
    {
        ui->combLumGroup->setCurrentIndex(ui->combLumGroup->findData(groupId&0xFF));
    }
    else
    {
        NodeGroup *nodeGroup=m_meshModel->nodeGroupsModel()->getNodeGroup(groupId);
        if(nodeGroup==NULL)
        {
            ui->combLumGroup->setCurrentIndex(ui->combLumGroup->findData(0));
        }
        else
        {
            ui->combLumGroup->setCurrentIndex(groupId&0xFF);
        }
    }
}

void MeshDetailsFrame::setExecuterListViewCurrentGroup(quint8 groupId)
{
    m_executerListViewModel->executerListModel()->setSelectedGroupId(groupId);
    ui->listExecuterBody->reset(); //因为模型变化了（数量等更新了），所以必须要用reset，而不应该用update
    doUpdateLumNumber();
}

void MeshDetailsFrame::updateSensorNumberDisplay()
{
    while(!m_currentSensorNumberDisplayFrameList.isEmpty())
    {
        SensorNumberDisplayFrame *frame=m_currentSensorNumberDisplayFrameList.takeFirst();
        frame->setVisible(false);
        ui->frameSensorNumberContainer->layout()->removeWidget(frame);
    }
    for(int i = m_sensorNumberCurrentPage*MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE;
        (i<(m_sensorNumberCurrentPage+1)*MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE &&
         i<m_allSensorNumberDisplayFrameList.size()); i++)
    {
        m_currentSensorNumberDisplayFrameList.push_back(m_allSensorNumberDisplayFrameList.at(i));
    }
    for(int i=0; i<m_currentSensorNumberDisplayFrameList.size(); i++)
    {
        SensorNumberDisplayFrame *frame=m_currentSensorNumberDisplayFrameList.at(i);
        ui->frameSensorNumberContainer->layout()->addWidget(frame);
        frame->setVisible(true);
    }
    int maxSensorNumberPage=getSensorNumberDisplayFrameMaxPage();
    if(m_sensorNumberCurrentPage>=maxSensorNumberPage)
    {
        ui->buttonSensorNumberScrollDown->setVisible(false);
    }
    else
    {
        ui->buttonSensorNumberScrollDown->setVisible(true);
    }
    if(m_sensorNumberCurrentPage<=0)
    {
        ui->buttonSensorNumberScrollUp->setVisible(false);
    }
    else
    {
        ui->buttonSensorNumberScrollUp->setVisible(true);
    }
    if(maxSensorNumberPage==0)
    {
        ui->frameSensorNumberScoller->setVisible(false);
    }
    else
    {
        ui->frameSensorNumberScoller->setVisible(true);
    }
}

int MeshDetailsFrame::getSensorNumberDisplayFrameMaxPage()
{
    int maxSensorNumberPage=0;
    if(m_allSensorNumberDisplayFrameList.size()<=MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE)
    {
        maxSensorNumberPage=0;
    }
    else
    {
        int remainder=m_allSensorNumberDisplayFrameList.size()%MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE;
        if(remainder==0)
        {
            maxSensorNumberPage=m_allSensorNumberDisplayFrameList.size()/MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE-1;
        }
        else
        {
            maxSensorNumberPage=m_allSensorNumberDisplayFrameList.size()/MAX_NUMBER_OF_SENSOR_NUMBER_DISPLAY_FRAME_PERPAGE;
        }
    }
    return maxSensorNumberPage;
}

bool MeshDetailsFrame::checkAndPopupManuallyControlWarning()
{
    return false;
}

void MeshDetailsFrame::setExecuterOrderCheckStates()
{
    actionOrderByOnlineStatus->setEnabled(true);
    actionOrderByID->setChecked(m_executerListViewModel->executerListModel()->orderMethod()
                                == MeshRuntimeParameterBox::ExecuterOrderByNodeID);
    actionOrderByName->setChecked(m_executerListViewModel->executerListModel()->orderMethod()
                                  == MeshRuntimeParameterBox::ExecuterOrderByNodeName);
    actionOrderByOnlineStatus->setChecked(m_executerListViewModel->executerListModel()->orderMethod()
                                          == MeshRuntimeParameterBox::ExecuterOrderByOnlineStatus);
}

void MeshDetailsFrame::setSensorOrderCheckStates()
{
    actionOrderByOnlineStatus->setEnabled(false);
    actionOrderByID->setChecked(m_sensorListViewModel->sensorListModel()->orderMethod()
                                == MeshRuntimeParameterBox::SensorOrderByNodeID);
    actionOrderByName->setChecked(m_sensorListViewModel->sensorListModel()->orderMethod()
                                  == MeshRuntimeParameterBox::SensorOrderByNodeName);
    actionOrderByOnlineStatus->setChecked(false);
}

void MeshDetailsFrame::fillGroupActions()
{
    menuGrouping->clear();
    if (!isSelectedSensorsDifferentType())
    {
        QString executerId = ui->listExecuterBody->currentIndex().data(Qt::UserRole).toString();
        Executer *e = m_meshModel->executersModel()->getExecuter(executerId);

        QList<QAction*> groupActionList=m_meshModel->nodeGroupsModel()->getNodeGroupActionList();
        for(int i=0; i<groupActionList.size(); i++)
        {
            QAction *groupAction=groupActionList.at(i);
            quint8 groupId=(quint8)(groupAction->property("group_id").toInt());

            NodeGroup *group = m_meshModel->nodeGroupsModel()->getNodeGroup(groupId);
            if (group != nullptr && group->getGroupType() == m_mapExecuterType[e->typeText()])
            {
                groupAction->setChecked(e->belongsToGroup(groupId));
                menuGrouping->addAction(groupAction);
            }
        }

        menuExecuters->addMenu(menuGrouping);
    }
}

void MeshDetailsFrame::fillExecuterMenus()
{
    QString executerId = ui->listExecuterBody->currentIndex().data(Qt::UserRole).toString();
    Executer *e = m_meshModel->executersModel()->getExecuter(executerId);
    if(e != nullptr)
    {
        if (e->typeText() == UVSterilizer::staticTypeText())
        {
            menuExecuters->addAction(actionStartSterilize);
        }
        else if (e->typeText() == WarningLight::staticTypeText())
        {
            menuExecuters->addAction(actionWarningLightAlarmOn);
            menuExecuters->addAction(actionWarningLightAlarmOff);
        }
        else
        {
            menuExecuters->addAction(actionOn);
            menuExecuters->addAction(actionOff);
        }

        if (e->isLuminaire())
        {
            menuExecuters->addAction(actionDimming);
            menuExecuters->addAction(actionFindLum);
        }
        if(e->hasSpecialControl())
        {
            menuExecuters->addAction(actionSpecialControl);
        }

        menuExecuters->addAction(actionRename);
        menuExecuters->addAction(actionDeleteConfigData);

        fillGroupActions();
    }
}

void MeshDetailsFrame::on_buttonRefreshIcon_clicked()
{
    m_meshModel->executersModel()->discover();
}

void MeshDetailsFrame::doActionOn()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        int len=list.size();
        for(int i=0; i<len; i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }
        m_meshModel->commandTaskManager()->runOnCommand(addrList);
    }
    else if(menuContext==ui->listNodeGroups)
    {
        QModelIndex index=ui->listNodeGroups->currentIndex();
        if(!index.isValid())
        {
            return;
        }
        NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
        if(group==NULL)
        {
            return;
        }
        m_meshModel->commandTaskManager()->runGroupAllOnCommand(group->getId());
    }
}

void MeshDetailsFrame::doActionOff()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        int len=list.size();
        for(int i=0; i<len; i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }
        m_meshModel->commandTaskManager()->runOffCommand(addrList);
    }
    else if(menuContext==ui->listNodeGroups)
    {
        QModelIndex index=ui->listNodeGroups->currentIndex();
        if(!index.isValid())
        {
            return;
        }
        NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
        if(group==NULL)
        {
            return;
        }
        m_meshModel->commandTaskManager()->runGroupAllOffCommand(group->getId());
    }
}

void MeshDetailsFrame::doActionAllOn()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    quint8 currentGroupId=m_executerListViewModel->executerListModel()->selectedGroupId();
    if(currentGroupId==0)
    {
        m_meshModel->commandTaskManager()->runAllOnCommand();
    }
    else
    {
        m_meshModel->commandTaskManager()->runGroupAllOnCommand(currentGroupId);
    }
}

void MeshDetailsFrame::doActionAllOff()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    quint8 currentGroupId=m_executerListViewModel->executerListModel()->selectedGroupId();
    if(currentGroupId==0)
    {
        m_meshModel->commandTaskManager()->runAllOffCommand();
    }
    else
    {
        m_meshModel->commandTaskManager()->runGroupAllOffCommand(currentGroupId);
    }
}

void MeshDetailsFrame::doActionDimming()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        int len=list.size();
        for(int i=0; i<len; i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }
        LuminaireDimmingDialog *dialog=new LuminaireDimmingDialog(m_meshModel,
                                                                  addrList,
                                                                  0,
                                                                  this);
        dialog->exec();
        dialog->deleteLater();
    }
    else if(menuContext==ui->listNodeGroups)
    {
        QModelIndex index=ui->listNodeGroups->currentIndex();
        if(!index.isValid())
        {
            return;
        }
        NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
        if(group==NULL)
        {
            return;
        }
        LuminaireDimmingDialog *dialog=new LuminaireDimmingDialog(m_meshModel, group->getId(), group->getBrightness(), this);
        dialog->exec();
        group->setBrightness(dialog->getCurrentBrightness());
        dialog->deleteLater();
    }
}

void MeshDetailsFrame::doActionAllDimming()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    quint8 currentGroupId=m_executerListViewModel->executerListModel()->selectedGroupId();

    int brightness=0;
    LuminaireDimmingDialog *dialog=new LuminaireDimmingDialog(m_meshModel,
                                                              currentGroupId,
                                                              brightness,
                                                              this);
    dialog->exec();
    dialog->deleteLater();
}

void MeshDetailsFrame::doActionSpecialControl()
{
    if(menuContext!=ui->listSensor)
    {
        if(checkAndPopupManuallyControlWarning())
        {
            return;
        }
    }
    if(menuContext==ui->listExecuterBody)
    {
        if(ui->listExecuterBody->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        else if(ui->listExecuterBody->selectionModel()->selectedIndexes().size()==1)
        {
            QString id=ui->listExecuterBody->currentIndex().data(Qt::UserRole).toString();
            Executer *executer=m_meshModel->executersModel()->getExecuterByExecuterId(id);
            if(executer==NULL)
            {
                return;
            }
            QDialog *dialog=DialogBuilder::getInstance()->buildExecuterSpecialConrolDialog(tr("Special Control Of %1").arg(tr(executer->displayEnglishTypeText().toLatin1().data())), m_meshModel, id, false);
            dialog->exec();
        }
    }
    else if(menuContext==ui->listNodeGroups)
    {
        if(ui->listNodeGroups->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        quint8 groupId=ui->listNodeGroups->currentIndex().data(Qt::UserRole).toInt();
        NodeGroup *nodeGroup=m_meshModel->nodeGroupsModel()->getNodeGroup(groupId);
        if(nodeGroup==NULL)
        {
            return;
        }
        QDialog *dialog=DialogBuilder::getInstance()->buildExecuterSpecialConrolDialog(tr("Group Special Control Of %1").arg(tr(NodeGroup::getTextByGroupType(nodeGroup->getGroupType()).toLatin1().data())), m_meshModel, groupId, true);
        dialog->exec();
    }
}

void MeshDetailsFrame::doActionAbout()
{
    QDialog *dialog=DialogBuilder::getInstance()->buildHelpAboutDialog(tr("MeshDetailsFrame.About"));
    dialog->exec();
}

void MeshDetailsFrame::doActionRename()
{
    if(menuContext==ui->listExecuterBody)
    {
        if(ui->listExecuterBody->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        else if(ui->listExecuterBody->selectionModel()->selectedIndexes().size()==1)
        {
            QString id=ui->listExecuterBody->currentIndex().data(Qt::UserRole).toString();
            QDialog *dialog=DialogBuilder::getInstance()->buildRenameOneDialog(m_meshModel, tr("MeshDetailsFrame.RenameLuminaries"), "light", id);
            dialog->exec();
        }
        else
        {
            QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
            QList<QString> idList;
            foreach (QModelIndex index, list) {
                idList.push_back(index.data(Qt::UserRole).toString());
            }
            QDialog *dialog=DialogBuilder::getInstance()->buildRenameDialog(m_meshModel, tr("MeshDetailsFrame.RenameLuminaries"), "light", idList);
            dialog->exec();
        }
    }
    else if(menuContext==ui->listSensor)
    {
        if(ui->listSensor->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        else if(ui->listSensor->selectionModel()->selectedIndexes().size()==1)
        {
            QString id=ui->listSensor->currentIndex().data(Qt::UserRole).toString();
            QDialog *dialog=DialogBuilder::getInstance()->buildRenameOneDialog(m_meshModel, tr("MeshDetailsFrame.RenameSensor"), "sensor", id);
            dialog->exec();
        }
        else
        {
            QModelIndexList list=ui->listSensor->selectionModel()->selectedIndexes();
            QList<QString> idList;
            foreach (QModelIndex index, list) {
                idList.push_back(index.data(Qt::UserRole).toString());
            }
            QDialog *dialog=DialogBuilder::getInstance()->buildRenameDialog(m_meshModel, tr("MeshDetailsFrame.RenameSensors"), "sensor", idList);
            dialog->exec();
        }
    }
    else if(menuContext==ui->listNodeGroups)
    {
        if(ui->listNodeGroups->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        else if(ui->listNodeGroups->selectionModel()->selectedIndexes().size()==1)
        {
            QString id=ui->listNodeGroups->currentIndex().data(Qt::UserRole).toString();
            QDialog *dialog=DialogBuilder::getInstance()->buildRenameOneDialog(m_meshModel, tr("MeshDetailsFrame.RenameGroup"), "group", id);
            dialog->exec();
        }
        else
        {
            QModelIndexList list=ui->listNodeGroups->selectionModel()->selectedIndexes();
            QList<QString> idList;
            foreach (QModelIndex index, list) {
                idList.push_back(index.data(Qt::UserRole).toString());
            }
            QDialog *dialog=DialogBuilder::getInstance()->buildRenameDialog(m_meshModel, tr("MeshDetailsFrame.RenameGroups"), "group", idList);
            dialog->exec();
        }
    }
}

void MeshDetailsFrame::doActionDeleteConfigData()
{
    if(menuContext==ui->listExecuterBody)
    {
        if(ui->listExecuterBody->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<QString> *idList=new QList<QString>;
        foreach (QModelIndex index, list) {
            idList->push_back(index.data(Qt::UserRole).toString());
        }
        QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Delete Configuration Data"),
                                                                          tr("Are you sure to delete the configuration data of selected executers?"));
        if(dialog->exec()!=QDialog::Accepted)
        {
            return;
        }
        m_meshModel->dbManager()->dbCaller()->deleteExecutersConfigData(idList);
    }
    else if(menuContext==ui->listSensor)
    {
        if(ui->listSensor->selectionModel()->selectedIndexes().size()==0)
        {
            return;
        }
        QModelIndexList list=ui->listSensor->selectionModel()->selectedIndexes();
        QList<QString> *idList=new QList<QString>;
        foreach (QModelIndex index, list) {
            idList->push_back(index.data(Qt::UserRole).toString());
        }
        QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Delete Configuration Data"),
                                                                          tr("Are you sure to delete the configuration data of selected sensors?"));
        if(dialog->exec()!=QDialog::Accepted)
        {
            return;
        }
        m_meshModel->dbManager()->dbCaller()->deleteSensorsConfigData(idList);
    }
}

void MeshDetailsFrame::doActionOrderByID()
{
    if(menuContext==ui->listExecuterBody)
    {
        m_executerListViewModel->executerListModel()->setOrderMethod(MeshRuntimeParameterBox::ExecuterOrderByNodeID);
    }
    else if(menuContext==ui->listSensor)
    {
        m_sensorListViewModel->sensorListModel()->setOrderMethod(MeshRuntimeParameterBox::SensorOrderByNodeID);
    }
    else if(menuContext==ui->listNodeGroups)
    {
        m_nodeGroupListViewModel->nodeGroupListModel()->setOrderMethod(MeshRuntimeParameterBox::NodeGroupOrderByGroupID);
    }
}

void MeshDetailsFrame::doActionOrderByName()
{
    if(menuContext==ui->listExecuterBody)
    {
        m_executerListViewModel->executerListModel()->setOrderMethod(MeshRuntimeParameterBox::ExecuterOrderByNodeName);
    }
    else if(menuContext==ui->listSensor)
    {
        m_sensorListViewModel->sensorListModel()->setOrderMethod(MeshRuntimeParameterBox::SensorOrderByNodeName);
    }
    else if(menuContext==ui->listNodeGroups)
    {
        m_nodeGroupListViewModel->nodeGroupListModel()->setOrderMethod(MeshRuntimeParameterBox::NodeGroupOrderByGroupName);
    }
}

void MeshDetailsFrame::doActionOrderByOnlineStatus()
{
    m_executerListViewModel->executerListModel()->setOrderMethod(MeshRuntimeParameterBox::ExecuterOrderByOnlineStatus);
}

void MeshDetailsFrame::doActionFindLum()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    QString executerId=ui->listExecuterBody->currentIndex().data(Qt::UserRole).toString();
    Executer *l=m_meshModel->executersModel()->getExecuter(executerId);
    if(l==NULL)
    {
        return;
    }
    m_meshModel->commandTaskManager()->runFindLumCommand(l->getBluetoothAddress());
}

void MeshDetailsFrame::doActionScheduleControlSettings()
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
    QDialog *dialog=DialogBuilder::getInstance()->buildTimeLineControlItemListsEditDialog(m_meshModel,
                                            group, tr("TimeLineControlItemListsEditDialogFrame.DialogTitle").arg(group->getName()));
    dialog->exec();
}

void MeshDetailsFrame::doActionStartSterilizing()
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        int len=list.size();
        for(int i=0; i<len; i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }

        UVSterilizerConfDialog *dialog=new UVSterilizerConfDialog(m_meshModel,
                                                                  addrList,
                                                                  this);

        dialog->exec();
        dialog->deleteLater();
    }
//    else if(menuContext==ui->listNodeGroups)
//    {
//        QModelIndex index=ui->listNodeGroups->currentIndex();
//        if(!index.isValid())
//        {
//            return;
//        }
//        NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
//        if(group==NULL)
//        {
//            return;
//        }
//        LuminaireDimmingDialog *dialog=new LuminaireDimmingDialog(m_meshModel, group->getId(), group->getBrightness(), this);
//        dialog->exec();
//        group->setBrightness(dialog->getCurrentBrightness());
//        dialog->deleteLater();
//    }
}

void MeshDetailsFrame::doActionWarningLighAlarmOn()
{
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        int len=list.size();
        for(int i=0; i<len; i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }
        m_meshModel->commandTaskManager()->runWarningLightAlarmOnCommand(addrList);
    }
}

void MeshDetailsFrame::doActionWarningLighAlarmOff()
{
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        int len=list.size();
        for(int i=0; i<len; i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }
        m_meshModel->commandTaskManager()->runWarningLightAlarmOffCommand(addrList);
    }
}

void MeshDetailsFrame::doActionAllocSwitchKeyGroup()
{
    QAction *ptrAction = static_cast<QAction *>(sender());
    if (ptrAction == nullptr)
    {
        return;
    }

    QString strSensorId = ptrAction->property("sensor_id").toString();
    QDialog *ptrDialog = DialogBuilder::getInstance()->buildAllocSwitchKeyGroupDialog(m_meshModel,
                                     strSensorId, tr("Global.AllocSwitchKeyGroup"));
    ptrDialog->exec();
}

void MeshDetailsFrame::on_buttonGroupMappingAdd_clicked()
{
    QDialog *dialog = DialogBuilder::getInstance()->buildAddNodeGroupDialog(m_meshModel, tr("MeshDetailsFrame.AddGroup"));
    dialog->exec();
}

bool MeshDetailsFrame::isSelectedExecutersDifferentType()
{
    QModelIndexList list = ui->listExecuterBody->selectionModel()->selectedIndexes();

    if (list.size() <= 1)
    {
        return false;
    }

    bool bRet = false;
    QString strTypeText = "";
    for (int i = 0; i < list.size();i++)
    {
        Executer *e = m_executerListViewModel->executerListModel()->at(list.at(i).row());

        if (strTypeText == "")
        {
            strTypeText = m_mapExecuterType[e->typeText()];
            continue;
        }

        if (strTypeText != m_mapExecuterType[e->typeText()])
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

bool MeshDetailsFrame::isSelectedSensorsDifferentType()
{
    QModelIndexList list = ui->listSensor->selectionModel()->selectedIndexes();

    if (list.size() <= 1)
    {
        return false;
    }

    bool bRet = false;
    int type = -1;
    for (int i = 0; i < list.size();i++)
    {
        Sensor *s = m_sensorListViewModel->sensorListModel()->at(list.at(i).row());

        if (s == nullptr)
        {
            continue;
        }

        if (type == -1)
        {
            type = static_cast<int>(s->sensorType());
            continue;
        }

        if (type != static_cast<int>(s->sensorType()))
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

bool MeshDetailsFrame::isGroupTypeMatch(quint8 groupId)
{
    QModelIndexList list = ui->listExecuterBody->selectionModel()->selectedIndexes();
    Executer *e = m_executerListViewModel->executerListModel()->at(list.at(0).row());
    int type = m_mapExecuterType[e->typeText()];
    NodeGroup *group = m_meshModel->nodeGroupsModel()->getNodeGroup(groupId);

    return (group != nullptr && group->getGroupType() == type);
}

void MeshDetailsFrame::doGroupActionTriggered(bool checked, quint8 groupId)
{
    if(checkAndPopupManuallyControlWarning())
    {
        return;
    }

    if (isSelectedExecutersDifferentType())
    {
        QMessageBox::critical(this, tr("MeshDetailsFrame.GroupingError"),
                              tr("MeshDetailsFrame.GroupingErrorMessageDiffType"));
        return;
    }

    if (!isGroupTypeMatch(groupId))
    {
        QMessageBox::critical(this, tr("MeshDetailsFrame.GroupingError"),
                              tr("MeshDetailsFrame.GroupingErrorMessageMatchType"));
        return;
    }

    qDebug() << "MeshDetailsFrame::doGroupActionTriggered, "
             << QString("checked=%1, groupdId=%2")
                .arg(checked)
                .arg((quint16)groupId);
    if(menuContext==ui->listExecuterBody)
    {
        QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();
        QList<quint16> addrList;
        for(int i=0; i<list.size(); i++)
        {
            Executer *l=m_executerListViewModel->executerListModel()->at(list.at(i).row());
            addrList.push_back(l->getBluetoothAddress());
        }
        if(checked)
        {
            m_meshModel->commandTaskManager()->runAddG1Command(addrList, groupId);
        }
        else
        {
            m_meshModel->commandTaskManager()->runDeleteG1Command(addrList, groupId);
        }
    }
}

void MeshDetailsFrame::on_listExecuterBody_customContextMenuRequested(const QPoint &pos)
{
    QModelIndexList list=ui->listExecuterBody->selectionModel()->selectedIndexes();

    menuExecuters->clear();
    if(list.size() == 0)
    {
        menuExecuters->addAction(actionAllOn);
        menuExecuters->addAction(actionAllOff);
        menuExecuters->addAction(actionAllDimming);
        menuExecuters->addMenu(menuOrder);
    }
    else
    {
        if (isSelectedExecutersDifferentType())
        {
            menuExecuters->addAction(actionRename);
            menuExecuters->addAction(actionDeleteConfigData);
            menuExecuters->addMenu(menuOrder);
        }
        else
        {
            fillExecuterMenus();
        }
    }

    setExecuterOrderCheckStates();
    menuContext = ui->listExecuterBody;
    menuExecuters->exec(ui->listExecuterBody->mapToGlobal(pos));
}

void MeshDetailsFrame::on_listSensor_customContextMenuRequested(const QPoint &pos)
{
    QModelIndexList list=ui->listSensor->selectionModel()->selectedIndexes();

    menuSensors->clear();

    if(list.size() == 0)
    {
        menuSensors->addAction(actionAddVirtualMotionSensor);
        menuSensors->addAction(actionAddVirtualSwitchSensor);
        menuSensors->addMenu(menuOrder);
    }
    else if (list.size() == 1)
    {
        Sensor *sensor = m_meshModel->sensorsModel()->getSensorBySensorId(list.at(0).data(Qt::UserRole).toString());

        if (sensor != nullptr)
        {
            if (sensor->isVirtual())
            {
                menuSensors->addAction(actionEditVirtualSensor);
                menuSensors->addAction(actionDeleteVirtualSensor);
            }

            if(sensor->hasSpecialControl())
            {
                menuSensors->addAction(actionSpecialControl);
            }

            menuSensors->addAction(actionRename);
            menuSensors->addAction(actionDeleteConfigData);

            if (sensor->sensorType() == Sensor::RealSwitchSensor
                    || sensor->sensorType() == Sensor::RealSwitchSensor2CH)
            {
                actionAllocSwitchKeyGroup->setProperty("sensor_id", sensor->sensorId());
                menuSensors->addAction(actionAllocSwitchKeyGroup);
            }
            else if(sensor->sensorType() == Sensor::RealMotionAndLuxCameraSensor)
            {
                menuSensors->addAction(actionEnableCameraAP);
                RealMotionAndLuxCameraSensor *camera = static_cast<RealMotionAndLuxCameraSensor*>(sensor);
                actionEnableCameraAP->setChecked(camera->apEnabled());
            }
            else
            {
            }
        }
        else
        {
            qWarning() << "MeshDetailsFrame::on_listSensor_customContextMenuRequested"
                       << "No such sensor!";
        }
    }
    else
    {
        menuSensors->addAction(actionRename);
        menuSensors->addAction(actionDeleteConfigData);
        menuSensors->addMenu(menuOrder);
    }

    setSensorOrderCheckStates();
    menuContext = ui->listSensor;
    menuSensors->exec(ui->listSensor->mapToGlobal(pos));
}

void MeshDetailsFrame::on_buttonGroupMappingDelete_clicked()
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
    if(group==NULL)
    {
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("MeshDetailsFrame.DeleteGroup"), tr("MeshDetailsFrame.DeleteGroupQuestion").arg(group->getName()));
    if(dialog->exec()==QDialog::Accepted)
    {
        m_meshModel->dbManager()->dbCaller()->removeNodeGroup(group->getId());
    }
}

void MeshDetailsFrame::on_listNodeGroups_doubleClicked(const QModelIndex &index)
{
    NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
    qDebug()<<"MeshDetailsFrame::on_listNodeGroups_doubleClicked, "<<QString("group=NULL is %1").arg(group==NULL);
    QDialog *dialog=DialogBuilder::getInstance()->buildTimeLineControlItemListsEditDialog(m_meshModel, group, tr("TimeLineControlItemListsEditDialogFrame.DialogTitle").arg(group->getName()));
    dialog->exec();
}

void MeshDetailsFrame::on_listExecuterBody_clicked(const QModelIndex &index)
{
    Executer *lum=m_executerListViewModel->executerListModel()->at(index.row());
    if(lum==NULL)
    {
        return;
    }
    ui->labelStatusTypeValue->setText(tr(lum->displayEnglishTypeText().toLatin1().data()));
    ui->labelStatusIDValue->setText(lum->executerId());
    ui->labelStatusNameValue->setText(lum->getName());
    ui->labelStatusDataValue->setText(lum->dataText());
}

void MeshDetailsFrame::on_combLumGroup_activated(int)
{
    quint8 groupId=(quint8)(ui->combLumGroup->currentData().toInt());
    setExecuterListViewCurrentGroup(groupId);
}

void MeshDetailsFrame::doNodeGroupsModelChanged()
{
    updateLumGroupCombox();
}

void MeshDetailsFrame::on_listNodeGroups_customContextMenuRequested(const QPoint &pos)
{
    QModelIndexList list=ui->listNodeGroups->selectionModel()->selectedIndexes();
    if(list.size()==0)
    {
        actionRename->setEnabled(false);
        actionOn->setEnabled(false);
        actionOff->setEnabled(false);
        actionDimming->setEnabled(false);
        menuScheduleType->setEnabled(false);
        actionScheduleControlSettings->setEnabled(false);
        actionSpecialControl->setVisible(false);
    }
    else
    {
        actionRename->setEnabled(true);
        actionOn->setEnabled(true);
        actionOff->setEnabled(true);
        actionDimming->setEnabled(true);
        actionScheduleControlSettings->setEnabled(true);

        QModelIndex index=ui->listNodeGroups->currentIndex();
        NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
        if(index.isValid() && (group!=NULL))
        {
            if(group->hasSpecialControl())
            {
                actionSpecialControl->setVisible(true);
            }
            else
            {
                actionSpecialControl->setVisible(false);
            }
            menuScheduleType->setEnabled(true);
            actionScheduleByDate->setChecked(false);
            actionScheduleByOnDuty->setChecked(false);
            actionScheduleByOffDuty->setChecked(false);
            actionScheduleOff->setChecked(false);

            switch (group->getScheduleType()) {
            case NodeGroup::ScheduleByDate:
                actionScheduleByDate->setChecked(true);
                break;
            case NodeGroup::ScheduleByOnDuty:
                actionScheduleByOnDuty->setChecked(true);
                break;
            case NodeGroup::ScheduleByOffDuty:
                actionScheduleByOffDuty->setChecked(true);
                break;
            case NodeGroup::ScheduleOff:
                actionScheduleOff->setChecked(true);
                break;
            default:
                break;
            }
        }
        else
        {
            menuScheduleType->setEnabled(false);
        }
    }
    actionOrderByOnlineStatus->setEnabled(false);
    actionOrderByOnlineStatus->setChecked(false);
    if(m_nodeGroupListViewModel->nodeGroupListModel()->orderMethod()==MeshRuntimeParameterBox::NodeGroupOrderByGroupID)
    {
        actionOrderByID->setChecked(true);
        actionOrderByName->setChecked(false);
    }
    else if(m_nodeGroupListViewModel->nodeGroupListModel()->orderMethod()==MeshRuntimeParameterBox::NodeGroupOrderByGroupName)
    {
        actionOrderByID->setChecked(false);
        actionOrderByName->setChecked(true);
    }
    menuContext=ui->listNodeGroups;
    menuGroups->exec(ui->listNodeGroups->mapToGlobal(pos));
}

void MeshDetailsFrame::doSensorNumChanged(Sensor::SensorType sensorType, int sensorNumber)
{
    SensorNumberDisplayFrame *displayFrame=m_sensorNumberDisplayFrameMap.value(sensorType);
    if(displayFrame!=NULL)
    {
        displayFrame->setNumber(sensorNumber);
    }
}

void MeshDetailsFrame::doLumListSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    doUpdateLumNumber();
}

void MeshDetailsFrame::doUpdateLumNumber()
{
    if(ui->listExecuterBody->selectionModel()->selectedIndexes().size()>0)
    {
        ui->labelLumNumber->setText(tr("Number Of Selected Executers: %1").arg(ui->listExecuterBody->selectionModel()->selectedIndexes().size()));
    }
    else
    {
        ui->labelLumNumber->setText(tr("Total Number In Selected Group: %1").arg(m_executerListViewModel->rowCount(QModelIndex())));
    }
}


void MeshDetailsFrame::on_lineSearch_textChanged(const QString &arg1)
{
    m_nodeGroupListViewModel->nodeGroupListModel()->setSearchString(ui->lineSearch->text().toLower().trimmed());
}

void MeshDetailsFrame::on_checkOnlyShowOnline_clicked(bool checked)
{

}

void MeshDetailsFrame::doActionGroupUseScheduleControlTriggered(bool checked)
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    NodeGroup *group=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
    if(group==NULL)
    {
        return;
    }

    if(checked)
    {
        m_meshModel->savingBox()->setGroupScheduleOn(group->getId(), true);
    }
    else
    {
        m_meshModel->savingBox()->setGroupScheduleOn(group->getId(), false);
    }

    ui->listNodeGroups->update();
}

void MeshDetailsFrame::doActionAddVirtualMotionSensorTriggered()
{
    VirtualMotionSensor *virtualMotionSensor=new VirtualMotionSensor;
    QDialog *dialog=DialogBuilder::getInstance()->buildVirtualSensorEditDialog(this->m_meshModel, virtualMotionSensor, tr("Global.AddVirtualMotionSensor"));
    if(dialog->exec()==QDialog::Accepted)
    {
        m_meshModel->sensorsModel()->addVirtualSensorToDB(virtualMotionSensor);
    }
    delete virtualMotionSensor;
}

void MeshDetailsFrame::doActionAddVirtualSwitchSensorTriggered()
{
    VirtualSwitchSensor *virtualSwitchSensor=new VirtualSwitchSensor;
    QDialog *dialog=DialogBuilder::getInstance()->buildVirtualSensorEditDialog(this->m_meshModel, virtualSwitchSensor, tr("Global.AddVirtualSwitchSensor"));
    if(dialog->exec()==QDialog::Accepted)
    {
        m_meshModel->sensorsModel()->addVirtualSensorToDB(virtualSwitchSensor);
    }
    delete virtualSwitchSensor;
}

void MeshDetailsFrame::doActionEditVirtualSensorTriggered()
{
    if(ui->listSensor->selectionModel()->selectedIndexes().size()==0)
    {
        return;
    }
    else if(ui->listSensor->selectionModel()->selectedIndexes().size()==1)
    {
        QString id=ui->listSensor->currentIndex().data(Qt::UserRole).toString();
        Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(id);
        if(sensor==NULL)
        {
            return;
        }
        if(sensor->isVirtual())
        {
            SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(sensor->sensorType());
            if(sensorParser==NULL)
            {
                return;
            }
            Sensor *copySensor=ObjectFactory::instance()->createSensorByTypeText(sensor->typeText());
            if(copySensor==NULL)
            {
                return;
            }
            copySensor->setSensorId(sensor->sensorId());
            sensorParser->updateSensorFromDB(m_meshModel, sensor, copySensor);
            QDialog *dialog=DialogBuilder::getInstance()->buildVirtualSensorEditDialog(this->m_meshModel, copySensor, QString("'%1' %2").arg(tr(sensor->displayEnglishTypeText().toLatin1().data())).arg(tr("Editor")));
            if(dialog!=NULL && dialog->exec()==QDialog::Accepted)
            {
                m_meshModel->sensorsModel()->updateVirtualSensorToDB(copySensor);
            }
            delete copySensor;
        }
    }
}

void MeshDetailsFrame::doActionDeleteVirtualSensorTriggered()
{
    if(ui->listSensor->selectionModel()->selectedIndexes().size()==0)
    {
        return;
    }
    else if(ui->listSensor->selectionModel()->selectedIndexes().size()==1)
    {
        QString id=ui->listSensor->currentIndex().data(Qt::UserRole).toString();
        QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Global.RemoveVirtualSensor"), tr("Global.RemoveQuestion"));
        if(dialog->exec()==QDialog::Accepted)
        {
            m_meshModel->sensorsModel()->deleteVirtualSensor(id);
        }
    }
}

void MeshDetailsFrame::doActionEnableCameraAPTriggered(bool checked)
{
    if(ui->listSensor->selectionModel()->selectedIndexes().size()==0)
    {
        return;
    }
    QString id=ui->listSensor->currentIndex().data(Qt::UserRole).toString();
    Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(id);
    if(sensor==NULL)
    {
        return;
    }
    if(checked)
    {
        if(DialogBuilder::getInstance()->buildQuestionDialog(tr("Global.EnableCameraAP"), tr("Global.EnableCameraAPQuestion"))->exec()==QDialog::Accepted)
        {
            m_meshModel->commandTaskManager()->runEnableCameraAP(sensor->getBluetoothAddress());
        }
    }
    else
    {
        if(DialogBuilder::getInstance()->buildQuestionDialog(tr("Global.DisableCameraAP"), tr("Global.DisableCameraAPQuestion"))->exec()==QDialog::Accepted)
        {
            m_meshModel->commandTaskManager()->runDisableCameraAP(sensor->getBluetoothAddress());
        }
    }
}

void MeshDetailsFrame::doActionScheduleByDateTriggered()
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(index.isValid())
    {
        quint8 groupId=(quint8)(index.data(Qt::UserRole).toInt());
        m_meshModel->nodeGroupsModel()->setGroupScheduleType(groupId, NodeGroup::ScheduleByDate);
    }
}

void MeshDetailsFrame::doActionScheduleByOnDutyTriggered()
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(index.isValid())
    {
        quint8 groupId=(quint8)(index.data(Qt::UserRole).toInt());
        m_meshModel->nodeGroupsModel()->setGroupScheduleType(groupId, NodeGroup::ScheduleByOnDuty);
    }
}

void MeshDetailsFrame::doActionScheduleByOffDutyTriggered()
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(index.isValid())
    {
        quint8 groupId=(quint8)(index.data(Qt::UserRole).toInt());
        m_meshModel->nodeGroupsModel()->setGroupScheduleType(groupId, NodeGroup::ScheduleByOffDuty);
    }
}

void MeshDetailsFrame::doActionScheduleOffTriggered()
{
    QModelIndex index=ui->listNodeGroups->currentIndex();
    if(index.isValid())
    {
        quint8 groupId=(quint8)(index.data(Qt::UserRole).toInt());
        m_meshModel->nodeGroupsModel()->setGroupScheduleType(groupId, NodeGroup::ScheduleOff);
    }
}

void MeshDetailsFrame::doActionShowOnlineExecutersTriggered(bool checked)
{
    m_executerListViewModel->executerListModel()->setOnlyShowOnlineNodes(checked);
}

void MeshDetailsFrame::doActionShowAvailableSensorsTriggered(bool checked)
{
    m_sensorListViewModel->sensorListModel()->setOnlyShowAvailableNodes(checked);
}

void MeshDetailsFrame::doActionShowAvailableExecutersTriggered(bool checked)
{
    m_executerListViewModel->executerListModel()->setOnlyShowAvailableNodes(checked);
}

void MeshDetailsFrame::on_buttonReset_clicked()
{
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Global.ResetMesh"), tr("Global.ResetMeshQuestion"));
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    m_meshModel->executersModel()->clear();
    m_meshModel->sensorsModel()->clear();
    m_meshModel->executersModel()->discover();
}

void MeshDetailsFrame::doUpdateAllSensorNumberDisplay()
{
    QList<Sensor::SensorType> sensorTypeList=m_meshModel->sensorsModel()->allNumberableSensorTypeList();
    for(int i=0; i<sensorTypeList.size(); i++)
    {
        Sensor::SensorType sensorType=sensorTypeList.at(i);
        SensorNumberDisplayFrame *frame = m_sensorNumberDisplayFrameMap.value(sensorType);
        if(frame)
        {
            frame->setNumber(m_meshModel->sensorsModel()->sensorNumber(sensorType));
        }
    }
}

void MeshDetailsFrame::on_buttonSensorNumberScrollUp_clicked()
{
    int nextSensorNumberPage=(m_sensorNumberCurrentPage-1);
    if(nextSensorNumberPage<0)
    {
        nextSensorNumberPage=0;
    }
    if(nextSensorNumberPage!=m_sensorNumberCurrentPage)
    {
        m_sensorNumberCurrentPage=nextSensorNumberPage;
        updateSensorNumberDisplay();
    }
}

void MeshDetailsFrame::on_buttonSensorNumberScrollDown_clicked()
{
    int nextSensorNumberPage=(m_sensorNumberCurrentPage+1);
    int maxSensorNumberPage=getSensorNumberDisplayFrameMaxPage();
    if(nextSensorNumberPage>maxSensorNumberPage)
    {
        nextSensorNumberPage=maxSensorNumberPage;
    }

    if(m_sensorNumberCurrentPage!=nextSensorNumberPage)
    {
        m_sensorNumberCurrentPage=nextSensorNumberPage;
        updateSensorNumberDisplay();
    }
}


void MeshDetailsFrame::on_buttonExecutersMenu_clicked()
{
    m_actionShowOnlineExecuters->setChecked(m_executerListViewModel->executerListModel()->onlyShowOnlineNodes());
    m_actionShowAvailableExecuters->setChecked(m_executerListViewModel->executerListModel()->onlyShowAvailableNodes());
    m_menuOfExecutersCorner->exec(ui->buttonExecutersMenu->mapToGlobal(QPoint(0, ui->buttonExecutersMenu->height())));
}

void MeshDetailsFrame::on_buttonSensorsMenu_clicked()
{
    m_actionShowAvailableSensors->setChecked(m_sensorListViewModel->sensorListModel()->onlyShowAvailableNodes());
    m_menuOfSensorsCorner->exec(ui->buttonSensorsMenu->mapToGlobal(QPoint(0, ui->buttonSensorsMenu->height())));
}

void MeshDetailsFrame::on_listSensor_clicked(const QModelIndex &index)
{
    Sensor *sensor=m_sensorListViewModel->sensorListModel()->at(index.row());
    if(sensor==NULL)
    {
        return;
    }
    ui->labelStatusTypeValue->setText(tr(sensor->displayEnglishTypeText().toLatin1().data()));
    ui->labelStatusIDValue->setText(sensor->sensorId());
    ui->labelStatusNameValue->setText(sensor->getName());
    ui->labelStatusDataValue->setText(sensor->dataText());
}
