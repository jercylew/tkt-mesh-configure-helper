#ifndef TEMPLATETIMELINECONTROLITEMCOLLECTIONSELECTDIALOGFRAME_H
#define TEMPLATETIMELINECONTROLITEMCOLLECTIONSELECTDIALOGFRAME_H

#include <QFrame>
#include "model/templatetimelinecontrolpolicylisteditmodel.h"
#include "domain/timelinecontrolpolicy.h"
#include "model/templatetimelinecontrolitemlistmodel.h"
#include <QMovie>

class MeshModel;

namespace Ui {
class TemplateTimeLineControlPolicySelectDialogFrame;
}

class TemplateTimeLineControlPolicySelectDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TemplateTimeLineControlPolicySelectDialogFrame(MeshModel *meshModel, NodeGroup *group, int subId, QWidget *parent = 0);
    ~TemplateTimeLineControlPolicySelectDialogFrame();

private slots:
    void on_buttonDialogBottomClose_clicked();

    void on_buttonAdd_clicked();

    void on_buttonRemove_clicked();

    void on_buttonEdit_clicked();

    void on_tableItemCollection_doubleClicked(const QModelIndex &index);

    void on_buttonCancel_clicked();

    void on_buttonOk_clicked();

    void doTemplateTimeLineControlItemListModelModelChanged();

    void doAddControlItemsFromTemplateFinished();

private:
    Ui::TemplateTimeLineControlPolicySelectDialogFrame *ui;
    TemplateTimeLineControlPolicyListEditModel *m_templateTimeLineControlPolicyListEditModel;
    NodeGroup *m_group;
    MeshModel *m_meshModel;
    int m_subId;
    TemplateTimeLineControlItemListModel *m_selectedTemplateTimeLineControlItemListModel;

    QMovie *m_applyMovie;

signals:
    void reject();
    void accept();
};

#endif // TEMPLATETIMELINECONTROLITEMCOLLECTIONSELECTDIALOGFRAME_H
