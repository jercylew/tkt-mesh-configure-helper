#ifndef SENSORDATACOMPARISIONLISTMODEL_H
#define SENSORDATACOMPARISIONLISTMODEL_H

#include <QObject>
#include <model/meshmodel.h>

class SensorDataComparisionListModel : public QObject
{
    Q_OBJECT
public:
    explicit SensorDataComparisionListModel(QObject *parent = 0);

    int count();
    SensorDataComparision *at(int index);
    void append(SensorDataComparision *data);
    void update(int index, SensorDataComparision *data);
    void removeAt(int index);
    void clear();

private:
    QList<SensorDataComparision*> m_lstDataCmp;

signals:
    void dataChanged();
    void modelChanged();

};

#endif // SENSORDATACOMPARISIONLISTMODEL_H
