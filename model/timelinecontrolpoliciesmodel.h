#ifndef TIMELINECONTROLITEMLISTSMODEL_H
#define TIMELINECONTROLITEMLISTSMODEL_H

#include <QObject>
#include "model/meshmodel.h"
#include "domain/timelinecontrolpolicy.h"
#include <QList>
#include <QMap>

class TimeLineControlPoliciesModel : public QObject
{
    Q_OBJECT
public:
    explicit TimeLineControlPoliciesModel(MeshModel *meshModel, QObject *parent = 0);

    QList<TimeLineControlPolicy *> getSortedAllByGroupId(quint8 groupId);
    TimeLineControlPolicy *getById(int id);
    void updatePolicy(int controlPolicyId, const QString &name, const QString &color, const QString &description, int policySubId);

    int getNextSubId(quint8 groupId);

private:
    MeshModel *m_meshModel;
    QList<TimeLineControlPolicy*> m_controlPolicyList;
    QMap<int, TimeLineControlPolicy*> m_controlPolicyMap;

signals:
    void controlPoliciesModelChanged();
    void controlPoliciesDataChanged();

public slots:
    void doControlPoliciesChanged(QList<TimeLineControlPolicy*> *controlPolicyList);
    void doControlPolicyChanged(TimeLineControlPolicy *controlPolicy);
};

#endif // TIMELINECONTROLITEMLISTSMODEL_H
