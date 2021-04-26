#ifndef TKTMESHUPDATETABLEVIEWMODEL_H
#define TKTMESHUPDATETABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <QSet>

class TKTMeshesUpdateModel;
class TKTMeshUpdateListModel;

class TKTMeshUpdateTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TKTMeshUpdateTableViewModel(TKTMeshesUpdateModel *tktMeshesUpdateModel, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    TKTMeshesUpdateModel *m_tktMeshesUpdateModel;
    TKTMeshUpdateListModel *m_tktMeshUpdateListModel;

public slots:
    void doModelChanged();
    void doDataChanged();
};

#endif // TKTMESHUPDATETABLEVIEWMODEL_H
