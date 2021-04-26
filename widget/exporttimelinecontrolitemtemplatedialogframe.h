#ifndef EXPORTTIMELINECONTROLITEMTEMPLATEDIALOGFRAME_H
#define EXPORTTIMELINECONTROLITEMTEMPLATEDIALOGFRAME_H

#include <QFrame>
#include "domain/timelinecontrolpolicy.h"
#include <QMovie>

class MeshModel;
class NodeGroup;

namespace Ui {
class ExportTimeLineControlItemTemplateDialogFrame;
}

class ExportTimeLineControlItemTemplateDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ExportTimeLineControlItemTemplateDialogFrame(MeshModel *meshModel, NodeGroup *group, int policySubId, QWidget *parent = 0);
    ~ExportTimeLineControlItemTemplateDialogFrame();

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

    void doSaveTemplateItemListAsNewCollectionFinished();

private:
    Ui::ExportTimeLineControlItemTemplateDialogFrame *ui;
    MeshModel   *m_meshModel;
    NodeGroup   *m_group;
    int         m_policySubId;
    QMovie      *m_applyMovie;

signals:
    void accept();
    void reject();
};

#endif // EXPORTTIMELINECONTROLITEMTEMPLATEDIALOGFRAME_H
