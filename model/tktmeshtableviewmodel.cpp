#include "tktmeshtableviewmodel.h"

#include "model/tktmeshesmodel.h"
#include "model/tktmeshlistmodel.h"
#include "domain/tktmesh.h"

TKTMeshTableViewModel::TKTMeshTableViewModel(TKTMeshesModel *tktMeshesModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    this->m_tktMeshesModel=tktMeshesModel;
    m_tktMeshListModel=new TKTMeshListModel(this->m_tktMeshesModel, this);
    connect(m_tktMeshListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_tktMeshListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int TKTMeshTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_tktMeshListModel->count();
}

int TKTMeshTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 11;
}

QVariant TKTMeshTableViewModel::data(const QModelIndex &index, int role) const
{
    TKTMesh *tktMesh=m_tktMeshListModel->at(index.row());
    if(tktMesh==NULL)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTip:
        switch (index.column()) {
        case 0:
            return tktMesh->hostID();
            break;
        case 1:
            return tktMesh->hostCode();
            break;
        case 2:
            return tktMesh->hostName();
            break;
        case 3:
            return tktMesh->hostDescription();
            break;
        case 4:
            return tktMesh->hostKey();
            break;
        case 5:
            return tktMesh->hostIP();
            break;
        case 6:
            return tktMesh->hostPeerIP();
            break;
        case 7:
            return tktMesh->hostHardVersion();
            break;
        case 8:
            return tktMesh->hostSoftVersion();
            break;
        case 9:
            return (tktMesh->hostRegistered()?"已注册":"未注册");
            break;
        case 10:
            return tktMesh->systemTime();
            break;
        default:
            break;
        }
        break;
    case Qt::UserRole:
        return tktMesh->hostID();
        break;
    case Qt::TextAlignmentRole:
        switch (index.column()) {
        case 3:
            return (int)(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        default:
            return Qt::AlignCenter;
        }
        break;
    default:
        break;
    }

    return QVariant();
}

QVariant TKTMeshTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return tr("ID");
            break;
        case 1:
            return tr("CODE");
            break;
        case 2:
            return "名称";
            break;
        case 3:
            return "描述";
            break;
        case 4:
            return "密钥";
            break;
        case 5:
            return "IP";
            break;
        case 6:
            return "通信IP";
            break;
        case 7:
            return "硬件版本";
            break;
        case 8:
            return "软件版本";
            break;
        case 9:
            return "注册信息";
            break;
        case 10:
            return "系统时间";
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TKTMeshTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void TKTMeshTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void TKTMeshTableViewModel::doDataChanged()
{
    int row=m_tktMeshListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
