#ifndef TKTMESHUPDATELISTMODEL_H
#define TKTMESHUPDATELISTMODEL_H


#include <QObject>

class TKTMeshesUpdateModel;
class TKTMesh;

class TKTMeshUpdateListModel : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshUpdateListModel(TKTMeshesUpdateModel *tktMeshesUpdateModel, QObject *parent = 0);

    TKTMesh *at(int index);
    int count();

    void refreshList();

private:
    TKTMeshesUpdateModel *m_tktMeshesUpdateModel;
    QList<TKTMesh*> m_tktMeshList;

signals:
    void dataChanged();
    void modelChanged();

public slots:
    void doDataChanged();
    void doModelChanged();
};
#endif // TKTMESHUPDATELISTMODEL_H
