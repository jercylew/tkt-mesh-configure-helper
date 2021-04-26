#include "clearablevboxlayout.h"

ClearableVBoxLayout::ClearableVBoxLayout(QObject *parent)
    : QVBoxLayout()
{

}

void ClearableVBoxLayout::addItem(QLayoutItem *item)
{
    QVBoxLayout::addItem(item);
    layoutItemList.push_back(item);
}

void ClearableVBoxLayout::clearAll()
{
    while(layoutItemList.size()>0)
    {
        this->removeItem(layoutItemList.takeFirst());
    }
}
