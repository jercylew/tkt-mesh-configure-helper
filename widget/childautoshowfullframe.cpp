#include "childautoshowfullframe.h"


ChildAutoShowFullFrame::ChildAutoShowFullFrame(QWidget *parent) : QFrame(parent)
{
    this->m_childWidget=NULL;
}

void ChildAutoShowFullFrame::setChildWidget(QWidget *childWidget)
{
    if(this->m_childWidget!=NULL)
    {
        this->m_childWidget->setVisible(false);
        this->m_childWidget->setParent(NULL);
    }
    this->m_childWidget=childWidget;
    if(this->m_childWidget!=NULL)
    {
        this->m_childWidget->setParent(this);
    }
    childShowFull();
}

void ChildAutoShowFullFrame::resizeEvent(QResizeEvent *event)
{
    childShowFull();
}

void ChildAutoShowFullFrame::childShowFull()
{
    if(this->m_childWidget==NULL)
    {
        return;
    }
    this->m_childWidget->move(0, 0);
    this->m_childWidget->resize(this->width(), this->height());
}
