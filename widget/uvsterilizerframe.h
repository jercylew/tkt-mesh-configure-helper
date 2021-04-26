#ifndef UVSTERILIZERFRAME_H
#define UVSTERILIZERFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class UVSterilizerFrame;
}

class UVSterilizerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit UVSterilizerFrame(QWidget *parent = 0);
    ~UVSterilizerFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::UVSterilizerFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // ONECHANNELRELAYFRAME_H
