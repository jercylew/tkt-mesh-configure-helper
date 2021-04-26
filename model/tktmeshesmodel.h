#ifndef TKTMESHESMODEL_H
#define TKTMESHESMODEL_H

#include <QObject>
#include <QMap>
#include <QHostAddress>

class TKTMesh;
class TKTMeshDiscoverHelper;

class TKTMeshesModel : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshesModel(QObject *parent = 0);

    void updateTKTMesh(TKTMesh *tktMesh);

    QList<TKTMesh *> getAll();

    TKTMesh *getByID(const QString &hostId);

    void scanTKTMesh(const QHostAddress &address1, const QHostAddress &address2, int waitMsec, bool clearAll=false);

private:
    QMap<QString, TKTMesh*> m_tktMeshMap;
    QList<TKTMesh*> m_tktMeshList;
    TKTMeshDiscoverHelper *m_tktMeshDiscoverHelper;

signals:
    void dataChanged();
    void modelChanged();
    void scanProgress(int total, int finished);

public slots:
    void doDiscovered(TKTMesh *tktMesh);
};

#endif // TKTMESHESMODEL_H
