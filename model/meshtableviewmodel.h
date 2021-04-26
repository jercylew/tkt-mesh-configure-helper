#ifndef MESHTABLEVIEWMODEL_H
#define MESHTABLEVIEWMODEL_H

#include <QObject>
#include <QAbstractTableModel>

class MeshListModel;
class MeshesModel;

class MeshTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MeshTableViewModel(MeshesModel *meshesModel, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    MeshesModel *m_meshesModel;
    MeshListModel *m_meshListModel;

signals:

public slots:
    void doDataChanged();
    void doModelChanged();
};

#endif // MESHTABLEVIEWMODEL_H
