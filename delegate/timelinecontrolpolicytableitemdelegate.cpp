#include "timelinecontrolpolicytableitemdelegate.h"

#include "model/timelinecontrolpolicylistmodel.h"
#include "domain/timelinecontrolpolicy.h"
#include <QPainter>
#include <QPixmap>

TimeLineControlPolicyTableItemDelegate::TimeLineControlPolicyTableItemDelegate(TimeLineControlPolicyListModel *timeLineControlPolicyListModel, QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_timeLineControlPolicyListModel=timeLineControlPolicyListModel;
}

TimeLineControlPolicyTableItemDelegate::~TimeLineControlPolicyTableItemDelegate()
{
}

void TimeLineControlPolicyTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==2)
    {
        TimeLineControlPolicy* controlPolicy=m_timeLineControlPolicyListModel->at(index.row());
        if(controlPolicy!=NULL)
        {
            QPixmap pixmap(20, 20);
            pixmap.fill(QColor(controlPolicy->color));
            int x = option.rect.center().x() - pixmap.rect().width() / 2;
            int y = option.rect.center().y() - pixmap.rect().height() / 2;
            if (option.state & QStyle::State_Selected) {
                painter->fillRect(option.rect, option.palette.highlight());
            }
            painter->drawPixmap(QRect(x, y, pixmap.rect().width(), pixmap.rect().height()), pixmap);
            return;
        }
    }
    QStyledItemDelegate::paint(painter, option, index);
}
