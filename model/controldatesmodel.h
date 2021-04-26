#ifndef CONTROLDATESMODEL_H
#define CONTROLDATESMODEL_H

#include <QObject>
#include "domain/controldate.h"
#include <QMap>
#include <QList>

class MeshModel;

class ControlDatesModel : public QObject
{
    Q_OBJECT
public:
    explicit ControlDatesModel(MeshModel *meshModel, QObject *parent = 0);

    ControlDate *getById(const QString &id);
    int getTodayPolicyId(quint8 groupId);
    void setControlDates(int groupId, QList<ControlDate*> *controlDateList);

private:
    QMap<QString, ControlDate*> m_controlDateMap;
    QList<ControlDate*> m_controlDateList;
    MeshModel *m_meshModel;

signals:
    void controlDateModelChanged();

public slots:
    void doControlDatesChanged(QList<ControlDate*> *controlDateList);
};

#endif // CONTROLDATESMODEL_H
