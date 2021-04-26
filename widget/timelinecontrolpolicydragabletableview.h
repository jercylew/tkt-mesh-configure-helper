#ifndef TIMELINECONTROLITEMCOLLECTIONDRAGABLETABLEVIEW_H
#define TIMELINECONTROLITEMCOLLECTIONDRAGABLETABLEVIEW_H

#include <QWidget>
#include <QTableView>

class TimeLineControlPolicyDragableTableView : public QTableView
{
    Q_OBJECT
public:
    TimeLineControlPolicyDragableTableView(QWidget *parent=0);

protected:
    void startDrag(Qt::DropActions supportedActions);
};

#endif // TIMELINECONTROLITEMCOLLECTIONDRAGABLETABLEVIEW_H
