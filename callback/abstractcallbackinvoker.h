#ifndef ABSTRACTCALLBACKINVOKER_H
#define ABSTRACTCALLBACKINVOKER_H

#include <QJsonObject>
#include <QList>

class TKTMesh;
class Mesh;
class MeshDeviceNode;
class Luminaire;
class NodeGroup;
class MeshRuntimeParams;

class AbstractCallbackInvoker
{
public:
    AbstractCallbackInvoker();

    virtual void reloadHostInfo();
    virtual void loadMesh(Mesh *mesh);
    virtual void loadMeshList(const QList<Mesh*> &meshList);
    virtual void updateMeshNodes(QList<MeshDeviceNode*> *meshNodeList);
    virtual void loadLuminaire(Luminaire *luminaire);
    virtual void loadComList(const QList<QString> &comList);
    virtual void updateNodeGroups(const QList<NodeGroup*> &nodeGroupList);
    virtual void loadMeshRuntimeParams(MeshRuntimeParams *params);
    virtual void loadMeshLog(QList<QString> *meshLogList);
};

#endif // ABSTRACTCALLBACKINVOKER_H
