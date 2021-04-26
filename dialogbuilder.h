#ifndef DIALOGBUILDER_H
#define DIALOGBUILDER_H

#include <QObject>
#include <QDialog>
#include "domain/timelinecontrolitem.h"
#include "model/templatetimelinecontrolitemlisteditmodel.h"

class NodeGroup;
class PrepareMeshModel;
class TKTMeshModel;
class MeshModel;
class DashboardsModel;
class DashboardTableViewModel;
class MeshsModel;
class VirtualCom;
class Sensor;
class MeshRuntimeParams;
class DialogContainer;

class DialogBuilder : public QObject
{
    Q_OBJECT
public:
    explicit DialogBuilder(QObject *parent = 0);

    static DialogBuilder *getInstance();

    QDialog *buildRemoteConfigPrepareDialog(PrepareMeshModel *prepareMeshModel);

    QDialog *buildSaveMeshDBDataDialog(PrepareMeshModel *prepareMeshModel);

    DialogContainer *buildDialog(QWidget *content, const QString &title, bool closable=true);

    QDialog *buildQuestionDialog(const QString &title, const QString &question);

    QDialog *buildAboutDialog(const QString &title, const QString &text);

    QDialog *buildHelpAboutDialog(const QString &title);

    QDialog *buildTimeLineControlItemsEditDialog(MeshModel *meshModel, const QString &title, int itemListId, int policySubId, NodeGroup *group);

    QDialog *buildTimeLineControlItemListsEditDialog(MeshModel *meshModel, NodeGroup *group, const QString &title);

    QDialog *buildControlItemEditDialog(MeshModel *meshModel, const QString &title, TimeLineControlItem *item);

    QDialog *buildAddNodeGroupDialog(MeshModel *meshModel, const QString &title);

    QDialog *buildAddTimePointDialog(const QString &title, int *minutes);

    QDialog *buildRenameDialog(MeshModel *meshModel, const QString &title, const QString &type, const QList<QString> &idList);

    QDialog *buildRenameOneDialog(MeshModel *meshModel, const QString &title, const QString &type, const QString &id);

    QDialog *buildInputDialog(const QString &dialogTitle, const QString &inputTitle, QString *text);

    QDialog *buildSelectDateDialog(const QString &title, QDate *date);

    QDialog *buildResetAllNodesDialog(MeshModel *meshModel, const QString &title);

    QDialog *buildLanguageDialog(const QString &title, const QString &titleSource);

    QColor runGetColorDialog(const QString &title, const QColor &color, bool *ok);

    QDialog *buildVirtualSensorEditDialog(MeshModel *meshModel, Sensor *sensor, const QString &title);

    QDialog *buildAddControlPolicyDialog(TimeLineControlPolicy *timeLineControlPolicy, const QString &title);

    QDialog *buildExecuterSpecialConrolDialog(const QString &title, MeshModel *meshModel, const QVariant &id, bool isGroup);

    QDialog *buildExportTimeLineControlItemTemplateDialog(MeshModel *meshModel, NodeGroup *group, int policySubId, const QString &title);

    QDialog *buildTemplateManagementDialog(MeshModel *meshModel, NodeGroup *group, const QString &title);

    QDialog *buildTemplateTimeLineControlPolicySelectDialog(MeshModel *meshModel, NodeGroup *group, int subId, const QString &title);

    QDialog *buildAddTemplateTimeLineControlPolicyDialog(const QString &title, QString *name, QString *description);

    QDialog *buildTemplateControlItemsEditDialog(MeshModel *meshModel, NodeGroup *group, const QString &policyId, const QString &title);

    QDialog *buildTemplateControlItemEditDialog(MeshModel *meshModel, NodeGroup *group, TimeLineControlItem *item, const QString &title);

    QDialog *buildAllocSwitchKeyGroupDialog(MeshModel *meshModel, QString sensorId, const QString &title);

private:
    static DialogBuilder *m_dialogBuilder;

signals:

public slots:
};

#endif // DIALOGBUILDER_H
