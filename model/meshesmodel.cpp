#include "meshesmodel.h"
#include "domain/mesh.h"
#include <QDebug>

MeshesModel::MeshesModel(QObject *parent) : QObject(parent)
{

}

void MeshesModel::updateMesh(Mesh *mesh)
{
    qDebug()<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    Mesh *oldMesh=m_meshMap.value(mesh->meshUUID());
    if(oldMesh==NULL)
    {
        oldMesh=new Mesh;
        m_meshMap.insert(mesh->meshUUID(), oldMesh);
        m_meshList.push_back(oldMesh);
    }
    oldMesh->load(mesh);
    emit modelChanged();
    delete mesh;
}

void MeshesModel::setMeshList(const QList<Mesh *> &meshList)
{
    qDebug()<<"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    while(!m_meshList.isEmpty())
    {
        delete m_meshList.takeFirst();
    }
    m_meshMap.clear();
    for(int i=0; i<meshList.size(); i++)
    {
        Mesh *mesh=meshList.at(i);
        Mesh *oldMesh=m_meshMap.value(mesh->meshUUID());
        if(oldMesh==NULL)
        {
            oldMesh=new Mesh;
            m_meshMap.insert(mesh->meshUUID(), oldMesh);
            m_meshList.push_back(oldMesh);
        }
        oldMesh->load(mesh);
        delete mesh;
    }
    emit modelChanged();
}

Mesh *MeshesModel::getByUUID(const QString &uuid)
{
    return this->m_meshMap.value(uuid);
}

QList<Mesh *> MeshesModel::getAll()
{
    return this->m_meshList;
}
