#ifndef ADBOARDFRAME_H
#define ADBOARDFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class ADBoardFrame;
}

class ADBoardFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ADBoardFrame(QWidget *parent = 0);
    ~ADBoardFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setExecuterId(const QString &executerId);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::ADBoardFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // ADBOARDFRAME_H
