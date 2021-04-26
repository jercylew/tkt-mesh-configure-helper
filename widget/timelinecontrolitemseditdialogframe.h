#ifndef TIMELINECONTROLITEMSEDITDIALOG_H
#define TIMELINECONTROLITEMSEDITDIALOG_H

#include <QFrame>
#include "domain/timelinecontrolpolicy.h"
#include "model/timelinecontrolitemtableviewmodel.h"
#include <QMenu>

namespace Ui {
class TimeLineControlItemsEditDialogFrame;
}

class MeshModel;

class TimeLineControlItemsEditDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TimeLineControlItemsEditDialogFrame(MeshModel *meshModel, int controlPolicyId,
                                                 int policySubId, NodeGroup *group, QWidget *parent = 0);
    ~TimeLineControlItemsEditDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonAdd_clicked();

    void on_buttonRemove_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void updateGraph();

    void on_buttonDialogBottomClose_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_lineControlPolicyName_editingFinished();

    void on_lineControlPolicyDescription_editingFinished();

    void on_buttonColor_clicked();

    void on_buttonLoadFrameTemplate_clicked();

    void on_buttonExportTemplate_clicked();

private:
    Ui::TimeLineControlItemsEditDialogFrame *ui;
    int m_policySubId;
    int m_controlPolicyId;
    NodeGroup *m_group;
    MeshModel *m_meshModel;

    TimeLineControlItemTableViewModel *m_timeLineControlItemTableViewModel;
    QMenu *m_menuMain;
    QAction *m_actionAdd;
    QAction *m_actionDelete;
    QString m_color;

    void initMenuAndAction();

signals:
    void accept();
};

#endif // TIMELINECONTROLITEMSEDITDIALOG_H
