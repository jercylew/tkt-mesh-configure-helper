#ifndef TKTMESHTABLEVIEWMODEL_H
#define TKTMESHTABLEVIEWMODEL_H

#include <QAbstractTableModel>

class TKTMeshesModel;
class TKTMeshListModel;

class TKTMeshTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TKTMeshTableViewModel(TKTMeshesModel *tktMeshesModel, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    TKTMeshesModel *m_tktMeshesModel;
    TKTMeshListModel *m_tktMeshListModel;

private slots:
    void doModelChanged();
    void doDataChanged();
};

#endif // TKTMESHTABLEVIEWMODEL_H
