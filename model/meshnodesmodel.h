#ifndef MESHNODESMODEL_H
#define MESHNODESMODEL_H

#include <QObject>
#include <QMap>
#include <QList>

class MeshDeviceNode;

class MeshNodesModel : public QObject
{
    Q_OBJECT
public:
    explicit MeshNodesModel(QObject *parent = 0);

    MeshDeviceNode *getByNodeID(const QString &nodeId);
    void updateNode(MeshDeviceNode *node);
    void updateNodes(QList<MeshDeviceNode*> nodeList);

    QList<MeshDeviceNode*> getAll();

private:
    QMap<QString, MeshDeviceNode*> m_nodeMap;
    QList<MeshDeviceNode*> m_nodeList;

signals:
    void dataChanged();
    void modelChanged();

public slots:
};

#endif // MESHNODESMODEL_H
