#ifndef NODEGROUPSMODEL_H
#define NODEGROUPSMODEL_H

#include <QObject>
#include "domain/nodegroup.h"
#include "widget/nodegroupframe.h"

class MeshModel;

class NodeGroupsModel : public QObject
{
    Q_OBJECT
public:
    explicit NodeGroupsModel(MeshModel *meshModel, QObject *parent = 0);
    void addGroup(quint8 groupId, const QString &groupName, const QColor &groupColor, NodeGroup::GroupType groupType);
    void removeGroup(quint8 groupId);
    int count();
    NodeGroup *at(int index);
    NodeGroup *getNodeGroup(quint8 groupId);
    QList<QColor> getColorList(const QList<quint8> groupList);
    QList<QAction*> getNodeGroupActionList();
    QList<NodeGroup*> getNodeGroupList();
    int countOfSearchString(const QString &searchString);
    NodeGroup *atOfSearchString(const QString &searchString, int index);
    QList<NodeGroup*> getAll();

    NodeGroup::ScheduleType getGroupScheduleType(quint8 groupId);
    void setGroupScheduleType(quint8 groupId, NodeGroup::ScheduleType scheduleType);
    void onExecuterGroupInfoChanged();

private:
    QMap<quint8, NodeGroup*> nodeGroupMap;
    QList<NodeGroup*> nodeGroupList;
    QList<QAction*> nodeGroupActionList;
    QMap<quint8, QAction*> nodeGroupActionMap;
    MeshModel *m_meshModel;

private slots:
    void doGroupActionTriggered(bool checked);
    void doNodeGroupsChanged(QList<NodeGroup*> *groupList);

signals:
    void groupModelChanged();
    void groupDataChanged();
    void groupActionTriggered(bool checked, quint8 groupId);

public slots:
};

#endif // NODEGROUPSMODEL_H
