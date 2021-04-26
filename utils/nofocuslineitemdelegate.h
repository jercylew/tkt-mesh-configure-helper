#ifndef NOFOCUSLINEITEMDELEGATE_H
#define NOFOCUSLINEITEMDELEGATE_H

#include <QItemDelegate>

class NoFocusLineItemDelegate : public QItemDelegate
{
public:
    NoFocusLineItemDelegate();

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // NOFOCUSLINEITEMDELEGATE_H
