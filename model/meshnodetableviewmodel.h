#ifndef MESHNODETABLEVIEWMODEL_H
#define MESHNODETABLEVIEWMODEL_H

#include <QAbstractTableModel>

class MeshNodesModel;
class MeshNodeListModel;

class MeshNodeTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MeshNodeTableViewModel(MeshNodesModel *meshNodesModel, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    MeshNodesModel *m_meshNodesModel;
    MeshNodeListModel *m_meshNodeListModel;

private slots:
    void doModelChanged();
    void doDataChanged();
};

#endif // MESHNODETABLEVIEWMODEL_H
