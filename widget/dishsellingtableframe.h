#ifndef DISHSELLINGTABLEFRAME_H
#define DISHSELLINGTABLEFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class DishSellingTableFrame;
}

class DishSellingTableFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DishSellingTableFrame(QWidget *parent = 0);
    ~DishSellingTableFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::DishSellingTableFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // DISHSELLINGTABLEFRAME_H
