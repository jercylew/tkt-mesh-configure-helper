#ifndef MESHMODEL_H
#define MESHMODEL_H

#include <QObject>

class Mesh;
class RemoteConfigManager;
class TKTMeshModel;
class ExecutersModel;
class SensorsModel;
class NodeGroupsModel;
class ExecuterListViewModel;
class SensorListViewModel;
class NodeGroupListViewModel;
class ControlDatesModel;
class MeshDBManager;
class CommandTaskManager;
class TimeLineControlManager;
class MeshLogManager;
class MeshSavingBox;
class PrepareMeshModel;

typedef struct _SwitchMeshInfo_ {
    QString m_strMeshName;
    QString m_strMeshPassword;
    QString m_strMeshLTK;
} SwitchMeshInfo;

typedef struct _SensorDataComparision_ {
    QString m_strSensorId;
    QString m_strValue;     //If the sensor has only one data, this is not needed
    QString m_strOperator;  // > or <
    double m_dDataThresold;
} SensorDataComparision;

class MeshModel : public QObject
{
    Q_OBJECT
public:
    explicit MeshModel(PrepareMeshModel *prepareMeshModel, QObject *parent = 0);
    ~MeshModel();

    void init();

    Mesh *mesh();

    ExecutersModel *executersModel();
    SensorsModel *sensorsModel();
    NodeGroupsModel *nodeGroupsModel();
    MeshDBManager *dbManager();
    CommandTaskManager *commandTaskManager();
    TimeLineControlManager *timeLineControlManager();
    ControlDatesModel *controlDatesModel();
    MeshLogManager *meshLogManager();
    MeshSavingBox *savingBox();
    PrepareMeshModel *prepareMeshModel();

private:
    ExecutersModel *m_executersModel;
    SensorsModel *m_sensorsModel;
    NodeGroupsModel *m_nodeGroupsModel;
    MeshDBManager *m_dbManager;
    CommandTaskManager *m_commandTaskManager;
    TimeLineControlManager *m_timeLineControlManager;
    ControlDatesModel *m_controlDatesModel;
    MeshLogManager *m_meshLogManager;
    MeshSavingBox *m_meshSavingBox;
    PrepareMeshModel *m_prepareMeshModel;

private slots:
    void doReadADataFrame(const QByteArray &dataFrame);

signals:

public slots:
};

#endif // MESHMODEL_H
