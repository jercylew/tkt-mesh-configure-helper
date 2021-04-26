#ifndef TKTMESHLISTMODEL_H
#define TKTMESHLISTMODEL_H

#include <QObject>

class TKTMeshesModel;
class TKTMesh;

class TKTMeshListModel : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshListModel(TKTMeshesModel *tktMeshesModel, QObject *parent = 0);

    TKTMesh *at(int index);
    int count();

    void refreshList();

private:
    TKTMeshesModel *m_tktMeshesModel;
    QList<TKTMesh*> m_tktMeshList;

signals:
    void dataChanged();
    void modelChanged();

public slots:
    void doDataChanged();
    void doModelChanged();
};

#endif // TKTMESHLISTMODEL_H
