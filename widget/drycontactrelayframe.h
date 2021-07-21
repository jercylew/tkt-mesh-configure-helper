#ifndef DRYCONTACTRELAYFRAME_H
#define DRYCONTACTRELAYFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class DryContactRelayFrame;
}

class DryContactRelayFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DryContactRelayFrame(QWidget *parent = 0);
    ~DryContactRelayFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::DryContactRelayFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // DRYCONTACTRELAYFRAME_H
