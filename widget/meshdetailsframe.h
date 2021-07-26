#ifndef MESHDETAILSFRAME_H
#define MESHDETAILSFRAME_H

#include <QFrame>
#include <QTimer>
#include <QList>
#include <QAction>
#include <QLabel>
#include <QDate>
#include <QItemSelection>
#include "domain/sensor.h"
#include "model/executerlistviewmodel.h"
#include "model/sensorlistviewmodel.h"
#include "model/nodegrouplistviewmodel.h"

#include <QFrame>

class MeshModel;
class MeshModel;
class SensorNumberDisplayFrame;
class NodeGroupListViewItemDelegate;

namespace Ui {
class MeshDetailsFrame;
}

class MeshDetailsFrame : public QFrame
{
    Q_OBJECT
public:
    explicit MeshDetailsFrame(MeshModel *meshModel, QWidget *parent = 0);
    ~MeshDetailsFrame();

private:
    bool checkAndPopupManuallyControlWarning();
    bool isSelectedExecutersDifferentType(QModelIndexList list = QModelIndexList());
    bool isSelectedSensorsDifferentType();
    bool isGroupTypeMatch(quint8 groupId, QModelIndexList list);
    void updateLumGroupCombox();
    void setExecuterListViewCurrentGroup(quint8 groupId);
    void initMenus();
    void initSensorNumberDisplay();
    void setExecuterOrderCheckStates();
    void setSensorOrderCheckStates();
    void fillGroupActions();
    void fillExecuterMenus();

    Ui::MeshDetailsFrame *ui;
    MeshModel *m_meshModel;

    QMenu *menuExecuters;
    QAction *actionOn;
    QAction *actionOff;
    QAction *actionAllOn;
    QAction *actionAllOff;
    QAction *actionDimming;
    QAction *actionAllDimming;
    QAction *actionSpecialControl;
    QAction *actionStartSterilize;
    QAction *actionWarningLightAlarmOn; //=Off
    QAction *actionWarningLightAlarmOff;   //=On
    QMenu *menuGrouping;

    QMenu *menuOrder;
    QAction *actionOrderByID;
    QAction *actionOrderByName;
    QAction *actionOrderByOnlineStatus;

    QAction *actionFindLum;

    QMenu *menuSensors;
    QAction *actionRename;
    QAction *actionDeleteConfigData;
    QAction *actionAddVirtualMotionSensor;
    QAction *actionAddVirtualSwitchSensor;
    QAction *actionEditVirtualSensor;
    QAction *actionDeleteVirtualSensor;
    QAction *actionEnableCameraAP;
    QAction *actionAllocSwitchKeyGroup;

    QMenu *menuGroups;
    QAction *actionScheduleControlSettings;
    QAction *actionAdd;
    QAction *actionDelete;
    QMenu *menuScheduleType;
    QAction *actionScheduleByDate;
    QAction *actionScheduleByOnDuty;
    QAction *actionScheduleByOffDuty;
    QAction *actionScheduleOff;

    QMenu *m_menuOfExecutersCorner;
    QAction *m_actionShowOnlineExecuters;
    QAction *m_actionShowAvailableExecuters;
    QMenu *m_menuOfSensorsCorner;
    QAction *m_actionShowAvailableSensors;

    QObject *menuContext;

    QMap<Sensor::SensorType, SensorNumberDisplayFrame*> m_sensorNumberDisplayFrameMap;
    QList<SensorNumberDisplayFrame*> m_allSensorNumberDisplayFrameList;
    QList<SensorNumberDisplayFrame*> m_currentSensorNumberDisplayFrameList;

    ExecuterListViewModel *m_executerListViewModel;
    SensorListViewModel *m_sensorListViewModel;
    NodeGroupListViewModel *m_nodeGroupListViewModel;

    NodeGroupListViewItemDelegate *m_nodeGroupListViewItemDelegate;

    int m_sensorNumberCurrentPage;
    void updateSensorNumberDisplay();
    int getSensorNumberDisplayFrameMaxPage();
    QMap<QString, int> m_mapExecuterType;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonRefreshIcon_clicked();
    void on_listExecuterBody_customContextMenuRequested(const QPoint &pos);
    void doActionOn();
    void doActionOff();
    void doActionAllOn();
    void doActionAllOff();
    void doActionDimming();
    void doActionAllDimming();
    void doActionSpecialControl();
    void doActionAbout();
    void doActionRename();
    void doActionDeleteConfigData();
    void doActionOrderByID();
    void doActionOrderByName();
    void doActionOrderByOnlineStatus();
    void doActionFindLum();
    void doActionScheduleControlSettings();
    void doActionStartSterilizing();

    void doActionWarningLighAlarmOn();
    void doActionWarningLighAlarmOff();

    void on_buttonGroupMappingAdd_clicked();

    void doGroupActionTriggered(bool checked, quint8 groupId);

    void on_listSensor_customContextMenuRequested(const QPoint &pos);

    void on_buttonGroupMappingDelete_clicked();

    void on_listNodeGroups_doubleClicked(const QModelIndex &index);

    void on_listExecuterBody_clicked(const QModelIndex &index);

    void on_combLumGroup_activated(int);

    void doNodeGroupsModelChanged();

    void on_listNodeGroups_customContextMenuRequested(const QPoint &pos);

    void doSensorNumChanged(Sensor::SensorType sensorType, int motionSensorNum);

    void doLumListSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

    void doUpdateLumNumber();

    void on_lineSearch_textChanged(const QString &arg1);

    void on_checkOnlyShowOnline_clicked(bool checked);

    void doActionGroupUseScheduleControlTriggered(bool checked);

    void doActionAddVirtualMotionSensorTriggered();

    void doActionAddVirtualSwitchSensorTriggered();

    void doActionEditVirtualSensorTriggered();

    void doActionDeleteVirtualSensorTriggered();

    void doActionEnableCameraAPTriggered(bool checked);

    void doActionScheduleByDateTriggered();
    void doActionScheduleByOnDutyTriggered();
    void doActionScheduleByOffDutyTriggered();
    void doActionScheduleOffTriggered();

    void doActionShowOnlineExecutersTriggered(bool checked);
    void doActionShowAvailableExecutersTriggered(bool checked);
    void doActionShowAvailableSensorsTriggered(bool checked);
    void doActionAllocSwitchKeyGroup();

    void on_buttonReset_clicked();
    void doUpdateAllSensorNumberDisplay();

    void on_buttonSensorNumberScrollUp_clicked();
    void on_buttonSensorNumberScrollDown_clicked();

    void on_buttonExecutersMenu_clicked();
    void on_buttonSensorsMenu_clicked();

    void on_listSensor_clicked(const QModelIndex &index);

    void on_buttonMeshClose_clicked();
    void on_buttonMeshSave_clicked();

signals:
    void backToTKTMeshPage();
};

#endif // MESHDETAILSFRAME_H
