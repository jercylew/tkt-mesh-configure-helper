#ifndef TEMPLATETIMELINECONTROLITEMSEDITDIALOGFRAME_H
#define TEMPLATETIMELINECONTROLITEMSEDITDIALOGFRAME_H

#include <QFrame>
#include "model/templatetimelinecontrolitemlisteditmodel.h"
#include <QAction>
#include <QMenu>

namespace Ui {
class TemplateTimeLineControlItemsEditDialogFrame;
}

class MeshModel;

class TemplateTimeLineControlItemsEditDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TemplateTimeLineControlItemsEditDialogFrame(MeshModel *meshModel, NodeGroup *group,
                                                         const QString &policyId, QWidget *parent = 0);
    ~TemplateTimeLineControlItemsEditDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonAdd_clicked();

    void on_buttonRemove_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void updateGraph();

    void on_buttonDialogBottomClose_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::TemplateTimeLineControlItemsEditDialogFrame *ui;

    QString m_policyId;

    TemplateTimeLineControlItemListEditModel *m_templateTimeLineControlItemListEditModel;

    QMenu *m_menuMain;
    QAction *m_actionAdd;
    QAction *m_actionRemove;

    void initMenuAndAction();

signals:
    void accept();

private:
    MeshModel *m_meshModel;
    NodeGroup *m_group;
};

#endif // TEMPLATETIMELINECONTROLITEMSEDITDIALOGFRAME_H
