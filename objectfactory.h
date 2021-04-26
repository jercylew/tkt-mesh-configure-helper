#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <QObject>
#include <QMutex>
#include "domain/sensor.h"
#include "domain/executer.h"
#include "domain/timelinecontrolitem.h"
#include "domain/nodegroup.h"

class ObjectFactory : public QObject
{
    Q_OBJECT
public:
    static ObjectFactory *instance();
    bool isSensorNodeTypeCode(quint8 nodeTypeCode);
    bool isSensorUserDataTypeCode(quint8 userDataTypeCode);
    bool isExecuterNodeTypeCode(quint8 nodeTypeCode);
    bool isExecuterUserDataTypeCode(quint8 userDataTypeCode);

    Executer *createExecuterByTypeText(const QString &typeText);
    Executer *createExecuterByNodeTypeCode(quint8 nodeTypeCode);
    Executer *createExecuterByNotifyDataCode(quint8 notifyCode);
    Sensor *createSensorByTypeText(const QString &typeText);
    Sensor *createSensorByNotifyDataCode(quint8 notifyCode);
    Sensor *createSensorByNodeTypeCode(quint8 nodeTypeCode);
    TimeLineControlItem *createTimeLineControlItemByGroupTypeText(const QString &groupTypeText);
    TimeLineControlItem *createTimeLineControlItemByGroupType(NodeGroup::GroupType groupType);

private:
    explicit ObjectFactory(QObject *parent = 0);
    static ObjectFactory *m_objectFactory;
    static QMutex m_objectFactoryMutex;

    QMap<quint8, QString> m_notifyCodeToTypeTextMap;
    QMap<quint8, QString> m_nodeTypeCodeToTypeTextMap;
    QSet<quint8> m_sensorNodeTypeSet;
    QSet<quint8> m_sensorUserDataTypeCodeSet;
    QSet<quint8> m_executerNodeTypeSet;
    QSet<quint8> m_executerUserDataTypeCodeSet;
    void loadMetaObjectInfo();

signals:

public slots:
};

#endif // OBJECTFACTORY_H
