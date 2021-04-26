#ifndef NODEGROUPLISTVIEWMODEL_H
#define NODEGROUPLISTVIEWMODEL_H

#include <QAbstractListModel>

class NodeGroupListModel;
class NodeGroupsModel;

class MeshModel;

class NodeGroupListViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    NodeGroupListViewModel(MeshModel *meshModel, NodeGroupsModel *nodeGroupsModel, QObject *parent=0);
    ~NodeGroupListViewModel();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    void setSearchString(const QString &searchString);

    QString getSearchString();

    NodeGroupListModel *nodeGroupListModel();

private:
    NodeGroupListModel *m_nodeGroupListModel;
    NodeGroupsModel *m_nodeGroupsModel;

    QString searchString;
    MeshModel *m_meshModel;
};

#endif // NODEGROUPLISTVIEWMODEL_H
