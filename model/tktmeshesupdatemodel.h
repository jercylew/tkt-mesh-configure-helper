#ifndef TKTMESHESUPDATEMODEL_H
#define TKTMESHESUPDATEMODEL_H

#include <QObject>

#include <QObject>
#include <QMap>
#include <QHostAddress>

class TKTMesh;
class TKTMeshUpdateDiscoverHelper;

class TKTMeshesUpdateModel : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshesUpdateModel(QObject *parent = 0);

    void updateTKTMesh(TKTMesh *tktMesh);

    QList<TKTMesh *> getAll();

    TKTMesh *getByID(const QString &hostId);

    void scanTKTMesh(const QHostAddress &address1, const QHostAddress &address2, int waitMsec, bool clearAll=false);

private:
    QMap<QString, TKTMesh*> m_tktMeshMap;
    QList<TKTMesh*> m_tktMeshList;
    TKTMeshUpdateDiscoverHelper *m_tktMeshUpdateDiscoverHelper;

signals:
    void dataChanged();
    void modelChanged();
    void scanProgress(int total, int finished);

public slots:
    void doDiscovered(TKTMesh *tktMesh);
};

#endif // TKTMESHESUPDATEMODEL_H
