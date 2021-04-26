#ifndef TEMPLATETIMELINECONTROLITEMTABLEVIEWMODEL_H
#define TEMPLATETIMELINECONTROLITEMTABLEVIEWMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "model/templatetimelinecontrolitemlistmodel.h"

class MeshModel;

class TemplateTimeLineControlItemTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TemplateTimeLineControlItemTableViewModel(MeshModel *meshModel, const QString &policyId, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    TemplateTimeLineControlItemListModel *templateTimeLineControlItemListModel();

private:
    TemplateTimeLineControlItemListModel *m_templateTimeLineControlItemListModel;
    MeshModel   *m_meshModel;

signals:

private slots:
    void doDataChanged();
    void doModelChanged();
};

#endif // TEMPLATETIMELINECONTROLITEMTABLEVIEWMODEL_H
