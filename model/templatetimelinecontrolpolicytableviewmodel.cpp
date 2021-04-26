#include "templatetimelinecontrolpolicytableviewmodel.h"
#include <QDebug>

TemplateTimeLineControlPolicyTableViewModel::TemplateTimeLineControlPolicyTableViewModel(MeshModel *meshModel, NodeGroup *group, QObject *parent) : QAbstractTableModel(parent)
{
    m_meshModel = meshModel;
    m_group = group;
    m_templateTimeLineControlPolicyListModel=new TemplateTimeLineControlPolicyListModel(m_meshModel, m_group, this);
    connect(m_templateTimeLineControlPolicyListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_templateTimeLineControlPolicyListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int TemplateTimeLineControlPolicyTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_templateTimeLineControlPolicyListModel->count();
}

int TemplateTimeLineControlPolicyTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TemplateTimeLineControlPolicyTableViewModel::data(const QModelIndex &index, int role) const
{
    TemplateTimeLineControlPolicy* itemCollection=m_templateTimeLineControlPolicyListModel->at(index.row());
    if(itemCollection==NULL)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
        if(index.column()==0)
        {
            return itemCollection->name;
        }
        break;
    case Qt::ToolTipRole:
        if(index.column()==0)
        {
            return itemCollection->name;
        }
        break;
    case Qt::UserRole:
        return itemCollection->id;
        break;
    case Qt::TextAlignmentRole:
        return (Qt::AlignLeft+Qt::AlignVCenter);
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant TemplateTimeLineControlPolicyTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        if(section==0)
        {
            return tr("Template");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TemplateTimeLineControlPolicyTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

TemplateTimeLineControlPolicyListModel *TemplateTimeLineControlPolicyTableViewModel::templateTimeLineControlPolicyListModel()
{
    return this->m_templateTimeLineControlPolicyListModel;
}

void TemplateTimeLineControlPolicyTableViewModel::doDataChanged()
{
    qDebug()<<"TemplateTimeLineControlPolicyTableViewModel::doModelChanged size="<<this->m_templateTimeLineControlPolicyListModel->count();
    int row=m_templateTimeLineControlPolicyListModel->count();
    qDebug()<<"row="<<row;
    emit dataChanged(this->index(0, 0), this->index(0, row-1));
}

void TemplateTimeLineControlPolicyTableViewModel::doModelChanged()
{
    qDebug()<<"TemplateTimeLineControlPolicyTableViewModel::doDataChanged size="<<this->m_templateTimeLineControlPolicyListModel->count();
    emit layoutChanged();
}
