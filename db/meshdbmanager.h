#ifndef MESHDBMANAGER_H
#define MESHDBMANAGER_H

#include <QObject>

class MeshDbCaller;

class MeshDBManager : public QObject
{
    Q_OBJECT
public:
    explicit MeshDBManager(const QString &meshUUID, QObject *parent = 0);
    ~MeshDBManager();

    MeshDbCaller *dbCaller();

private:
    MeshDbCaller *m_dbCaller;

signals:

public slots:
};

#endif // MESHDBMANAGER_H
