#ifndef EXECUTERSMODEL_H
#define EXECUTERSMODEL_H

#include <QObject>
#include "domain/executer.h"
#include <QTimer>

class MeshModel;

class ExecutersModel : public QObject
{
    Q_OBJECT
public:
    explicit ExecutersModel(MeshModel *meshModel, QObject *parent = 0);

    void discover();
    int count();
    Executer* at(int index);
    Executer *getExecuter(const QString &id);
    Executer *getExecuterByExecuterId(const QString &executerId);
    void setGroupList(quint8 addr, QList<quint8> groupList);

    int countOfGroup(quint8 groupId);

    QString getNodeName(const QString &id);
    int getBrightnessValue(const QString &id);

    QList<Executer *> getAll();
    void clear();

    bool contains(const QString &executerId);
    void deleteExecutersConfigData(QList<QString> *executerIdList);

    bool parseADataFrame(const QByteArray &dataFrame);

private:
    QList<Executer *> m_executerList;
    QMap<QString, Executer *> m_executerMap;

    void updateExecuterByDBReason(Executer *dbExecuter);
    void updateExecuterByOnlineStatusReason(Executer *executer);
    void updateExecuterByUserDataNotifyReason(Executer *executer);
    void updateReferredExecuterDataAndNotify(Executer *executer);

    int nodeIndex;
    MeshModel *m_meshModel;
    QTimer m_checkOnlineStatusChangeTimer;

private slots:
    void doCheckOnlineStatusChange();

signals:
    void modelChanged();
    void dataChanged();
    void bluetoothNodeChanged(BluetoothNode *bluetoothNode);

public slots:
    void doSyncExecuterNodes(QMap<QString, Executer*> *executerMap);

};

#endif // EXECUTERSMODEL_H
