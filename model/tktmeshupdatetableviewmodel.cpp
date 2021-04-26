#include "tktmeshupdatetableviewmodel.h"

#include "model/tktmeshesupdatemodel.h"
#include "model/tktmeshupdatelistmodel.h"
#include "domain/tktmesh.h"

TKTMeshUpdateTableViewModel::TKTMeshUpdateTableViewModel(TKTMeshesUpdateModel *tktMeshesUpdateModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    this->m_tktMeshesUpdateModel=tktMeshesUpdateModel;
    m_tktMeshUpdateListModel=new TKTMeshUpdateListModel(this->m_tktMeshesUpdateModel, this);
    connect(m_tktMeshUpdateListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_tktMeshUpdateListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

int TKTMeshUpdateTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_tktMeshUpdateListModel->count();
}

int TKTMeshUpdateTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant TKTMeshUpdateTableViewModel::data(const QModelIndex &index, int role) const
{
    TKTMesh *tktMesh=m_tktMeshUpdateListModel->at(index.row());
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
            return tktMesh->hostName();
            break;
        case 2:
            return tktMesh->hostPeerIP();
            break;
        case 3:
            return tktMesh->platform();
            break;
        case 4:
            return tktMesh->updateDescription();
            break;
        case 5:
            return tktMesh->updateProgress();
        default:
            break;
        }
        break;
    case Qt::UserRole:
        return tktMesh->hostID();
        break;
    case Qt::TextAlignmentRole:
        switch (index.column()) {
        case 4:
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

QVariant TKTMeshUpdateTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return tr("ID");
            break;
        case 1:
            return "名称";
            break;
        case 2:
            return "通信IP";
            break;
        case 3:
            return "平台";
            break;
        case 4:
            return "升级信息";
            break;
        case 5:
            return "进度";
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TKTMeshUpdateTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void TKTMeshUpdateTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void TKTMeshUpdateTableViewModel::doDataChanged()
{
    int row=m_tktMeshUpdateListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
