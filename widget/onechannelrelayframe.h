#ifndef ONECHANNELRELAYFRAME_H
#define ONECHANNELRELAYFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class OneChannelRelayFrame;
}

class OneChannelRelayFrame : public QFrame
{
    Q_OBJECT

public:
    explicit OneChannelRelayFrame(QWidget *parent = 0);
    ~OneChannelRelayFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::OneChannelRelayFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // ONECHANNELRELAYFRAME_H
