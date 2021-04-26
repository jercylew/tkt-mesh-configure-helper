//#include "controlitemtabledelegate.h"
//#include "widget/controlitemeditdialogframe.h"
//#include "timelinecontrolmanager.h"

//ControlItemTableDelegate::ControlItemTableDelegate(quint8 groupId, QObject *parent)
//    : QItemDelegate(parent)
//{
//    this->groupId=groupId;
//}

//QWidget *ControlItemTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    ControlItemEditDelegateFrame *frame=new ControlItemEditDelegateFrame;
//    return frame;
//}

//void ControlItemTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    ControlItemEditDelegateFrame *frame=static_cast<ControlItemEditDelegateFrame*>(editor);
//    QList<TimeLineControlItem*> *list=TimeLineControlManager::getInstance()->getTimeLineControlItemsModel()->getTimeLineControlItemList(groupId);
//    TimeLineControlItem* item=list->at(index.row());
//    frame->setControlItem(item);
//}

//void ControlItemTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//    QItemDelegate::setModelData(editor, model, index);
//}

//void ControlItemTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    editor->move(option.rect.x(), option.rect.y()+option.rect.height());
//}
