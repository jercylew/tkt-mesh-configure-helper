#ifndef TIMELINECONTROLITEMCOLLECTIONTABLEITEMDELEGATE_H
#define TIMELINECONTROLITEMCOLLECTIONTABLEITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TimeLineControlPolicyListModel;

class TimeLineControlPolicyTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TimeLineControlPolicyTableItemDelegate(TimeLineControlPolicyListModel *timeLineControlPolicyListModel, QObject *parent=0);
    ~TimeLineControlPolicyTableItemDelegate();

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    TimeLineControlPolicyListModel *m_timeLineControlPolicyListModel;
};

#endif // TIMELINECONTROLITEMCOLLECTIONTABLEITEMDELEGATE_H
