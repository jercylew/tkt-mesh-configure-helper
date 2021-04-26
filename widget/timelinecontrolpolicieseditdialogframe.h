#ifndef TIMELINECONTROLITEMLISTSEDITDIALOGFRAME_H
#define TIMELINECONTROLITEMLISTSEDITDIALOGFRAME_H

#include <QFrame>
#include "model/meshmodel.h"
#include "model/timelinecontrolpolicytableviewmodel.h"
#include "widget/calendardropablebutton.h"

namespace Ui {
class TimeLineControlPoliciesEditDialogFrame;
}

class CustomOneYearCalendarFrame;

class TimeLineControlPoliciesEditDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TimeLineControlPoliciesEditDialogFrame(MeshModel *meshModel, NodeGroup *group, QWidget *parent = 0);
    ~TimeLineControlPoliciesEditDialogFrame();

private slots:
    void on_buttonDialogBottomClose_clicked();

    void on_tableControlPolicy_doubleClicked(const QModelIndex &index);

    void doDutyButtonPolicyDrop(int policyId);

    void doControlDateModelChanged();

    void on_buttonAdd_clicked();

    void on_buttonRemove_clicked();

private:
    Ui::TimeLineControlPoliciesEditDialogFrame *ui;
    MeshModel *m_meshModel;
    NodeGroup *m_group;
    TimeLineControlPolicyTableViewModel *m_timeLineControlPolicyTableViewModel;
    CustomOneYearCalendarFrame *m_customOneYearCalendarFrame;

    CalendarDropableButton *m_buttonOnDuty;
    CalendarDropableButton *m_buttonOffDuty;

    void updateDutyButtons();

signals:
    void accept();
};

#endif // TIMELINECONTROLITEMLISTSEDITDIALOGFRAME_H
