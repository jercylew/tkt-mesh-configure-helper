#ifndef MESHDBHOLDER_H
#define MESHDBHOLDER_H

#include <QObject>
#include <QSqlDatabase>
#include "domain/initdbsql.h"

class MeshDBHolder : public QObject
{
    Q_OBJECT
public:
    explicit MeshDBHolder(const QString &meshUUID, QObject *parent = 0);
    ~MeshDBHolder();

    void initDB(bool initDbStructure=false);

    QSqlDatabase getDb();

private:

    QSqlDatabase db;

    QList<InitDbSql> getInitDbSqlList();

    QString getDbPath();

    QString m_meshUUID;

};

#endif // MESHDBHOLDER_H
