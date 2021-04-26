#ifndef MESHRUNTIMEPARAMETERBOX_H
#define MESHRUNTIMEPARAMETERBOX_H

#include <QObject>

class MeshRuntimeParameterBox : public QObject
{
    Q_OBJECT
public:
    typedef enum{
        ExecuterOrderByNodeID,
        ExecuterOrderByNodeName,
        ExecuterOrderByOnlineStatus
    }ExecuterOrderMethod;

    typedef enum{
        SensorOrderByNodeID,
        SensorOrderByNodeName
    }SensorOrderMethod;

    typedef enum{
        NodeGroupOrderByGroupID,
        NodeGroupOrderByGroupName
    }NodeGroupOrderMethod;

    explicit MeshRuntimeParameterBox(QObject *parent = 0);

    void setExecuterOrderMethod(ExecuterOrderMethod executerOrderMethod);
    ExecuterOrderMethod executerOrderMethod();

    void setSensorOrderMethod(SensorOrderMethod sensorOrderMethod);
    SensorOrderMethod sensorOrderMethod();

    void setNodeGroupOrderMethod(NodeGroupOrderMethod nodeGroupOrderMethod);
    NodeGroupOrderMethod nodeGroupOrderMethod();

    void setExecuterListViewCurrentGroupId(quint8 groupId);
    quint8 executerListViewCurrentGroupId();

    void setExecuterListOnlyShowOnlineNodes(bool onlyShowOnline);
    bool executerListOnlyShowOnlineNodes();

    void setExecuterListViewScaleValue(double scaleValue);
    double executerListViewScaleValue();

    void setSensorListViewScaleValue(double scaleValue);
    double sensorListViewScaleValue();

    bool executerListOnlyShowAvailableNodes();
    void setExecuterListOnlyShowAvailableNodes(bool executerListOnlyShowAvailableNodes);

    bool sensorListOnlyShowAvailableNodes();
    void setSensorListOnlyShowAvailableNodes(bool sensorListOnlyShowAvailableNodes);

signals:

public slots:

private:
    ExecuterOrderMethod m_executerOrderMethod;
    SensorOrderMethod m_sensorOrderMethod;
    NodeGroupOrderMethod m_nodeGroupOrderMethod;

    quint8 m_executerListViewCurrentGroupId;
    bool m_executerListOnlyShowOnlineNodes;
    bool m_executerListOnlyShowAvailableNodes;
    double m_executerListViewScaleValue;

    double m_sensorListViewScaleValue;
    bool m_sensorListOnlyShowAvailableNodes;

};

#endif // MESHRUNTIMEPARAMETERBOX_H
