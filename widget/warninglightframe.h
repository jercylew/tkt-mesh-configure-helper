#ifndef WARNINGLIGHTFRAME_H
#define WARNINGLIGHTFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class WarningLightFrame;
}

class WarningLightFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WarningLightFrame(QWidget *parent = 0);
    ~WarningLightFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setPower(const QString &power);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::WarningLightFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // WARNINGLIGHT_H
