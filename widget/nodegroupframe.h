#ifndef NODEGROUPFRAME_H
#define NODEGROUPFRAME_H

#include <QFrame>

namespace Ui {
class NodeGroupFrame;
}

class NodeGroupFrame : public QFrame
{
    Q_OBJECT

public:
    explicit NodeGroupFrame(QWidget *parent = 0);
    ~NodeGroupFrame();

    void setGroupColor(const QColor &color);
    void setGroupName(const QString &name);
    void setNodeNumber(int nodeNumber);
    void setScheduleOn(bool on);
    void setGroupType(const QString &groupType);

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::NodeGroupFrame *ui;
};

#endif // NODEGROUPFRAME_H
