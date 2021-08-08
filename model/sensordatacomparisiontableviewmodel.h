#ifndef SENSORDATACOMPARISIONTABLEVIEWMODEL_H
#define SENSORDATACOMPARISIONTABLEVIEWMODEL_H

#include <QAbstractTableModel>

class SensorDataComparisionListModel;

class SensorDataComparisionTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SensorDataComparisionTableViewModel(SensorDataComparisionListModel *dataCmpListModel,
                                        QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    SensorDataComparisionListModel *m_ptrSensorDataCmpListModel;

private slots:
    void doModelChanged();
    void doDataChanged();
};

#endif // SENSORDATACOMPARISIONTABLEVIEWMODEL_H
