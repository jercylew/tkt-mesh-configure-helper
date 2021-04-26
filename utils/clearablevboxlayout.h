#ifndef CLEARABLEVBOXLAYOUT_H
#define CLEARABLEVBOXLAYOUT_H

#include <QVBoxLayout>
#include <QList>
#include <QLayoutItem>

class ClearableVBoxLayout : public QVBoxLayout
{
public:
    ClearableVBoxLayout(QObject *parent=0);

    void addItem(QLayoutItem *item);

    void clearAll();

private:
    QList<QLayoutItem *> layoutItemList;
};

#endif // CLEARABLEVBOXLAYOUT_H
