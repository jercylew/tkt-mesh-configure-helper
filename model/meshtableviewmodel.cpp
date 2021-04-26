#include "meshtableviewmodel.h"

#include "model/meshesmodel.h"
#include "model/meshlistmodel.h"

#include "domain/mesh.h"

MeshTableViewModel::MeshTableViewModel(MeshesModel *meshesModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    this->m_meshesModel=meshesModel;
    this->m_meshListModel=new MeshListModel(this->m_meshesModel, this);
    connect(this->m_meshListModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
    connect(this->m_meshListModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
}

int MeshTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_meshListModel->count();
}

int MeshTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant MeshTableViewModel::data(const QModelIndex &index, int role) const
{
    Mesh *mesh=m_meshListModel->at(index.row());
    if(mesh==NULL)
    {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTip:
        switch (index.column()) {
        case 0:
            return mesh->meshName();
            break;
        case 1:
            return mesh->meshUUID();
            break;
        default:
            break;
        }
        break;
    case Qt::UserRole:
        return mesh->meshUUID();
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
        break;
    default:
        break;
    }

    return QVariant();
}

QVariant MeshTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return "Mesh名";
            break;
        case 1:
            return "UUID";
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags MeshTableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void MeshTableViewModel::doModelChanged()
{
    emit layoutChanged();
}

void MeshTableViewModel::doDataChanged()
{
    int row=m_meshListModel->count();
    emit dataChanged(this->index(0, 0), this->index(1, row-1));
}
