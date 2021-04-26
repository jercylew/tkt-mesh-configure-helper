#ifndef SENSORLISTMODEL_H
#define SENSORLISTMODEL_H

#include <QObject>
#include <QList>
#include "utils/meshruntimeparameterbox.h"

class Sensor;
class SensorsModel;
class MeshSavingBox;

class SensorListModel : public QObject
{
    Q_OBJECT
public:
    explicit SensorListModel(SensorsModel *sensorsModel, MeshSavingBox *meshSavingBox, QObject *parent = 0);

    void init();

    MeshRuntimeParameterBox::SensorOrderMethod orderMethod();
    void setOrderMethod(MeshRuntimeParameterBox::SensorOrderMethod orderMethod);

    bool onlyShowAvailableNodes();
    void setOnlyShowAvailableNodes(bool onlyShowAvailable);

    int count();
    Sensor *at(int index);

    double scale();
    void setScale(double scale);

    void scaleUp();
    void scaleDown();

    void refreshList();

public slots:
    void doSensorsModelDataChanged();
    void doSensorsModelModelChanged();

signals:
    void dataChanged();
    void modelChanged();

private:
    quint8 m_selectedGroupId;
    QList<Sensor *> m_sensorList;
    SensorsModel *m_sensorsModel;

    MeshSavingBox *m_savingBox;

public slots:
};

#endif // SENSORLISTMODEL_H
