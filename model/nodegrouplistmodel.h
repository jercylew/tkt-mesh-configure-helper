#ifndef NODEGROUPLISTMODEL_H
#define NODEGROUPLISTMODEL_H

#include <QObject>
#include <QList>
#include "utils/meshruntimeparameterbox.h"

class NodeGroup;
class NodeGroupsModel;
class MeshSavingBox;

class NodeGroupListModel : public QObject
{
    Q_OBJECT
public:
    explicit NodeGroupListModel(NodeGroupsModel *nodeGroupsModel, MeshSavingBox *savingBox, QObject *parent = 0);

    void init();

    int count();
    NodeGroup *at(int index);

    void setSearchString(const QString &searchString);
    QString searchString();

    MeshRuntimeParameterBox::NodeGroupOrderMethod orderMethod();
    void setOrderMethod(MeshRuntimeParameterBox::NodeGroupOrderMethod orderMethod);

    void refreshList();

private:
    QString m_searchString;
    QList<NodeGroup*> m_nodeGroupList;
    NodeGroupsModel *m_nodeGroupsModel;

    MeshSavingBox *m_savingBox;

signals:
    void dataChanged();
    void modelChanged();

public slots:
    void doNodeGroupsModelDataChanged();
    void doNodeGroupsModelModelChanged();
};

#endif // NODEGROUPLISTMODEL_H
