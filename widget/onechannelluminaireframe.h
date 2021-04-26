#ifndef ONECHANNELLUMINAIREFRAME_H
#define LUMINAIREFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class OneChannelLuminaireFrame;
}

class OneChannelLuminaireFrame : public QFrame
{
    Q_OBJECT

public:
    explicit OneChannelLuminaireFrame(QWidget *parent = 0);
    ~OneChannelLuminaireFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setPower(const QString &power);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::OneChannelLuminaireFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // ONECHANNELLUMINAIREFRAME_H
