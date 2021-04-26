#include "nofocuslineitemdelegate.h"
#include <QStyledItemDelegate>

NoFocusLineItemDelegate::NoFocusLineItemDelegate()
{

}

void NoFocusLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
    {
    itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    }
    QItemDelegate::paint(painter, itemOption, index);
}


