#include "sensordatacomparisiontableviewmodel.h"
#include "model/sensordatacomparisionlistmodel.h"

SensorDataComparisionTableViewModel::SensorDataComparisionTableViewModel(
        SensorDataComparisionListModel *dataCmpListModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_ptrSensorDataCmpListModel = dataCmpListModel;
    connect(dataCmpListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(dataCmpListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int SensorDataComparisionTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_ptrSensorDataCmpListModel->count();
}

int SensorDataComparisionTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant SensorDataComparisionTableViewModel::data(const QModelIndex &index, int role) const
{
    SensorDataComparision *dataCmp = m_ptrSensorDataCmpListModel->at(index.row());
    if(dataCmp == nullptr)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTip:
        switch (index.column()) {
        case 0:
            return dataCmp->m_strSensorId;
            break;
        case 1:
            return dataCmp->m_strValue;
            break;
        case 2:
            return dataCmp->m_strOperator;
            break;
        case 3:
            return dataCmp->m_dDataThresold;
            break;
        default:
            break;
        }
        break;
    case Qt::UserRole:
        return dataCmp->m_strSensorId;
        break;
    case Qt::TextAlignmentRole:
        switch (index.column()) {
        case 6:
            return (int)(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        default:
            return Qt::AlignCenter;
            break;
        }
        break;
    default:
        break;
    }

    return QVariant();
}

QVariant SensorDataComparisionTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return "绑定传感器ID";
            break;
        case 1:
            return "传感器值";
            break;
        case 2:
            return "比较关系";
            break;
        case 3:
            return "比较阈值";
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags SensorDataComparisionTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void SensorDataComparisionTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void SensorDataComparisionTableViewModel::doDataChanged()
{
    int row = m_ptrSensorDataCmpListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
