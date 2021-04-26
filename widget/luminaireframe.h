#ifndef LUMINAIREFRAME_H
#define LUMINAIREFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class LuminaireFrame;
}

class LuminaireFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LuminaireFrame(QWidget *parent = 0);
    ~LuminaireFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setPower(const QString &power);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::LuminaireFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // LUMINAIREFRAME_H
