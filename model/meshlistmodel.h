#ifndef MESHLISTMODEL_H
#define MESHLISTMODEL_H

#include <QObject>

class MeshesModel;
class Mesh;

class MeshListModel : public QObject
{
    Q_OBJECT
public:
    explicit MeshListModel(MeshesModel *meshesModel, QObject *parent = 0);
    int count();
    Mesh *at(int index);

private:
    MeshesModel *m_meshesModel;
    void refreshList();
    QList<Mesh*> m_meshList;

signals:
    void modelChanged();
    void dataChanged();

public slots:
    void doModelChanged();
};

#endif // MESHLISTMODEL_H
