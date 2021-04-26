#ifndef ADDCONTROLPOLICYDIALOGFRAME_H
#define ADDCONTROLPOLICYDIALOGFRAME_H

#include <QFrame>
#include "domain/timelinecontrolpolicy.h"

namespace Ui {
class AddControlPolicyDialogFrame;
}

class AddControlPolicyDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddControlPolicyDialogFrame(TimeLineControlPolicy *timeLineControlPolicy, QWidget *parent = 0);
    ~AddControlPolicyDialogFrame();

private slots:
    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

    void on_buttonControlPolicyColor_clicked();

private:
    Ui::AddControlPolicyDialogFrame *ui;
    TimeLineControlPolicy *m_timeLineControlPolicy;
    QColor m_color;

signals:
    void accept();
    void reject();
};

#endif // ADDCONTROLPOLICYDIALOGFRAME_H
