#include "templatetimelinecontrolitemtableviewmodel.h"
#include "model/meshmodel.h"

TemplateTimeLineControlItemTableViewModel::TemplateTimeLineControlItemTableViewModel(MeshModel *meshModel, const QString &policyId, QObject *parent) : QAbstractTableModel(parent)
{
    m_meshModel = meshModel;
    m_templateTimeLineControlItemListModel=new TemplateTimeLineControlItemListModel(m_meshModel, policyId, this);
    connect(m_templateTimeLineControlItemListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_templateTimeLineControlItemListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int TemplateTimeLineControlItemTableViewModel::rowCount(const QModelIndex &parent) const
{
    return this->m_templateTimeLineControlItemListModel->count();
}

int TemplateTimeLineControlItemTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant TemplateTimeLineControlItemTableViewModel::data(const QModelIndex &index, int role) const
{
    TimeLineControlItem* item=m_templateTimeLineControlItemListModel->at(index.row());
    if(item == nullptr)
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
                return QString("[%1] %2%").arg(tr("Power")).arg(item->powerTypePower);
            }
            else if(item->controlType=="motion_sensor")
            {
                return QString("[%1] %2").arg(tr("Motion Sensor")).arg(item->motionTypeBindSensorId);
            }
            else if(item->controlType=="lux_sensor")
            {
                return QString("[%1] %2").arg(tr("Lux Sensor")).arg(item->luxTypeBindSensorId);
            }
            else if(item->controlType=="gas_transducer")
            {
                return QString("[%1] %2").arg(tr("Gas Transducer")).arg(item->gasTrnsdcrTypeBindSensorId);
            }
            else if(item->controlType=="no_operation")
            {
                return tr("No Operation");
            }
        }
        break;
    case Qt::UserRole:
        return item->templateItemId;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant TemplateTimeLineControlItemTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        if(section==0)
        {
            return tr("Time");
        }
        else if(section==1)
        {
            return tr("Control");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TemplateTimeLineControlItemTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

TemplateTimeLineControlItemListModel *TemplateTimeLineControlItemTableViewModel::templateTimeLineControlItemListModel()
{
    return this->m_templateTimeLineControlItemListModel;
}

void TemplateTimeLineControlItemTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void TemplateTimeLineControlItemTableViewModel::doDataChanged()
{
    int row= m_templateTimeLineControlItemListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
