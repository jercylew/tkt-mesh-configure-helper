#include "timelinecontrolpolicytableviewmodel.h"
#include <QDebug>
#include "languagemanager.h"
#include <QPixmap>

TimeLineControlPolicyTableViewModel::TimeLineControlPolicyTableViewModel(MeshModel *meshModel, quint8 groupId, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_meshModel=meshModel;
    m_groupId=groupId;
    m_timeLineControlPolicyListModel=new TimeLineControlPolicyListModel(meshModel, groupId);
    connect(m_timeLineControlPolicyListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_timeLineControlPolicyListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int TimeLineControlPolicyTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_timeLineControlPolicyListModel->count();
}

int TimeLineControlPolicyTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant TimeLineControlPolicyTableViewModel::data(const QModelIndex &index, int role) const
{
    TimeLineControlPolicy* controlPolicy=m_timeLineControlPolicyListModel->at(index.row());
    if(controlPolicy==NULL)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
        if(index.column()==0)
        {
            return controlPolicy->subId;
        }
        else if(index.column()==1)
        {
            return controlPolicy->name;
        }
        break;
    case Qt::DecorationRole:
        if(index.column()==2)
        {
            QPixmap pixmap(20, 20);
            pixmap.fill(QColor(controlPolicy->color));
            return pixmap;
        }
        break;
    case Qt::UserRole:
        return controlPolicy->id;
        break;

    case Qt::TextAlignmentRole:
        if(index.column()==0)
        {
            return Qt::AlignCenter;
        }
        else if(index.column()==1)
        {
            return (Qt::AlignLeft+Qt::AlignVCenter);
        }
        else if(index.column()==2)
        {
            return Qt::AlignCenter;
        }
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant TimeLineControlPolicyTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        if(section==0)
        {
            return tr("TimeLineControlPolicyTableViewModel.Number");
        }
        else if(section==1)
        {
            return tr("TimeLineControlPolicyTableViewModel.Name");
        }
        else if(section==2)
        {
            return tr("TimeLineControlPolicyTableViewModel.Color");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TimeLineControlPolicyTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsDragEnabled;
}

TimeLineControlPolicyListModel *TimeLineControlPolicyTableViewModel::timeLineControlPolicyListModel()
{
    return m_timeLineControlPolicyListModel;
}

void TimeLineControlPolicyTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void TimeLineControlPolicyTableViewModel::doDataChanged()
{
    int row= m_timeLineControlPolicyListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
