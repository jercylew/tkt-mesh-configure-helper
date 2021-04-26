#include "timelinecontrolpolicydragabletableview.h"
#include <QMimeData>
#include <QDrag>

TimeLineControlPolicyDragableTableView::TimeLineControlPolicyDragableTableView(QWidget *parent)
    : QTableView(parent)
{

}

void TimeLineControlPolicyDragableTableView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndex index=this->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    QMimeData *mimeData=new QMimeData;
    QString policyId=index.data(Qt::UserRole).toString();
    mimeData->setData("application/taikwan-meshlight-item-policy-id", policyId.toLatin1());
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(supportedActions, Qt::MoveAction);
    drag->deleteLater();
}
