#ifndef MESHNODELISTMODEL_H
#define MESHNODELISTMODEL_H

#include <QObject>

class MeshNodesModel;
class MeshDeviceNode;

class MeshNodeListModel : public QObject
{
    Q_OBJECT
public:
    explicit MeshNodeListModel(MeshNodesModel *meshNodesModel, QObject *parent = 0);

    MeshNodesModel *meshNodesModel();

    int count();

    MeshDeviceNode *at(int index);

    void refreshList();

private:
    MeshNodesModel *m_meshNodesModel;
    QList<MeshDeviceNode*> m_nodeList;

signals:
    void dataChanged();
    void modelChanged();

public slots:
    void doModelChanged();
};

#endif // MESHNODELISTMODEL_H
