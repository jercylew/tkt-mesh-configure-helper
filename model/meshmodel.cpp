#include "meshmodel.h"
#include "utils/remoteconfigmanager.h"

#include "model/executersmodel.h"
#include "model/sensorsmodel.h"
#include "model/nodegroupsmodel.h"
#include "model/executerlistviewmodel.h"
#include "model/sensorlistviewmodel.h"
#include "model/nodegrouplistviewmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include "utils/timelinecontrolmanager.h"
#include "utils/commandtaskmanager.h"
#include "model/controldatesmodel.h"
#include "utils/meshlogmanager.h"
#include "utils/remoteconfigsessionholder.h"
#include "model/preparemeshmodel.h"
#include "domain/mesh.h"
#include "utils/meshsavingbox.h"

MeshModel::MeshModel(PrepareMeshModel *prepareMeshModel, QObject *parent) : QObject(parent)
{
    this->m_prepareMeshModel=prepareMeshModel;

    m_executersModel=NULL;
    m_sensorsModel=NULL;
    m_nodeGroupsModel=NULL;
    m_dbManager=NULL;
    m_commandTaskManager=NULL;
    m_timeLineControlManager=NULL;
    m_controlDatesModel=NULL;
    m_meshLogManager=NULL;
    m_meshSavingBox=NULL;

    connect(this->dbManager()->dbCaller(), SIGNAL(executerNodesChanged(QMap<QString,Executer*>*)),
            this->executersModel(), SLOT(doSyncExecuterNodes(QMap<QString,Executer*>*)));
    connect(this->dbManager()->dbCaller(), SIGNAL(sensorNodesChanged(QMap<QString,Sensor*>*)),
            this->sensorsModel(), SLOT(doSyncSensorNodes(QMap<QString,Sensor*>*)));
    connect(this->dbManager()->dbCaller(), SIGNAL(nodeGroupsChanged(QList<NodeGroup*>*)),
            this->nodeGroupsModel(), SLOT(doNodeGroupsChanged(QList<NodeGroup*>*)));
    connect(this->dbManager()->dbCaller(), SIGNAL(controlDatesChanged(QList<ControlDate*>*)),
            this->controlDatesModel(), SLOT(doControlDatesChanged(QList<ControlDate*>*)));

    connect(this->prepareMeshModel()->remoteConfigManager()->remoteConfigSessionHolder(), SIGNAL(readADataFrame(QByteArray)),
            this, SLOT(doReadADataFrame(QByteArray)));
}

MeshModel::~MeshModel()
{
    if(m_executersModel!=NULL)
    {
        delete m_executersModel;
        m_executersModel=NULL;
    }
    if(m_sensorsModel!=NULL)
    {
        delete m_sensorsModel;
        m_sensorsModel=NULL;
    }
    if(m_nodeGroupsModel!=NULL)
    {
        delete m_nodeGroupsModel;
        m_nodeGroupsModel=NULL;
    }
    if(m_dbManager!=NULL)
    {
        delete m_dbManager;
        m_dbManager=NULL;
    }
    if(m_commandTaskManager!=NULL)
    {
        delete m_commandTaskManager;
        m_commandTaskManager=NULL;
    }
    if(m_timeLineControlManager!=NULL)
    {
        delete m_timeLineControlManager;
        m_timeLineControlManager=NULL;
    }
    if(m_controlDatesModel!=NULL)
    {
        delete m_controlDatesModel;
        m_controlDatesModel=NULL;
    }
    if(m_meshLogManager!=NULL)
    {
        delete m_meshLogManager;
        m_meshLogManager=NULL;
    }
}

void MeshModel::init()
{
    this->timeLineControlManager()->init();
    this->dbManager()->dbCaller()->syncNodeGroups();
    this->dbManager()->dbCaller()->syncControlPolicies();
    this->dbManager()->dbCaller()->syncControlItems();
    this->dbManager()->dbCaller()->syncExecuterNodes();
    this->dbManager()->dbCaller()->syncSensorNodes();
    this->dbManager()->dbCaller()->syncControlDates();
}

Mesh *MeshModel::mesh()
{
    return this->prepareMeshModel()->mesh();
}

ExecutersModel *MeshModel::executersModel()
{
    if(this->m_executersModel==NULL)
    {
        this->m_executersModel=new ExecutersModel(this);
    }
    return this->m_executersModel;
}

SensorsModel *MeshModel::sensorsModel()
{
    if(this->m_sensorsModel==NULL)
    {
        this->m_sensorsModel=new SensorsModel(this);
    }
    return this->m_sensorsModel;
}

NodeGroupsModel *MeshModel::nodeGroupsModel()
{
    if(this->m_nodeGroupsModel==NULL)
    {
        this->m_nodeGroupsModel=new NodeGroupsModel(this);
    }
    return this->m_nodeGroupsModel;
}

MeshDBManager *MeshModel::dbManager()
{
    if(this->m_dbManager==NULL)
    {
        this->m_dbManager=new MeshDBManager(this->prepareMeshModel()->mesh()->meshUUID());
    }
    return this->m_dbManager;
}

CommandTaskManager *MeshModel::commandTaskManager()
{
    if(this->m_commandTaskManager==NULL)
    {
        this->m_commandTaskManager=new CommandTaskManager(this);
    }
    return this->m_commandTaskManager;
}

TimeLineControlManager *MeshModel::timeLineControlManager()
{
    if(this->m_timeLineControlManager==NULL)
    {
        this->m_timeLineControlManager=new TimeLineControlManager(this, this->dbManager(), this->sensorsModel());
    }
    return this->m_timeLineControlManager;
}

ControlDatesModel *MeshModel::controlDatesModel()
{
    if(this->m_controlDatesModel==NULL)
    {
        this->m_controlDatesModel=new ControlDatesModel(this);
    }
    return this->m_controlDatesModel;
}

MeshLogManager *MeshModel::meshLogManager()
{
    if(this->m_meshLogManager==NULL)
    {
        this->m_meshLogManager=new MeshLogManager(this);
    }
    return this->m_meshLogManager;
}

MeshSavingBox *MeshModel::savingBox()
{
    if(this->m_meshSavingBox==NULL)
    {
        this->m_meshSavingBox=new MeshSavingBox(this->mesh()->meshUUID());
    }
    return this->m_meshSavingBox;
}

PrepareMeshModel *MeshModel::prepareMeshModel()
{
    return this->m_prepareMeshModel;
}

void MeshModel::doReadADataFrame(const QByteArray &dataFrame)
{
    bool parsed=this->executersModel()->parseADataFrame(dataFrame);
    if(!parsed)
    {
        this->sensorsModel()->parseADataFrame(dataFrame);
    }
}
