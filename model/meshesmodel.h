#ifndef MESHESMODEL_H
#define MESHESMODEL_H

#include <QObject>
#include <QMap>
#include <QList>

class Mesh;

class MeshesModel : public QObject
{
    Q_OBJECT
public:
    explicit MeshesModel(QObject *parent = 0);

    void updateMesh(Mesh *mesh);

    void setMeshList(const QList<Mesh*> &meshList);

    Mesh *getByUUID(const QString &uuid);

    QList<Mesh *> getAll();

private:
    QMap<QString, Mesh*> m_meshMap;
    QList<Mesh*> m_meshList;

signals:
    void modelChanged();
    void dataChanged();

public slots:
};

#endif // MESHESMODEL_H
