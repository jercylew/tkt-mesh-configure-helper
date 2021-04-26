#include "meshnodetableviewmodel.h"

#include "model/meshnodesmodel.h"
#include "model/meshnodelistmodel.h"
#include "domain/meshdevicenode.h"

MeshNodeTableViewModel::MeshNodeTableViewModel(MeshNodesModel *meshNodesModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    this->m_meshNodesModel=meshNodesModel;
    m_meshNodeListModel=new MeshNodeListModel(this->m_meshNodesModel, this);
    connect(m_meshNodeListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_meshNodeListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int MeshNodeTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_meshNodeListModel->count();
}

int MeshNodeTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant MeshNodeTableViewModel::data(const QModelIndex &index, int role) const
{
    MeshDeviceNode *node=m_meshNodeListModel->at(index.row());
    if(node==NULL)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTip:
        switch (index.column()) {
        case 0:
            return node->nodeId();
            break;
        case 1:
            return node->bluetoothAddress()&0xFF;
            break;
        case 2:
            return node->typeText();
            break;
        case 3:
            return node->online()?"在线":"离线";
            break;
        case 4:
            return node->registered()?"已注册":"未注册";
            break;
        case 5:
        {
            QList<quint8> groupList=node->groupList();
            QString groupText;
            for(int i=0; i<groupList.size(); i++)
            {
                if(i>0)
                {
                    groupText.push_back(", ");
                }
                groupText.push_back(QString::number(groupList.at(i)&0xFF));
            }
            return groupText;
        }
            break;
        case 6:
        {
            return node->dataText();
        }
            break;
        default:
            break;
        }
        break;
    case Qt::UserRole:
        return node->nodeId();
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

QVariant MeshNodeTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return tr("ID");
            break;
        case 1:
            return "蓝牙地址";
            break;
        case 2:
            return "节点类型";
            break;
        case 3:
            return "在线状态";
            break;
        case 4:
            return "注册状态";
            break;
        case 5:
            return "分组信息";
            break;
        case 6:
            return "数据";
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags MeshNodeTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void MeshNodeTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void MeshNodeTableViewModel::doDataChanged()
{
    int row=m_meshNodeListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
