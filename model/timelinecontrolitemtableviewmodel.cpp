#include "timelinecontrolitemtableviewmodel.h"
#include <QDebug>
#include "meshmodel.h"
#include "utils/timelinecontrolmanager.h"
#include "domain/sensor.h"
#include "sensorsmodel.h"

TimeLineControlItemTableViewModel::TimeLineControlItemTableViewModel(MeshModel *meshModel, quint8 groupId, int policySubId, QObject *parent)
    : QAbstractTableModel(parent),
      m_meshModel(meshModel)
{
    m_groupId=groupId;
    m_policySubId=policySubId;
    m_timeLineControlPolicyModel=new TimeLineControlPolicyModel(meshModel, groupId, policySubId);
    connect(m_timeLineControlPolicyModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_timeLineControlPolicyModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int TimeLineControlItemTableViewModel::rowCount(const QModelIndex &parent) const
{
     return m_timeLineControlPolicyModel->count();
}

int TimeLineControlItemTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant TimeLineControlItemTableViewModel::data(const QModelIndex &index, int role) const
{
    TimeLineControlItem* item=m_timeLineControlPolicyModel->at(index.row());
    if(item==NULL)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
        if(index.column()==0)
        {
            return QString("%1:%2").arg(item->startMinute/60, 2, 10, QLatin1Char('0')).arg(item->startMinute%60, 2, 10, QLatin1Char('0'));
        }
        else if(index.column()==1)
        {
            if(item->controlType=="power")
            {
                return QString("[%1] %2%").arg(tr("Global.Power")).arg(item->powerTypePower);
            }
            else if(item->controlType=="motion_sensor")
            {
                Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(item->motionTypeBindSensorId);
                if(sensor!=NULL)
                {
                    return QString("[%1] %2")
                            .arg(tr("Global.MotionSensor"),
                                 m_meshModel->sensorsModel()->getNodeName(sensor->sensorId()));
                }
                else
                {
                    return QString("[%1] No Sensor!").arg(tr("Global.MotionSensor"));
                }
            }
            else if(item->controlType=="lux_sensor")
            {
                Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(item->luxTypeBindSensorId);
                if(sensor!=NULL)
                {
                    return QString("[%1] %2")
                            .arg(tr("Global.LuxSensor"), m_meshModel->sensorsModel()->getNodeName(sensor->sensorId()));
                }
                else
                {
                    return QString("[%1] No Sensor!").arg(tr("Global.LuxSensor"));
                }
            }
            else if(item->controlType=="gas_transducer")
            {
                Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(item->gasTrnsdcrTypeBindSensorId);
                if(sensor!=NULL)
                {
                    return QString("[%1] %2")
                            .arg(tr("Global.GasTransducer"),
                                 m_meshModel->sensorsModel()->getNodeName(sensor->sensorId()));
                }
                else
                {
                    return QString("[%1] No Sensor!").arg(tr("Global.GasTransducer"));
                }
            }
            else if(item->controlType=="composite_sensor")
            {
                if(item->compositeSensorTypeCmpExpr != "")
                {
                    QString strOutlineInfo = item->compositeSensorTypeCmpExpr.left(8)+"...";
                    return QString("[%1] %2")
                            .arg(tr("Global.CompositeSensor"),
                                 strOutlineInfo);
                }
                else
                {
                    return QString("[%1] No Sensor!").arg(tr("Global.CompositeSensor"));
                }
            }
            else if(item->controlType=="no_operation")
            {
                return tr("Global.NoOperation");
            }
        }
        break;
    case Qt::UserRole:
        return item->id;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant TimeLineControlItemTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        if(section==0)
        {
            return tr("TimeLineControlItemTableViewModel.Time");
        }
        else if(section==1)
        {
            return tr("TimeLineControlItemTableViewModel.Control");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TimeLineControlItemTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void TimeLineControlItemTableViewModel::init()
{
    this->m_timeLineControlPolicyModel->init();
}

TimeLineControlPolicyModel *TimeLineControlItemTableViewModel::timeLineControlPolicyModel()
{
    return this->m_timeLineControlPolicyModel;
}

void TimeLineControlItemTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void TimeLineControlItemTableViewModel::doDataChanged()
{
    int row= m_timeLineControlPolicyModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}

