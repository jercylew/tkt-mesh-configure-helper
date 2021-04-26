#ifndef RESETALLNODEDIALOGFRAME_H
#define RESETALLNODEDIALOGFRAME_H

#include <QFrame>
#include <QPair>
#include <QList>
#include <QTimer>

namespace Ui {
class ResetAllNodeDialogFrame;
}

class MeshModel;

class ResetAllNodeDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ResetAllNodeDialogFrame(MeshModel *meshModel, QWidget *parent = 0);
    ~ResetAllNodeDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::ResetAllNodeDialogFrame *ui;
    QList<QPair<quint8, quint8> > pairList;
    QTimer runTimer;
    int currentIndex;
    MeshModel *m_meshModel;

signals:
    void reject();

private slots:
    void on_buttonDialogBottomClose_clicked();
    void doRunTimerTimeout();
};

#endif // RESETALLNODEDIALOGFRAME_H
