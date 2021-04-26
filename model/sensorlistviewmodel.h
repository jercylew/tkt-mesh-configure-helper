#ifndef SENSORLISTVIEWMODEL_H
#define SENSORLISTVIEWMODEL_H

#include <QAbstractListModel>
#include "sensorsmodel.h"

class SensorListModel;
class MeshModel;
class SensorFrame;

class SensorListViewModel : public QAbstractListModel
{
public:
    SensorListViewModel(MeshModel *meshModel, SensorsModel *sensorsModel, QObject *parent=0);
    ~SensorListViewModel();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    SensorListModel *sensorListModel();

private:
    SensorsModel *sensorsModel;

    SensorListModel *m_sensorListModel;
    MeshModel *m_meshModel;
};

#endif // SENSORLISTVIEWMODEL_H
