#ifndef LUMINAIRELISTVIEWMODEL_H
#define LUMINAIRELISTVIEWMODEL_H

#include <QAbstractListModel>
#include "model/executersmodel.h"

class ExecuterListModel;
class MeshModel;

class ExecuterListViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ExecuterListViewModel(MeshModel *meshModel, ExecutersModel *executersModel, QObject *parent=0);
    ~ExecuterListViewModel();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    ExecuterListModel *executerListModel();

private slots:
    void doListModelChanged();
    void doListDataChanged();

private:
    ExecutersModel *executersModel;
    ExecuterListModel *m_executerListModel;
    MeshModel *m_meshModel;
};

#endif // LUMINAIRELISTVIEWMODEL_H
