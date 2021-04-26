#include "nodegrouplistviewitemdelegate.h"

#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/nodegroupframe.h"
#include <QPainter>
#include <QColor>

NodeGroupListViewItemDelegate::NodeGroupListViewItemDelegate(MeshModel *meshModel, QObject *parent) : QStyledItemDelegate(parent)
{
    this->m_meshModel=meshModel;
    m_nodeGroupFrame=new NodeGroupFrame;
}

NodeGroupListViewItemDelegate::~NodeGroupListViewItemDelegate()
{
    if(m_nodeGroupFrame)
    {
        m_nodeGroupFrame->deleteLater();
        m_nodeGroupFrame=NULL;
    }
}

void NodeGroupListViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==0)
    {
        NodeGroup *nodeGroup=m_meshModel->nodeGroupsModel()->getNodeGroup((quint8)(index.data(Qt::UserRole).toInt()));
        if(nodeGroup)
        {
                m_nodeGroupFrame->setGroupName(nodeGroup->getName());
                m_nodeGroupFrame->setGroupType(NodeGroup::getTextByGroupType(nodeGroup->getGroupType()));
                m_nodeGroupFrame->setGroupColor(QColor(nodeGroup->getColor()));
                m_nodeGroupFrame->setNodeNumber(m_meshModel->executersModel()->countOfGroup(nodeGroup->getId()));
                m_nodeGroupFrame->setMaximumSize(option.rect.width(), option.rect.height());
                m_nodeGroupFrame->setMinimumSize(option.rect.width(), option.rect.height());
                if(index.row()%2)
                {
                    m_nodeGroupFrame->setProperty("row", "even");
                }
                else
                {
                    m_nodeGroupFrame->setProperty("row", "odd");
                }
                if(nodeGroup->isScheduleOn())
                {
                    m_nodeGroupFrame->setScheduleOn(true);
                }
                else
                {
                    m_nodeGroupFrame->setScheduleOn(false);
                }
                m_nodeGroupFrame->setStyle(m_nodeGroupFrame->style());
                painter->drawPixmap(option.rect, m_nodeGroupFrame->grab());
                if(option.state & QStyle::State_Selected)
                {
                    painter->fillRect(option.rect, QColor(128, 128, 128, 60));
                }
                return;
        }
    }
    QStyledItemDelegate::paint(painter, option, index);
}
