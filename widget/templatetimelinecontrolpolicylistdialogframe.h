#ifndef TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTDIALOGFRAME_H
#define TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTDIALOGFRAME_H

#include <QFrame>
#include "model/templatetimelinecontrolpolicylisteditmodel.h"
#include <QMenu>
#include <QAction>

namespace Ui {
class TemplateTimeLineControlPolicyListDialogFrame;
}

class MeshModel;
class NodeGroup;

class TemplateTimeLineControlPolicyListDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TemplateTimeLineControlPolicyListDialogFrame(MeshModel *meshModel, NodeGroup *group, QWidget *parent = 0);
    ~TemplateTimeLineControlPolicyListDialogFrame();

private slots:
    void on_buttonDialogBottomClose_clicked();

    void on_buttonAdd_clicked();

    void on_buttonRemove_clicked();

    void on_buttonEdit_clicked();

    void on_tableItemCollection_doubleClicked(const QModelIndex &index);

    void on_tableItemCollection_customContextMenuRequested(const QPoint &pos);

private:
    Ui::TemplateTimeLineControlPolicyListDialogFrame *ui;
    TemplateTimeLineControlPolicyListEditModel *m_templateTimeLineControlPolicyListEditModel;

    QMenu *m_menuMain;
    QAction *m_actionAdd;
    QAction *m_actionEdit;
    QAction *m_actionRemove;
    MeshModel *m_meshModel;
    NodeGroup *m_group;

    void initMenuAndAction();

signals:
    void reject();
};

#endif // TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTDIALOGFRAME_H
