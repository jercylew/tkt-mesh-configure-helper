#ifndef NODEGROUPLISTVIEWITEMDELEGATE_H
#define NODEGROUPLISTVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

class MeshModel;
class NodeGroupFrame;

class NodeGroupListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NodeGroupListViewItemDelegate(MeshModel *meshModel, QObject *parent = 0);
    ~NodeGroupListViewItemDelegate();

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    MeshModel *m_meshModel;
    NodeGroupFrame *m_nodeGroupFrame;

signals:

public slots:
};

#endif // NODEGROUPLISTVIEWITEMDELEGATE_H
