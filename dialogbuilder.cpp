#include "dialogbuilder.h"
#include "widget/dialogcontainer.h"
#include "widget/addnodegroupdialogframe.h"
#include "widget/remoteconfigpreparedialogframe.h"
#include "widget/savemeshdbdatadialogframe.h"

#include "widget/dialogcontainer.h"
#include "widget/questiondialogframe.h"
#include "widget/aboutdialogframe.h"
#include "widget/timelinecontrolitemseditdialogframe.h"
#include "widget/addnodegroupdialogframe.h"
#include "widget/addtimepointdialogframe.h"
#include "widget/renamedialogframe.h"
#include "widget/renameonedialogframe.h"
#include "widget/inputdialogframe.h"
#include "widget/getdatedialogframe.h"
#include "widget/resetallnodedialogframe.h"
#include <QColorDialog>
#include "model/meshmodel.h"
#include "domain/mesh.h"
#include "widget/timelinecontrolpolicieseditdialogframe.h"
#include "widget/exporttimelinecontrolitemtemplatedialogframe.h"
#include "widget/virtualmotionsensoreditdialogframe.h"
#include "widget/virtualswitchsensoreditdialogframe.h"
#include "widget/luxsensorfiltersettingdialogframe.h"
#include "widget/helpaboutframe.h"
#include "widget/languagedialogframe.h"
#include "widget/aboutdialogframe.h"
#include "widget/addcontrolpolicydialogframe.h"
#include "widget/controlitemeditdialogframeofluminairegroup.h"
#include "widget/controlitemeditdialogframeofadboardgroup.h"
#include "widget/controlitemeditdialogframeofrelaygroup.h"
#include "widget/adboardcontroldialogframe.h"
#include "widget/templatetimelinecontrolpolicylistdialogframe.h"
#include "widget/templatetimelinecontrolpolicyselectdialogframe.h"
#include "widget/addtemplatetimelinecontrolpolicydialogframe.h"
#include "widget/templatetimelinecontrolitemseditdialogframe.h"
#include "widget/allocswitchkeygroupframe.h"
#include "widget/setmeshinfodialogframe.h"
#include "widget/setgatewayiddialogframe.h"

#include "domain/sensor.h"
#include "domain/virtualmotionsensor.h"
#include "domain/virtualswitchsensor.h"
#include "domain/nodegroup.h"
#include "domain/executer.h"
#include "domain/adboard.h"
#include "model/nodegroupsmodel.h"
#include "model/executersmodel.h"

DialogBuilder* DialogBuilder::m_dialogBuilder=NULL;

DialogBuilder::DialogBuilder(QObject *parent) : QObject(parent)
{

}

DialogBuilder *DialogBuilder::getInstance()
{
    if(m_dialogBuilder==NULL)
    {
        m_dialogBuilder=new DialogBuilder;
    }
    return m_dialogBuilder;
}

QDialog *DialogBuilder::buildRemoteConfigPrepareDialog(PrepareMeshModel *parepareMeshModel)
{
    RemoteConfigPrepareDialogFrame *frame=new RemoteConfigPrepareDialogFrame(parepareMeshModel);
    DialogContainer *dialog=buildDialog(frame, "", false);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(close()));
    connect(frame, SIGNAL(dialogClosableChanged(bool)), dialog, SLOT(setClosable(bool)));
    return dialog;
}

QDialog *DialogBuilder::buildSaveMeshDBDataDialog(PrepareMeshModel *prepareMeshModel)
{
    SaveMeshDBDataDialogFrame *frame=new SaveMeshDBDataDialogFrame(prepareMeshModel);
    QDialog *dialog=buildDialog(frame, tr("Save Data"));
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

DialogContainer *DialogBuilder::buildDialog(QWidget *content, const QString &title, bool closable)
{
    DialogContainer *dialog=new DialogContainer(closable);
    dialog->setContentWidget(content);
    dialog->setDialogTitle(title);
    return dialog;
}

QDialog *DialogBuilder::buildQuestionDialog(const QString &title, const QString &question)
{
    QuestionDialogFrame *frame=new QuestionDialogFrame;
    frame->setQuestion(question);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildAboutDialog(const QString &title, const QString &text)
{
    AboutDialogFrame *frame=new AboutDialogFrame;
    frame->setText(text);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    return dialog;
}

QDialog *DialogBuilder::buildHelpAboutDialog(const QString &title)
{
    HelpAboutFrame *frame=new HelpAboutFrame;
    QDialog *dialog=buildDialog(frame, title);
    return dialog;
}

QDialog *DialogBuilder::buildTimeLineControlItemsEditDialog(MeshModel *meshModel, const QString &title, int itemListId, int policySubId, NodeGroup *group)
{
    TimeLineControlItemsEditDialogFrame *frame=new TimeLineControlItemsEditDialogFrame(meshModel, itemListId, policySubId, group);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    return dialog;
}

QDialog *DialogBuilder::buildTimeLineControlItemListsEditDialog(MeshModel *meshModel, NodeGroup *group, const QString &title)
{
    TimeLineControlPoliciesEditDialogFrame *frame=new TimeLineControlPoliciesEditDialogFrame(meshModel, group);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    return dialog;
}

QDialog *DialogBuilder::buildControlItemEditDialog(MeshModel *meshModel, const QString &title, TimeLineControlItem *item)
{
    NodeGroup *group=meshModel->nodeGroupsModel()->getNodeGroup(item->groupId);
    if(group==NULL)
    {
        return NULL;
    }
    QFrame *frame;
    if(group->getGroupType()==NodeGroup::ADBoardGroup)
    {
        frame =new ControlItemEditDialogFrameOfADBoardGroup(meshModel, item);
    }
    else if(group->getGroupType()==NodeGroup::RelayGroup)
    {
        frame=new ControlItemEditDialogFrameOfRelayGroup(meshModel, item);
    }
    else
    {
        frame=new ControlItemEditDialogFrameOfLuminaireGroup(meshModel, item);
    }
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildAddNodeGroupDialog(MeshModel *meshModel, const QString &title)
{
    AddNodeGroupDialogFrame *frame=new AddNodeGroupDialogFrame(meshModel);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildSetMeshInfoDialog(SwitchMeshInfo *switchMeshInfo, const QString &title)
{
    SetMeshInfoDialogFrame *frame = new SetMeshInfoDialogFrame(switchMeshInfo);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildSetGatewayIdDialog(quint8 *pGatewayId, const QString &title)
{
    SetGatewayIdDialogFrame *frame = new SetGatewayIdDialogFrame(pGatewayId);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildAddTimePointDialog(const QString &title, int *minutes)
{
    AddTimePointDialogFrame *frame=new AddTimePointDialogFrame(minutes);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildRenameDialog(MeshModel *meshModel, const QString &title, const QString &type, const QList<QString> &idList)
{
    RenameDialogFrame *frame=new RenameDialogFrame(meshModel, type, idList);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildRenameOneDialog(MeshModel *meshModel, const QString &title, const QString &type, const QString &id)
{
    RenameOneDialogFrame *frame=new RenameOneDialogFrame(meshModel, id, type);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildInputDialog(const QString &dialogTitle, const QString &inputTitle, QString *text)
{
    InputDialogFrame *frame=new InputDialogFrame(text, inputTitle);

    QDialog *dialog=buildDialog(frame, dialogTitle);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildSelectDateDialog(const QString &title, QDate *date)
{
    GetDateDialogFrame *frame=new GetDateDialogFrame(date);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildResetAllNodesDialog(MeshModel *meshModel, const QString &title)
{
    ResetAllNodeDialogFrame *frame=new ResetAllNodeDialogFrame(meshModel);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildLanguageDialog(const QString &title, const QString &titleSource)
{
    LanguageDialogFrame *frame=new LanguageDialogFrame;
    QDialog *dialog=buildDialog(frame, title);
    dialog->setProperty("DialogTitleSource", titleSource);
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QColor DialogBuilder::runGetColorDialog(const QString &title, const QColor &color, bool *ok)
{
    QColorDialog *colorDialog=new QColorDialog(color);
    colorDialog->resize(530, 450);
    QDialog *dialog=buildDialog(colorDialog, title);
    connect(colorDialog, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(colorDialog, SIGNAL(rejected()), dialog, SLOT(reject()));
    if(dialog->exec()==QDialog::Accepted)
    {
        *ok=true;
    }
    else
    {
        *ok=false;
    }
    return colorDialog->selectedColor();
}

QDialog *DialogBuilder::buildVirtualSensorEditDialog(MeshModel *meshModel, Sensor *sensor, const QString &title)
{
    if(!sensor->isVirtual())
    {
        return NULL;
    }

    if(sensor->sensorType()==Sensor::VirtualMotionSensor)
    {
        VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
        if(!virtualMotionSensor)
        {
            return NULL;
        }
        VirtualMotionSensorEditDialogFrame *frame=new VirtualMotionSensorEditDialogFrame(meshModel, virtualMotionSensor);
        QDialog *dialog=buildDialog(frame, title);
        connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
        connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
        return dialog;
    }
    else if(sensor->sensorType()==Sensor::VirtualSwitchSensor)
    {
        VirtualSwitchSensor *virtualSwitchSensor=static_cast<VirtualSwitchSensor*>(sensor);
        if(!virtualSwitchSensor)
        {
            return NULL;
        }
        VirtualSwitchSensorEditDialogFrame *frame=new VirtualSwitchSensorEditDialogFrame(meshModel, virtualSwitchSensor);
        QDialog *dialog=buildDialog(frame, title);
        connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
        connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
        return dialog;
    }
    else
    {
        return NULL;
    }
}

QDialog *DialogBuilder::buildAddControlPolicyDialog(TimeLineControlPolicy *timeLineControlPolicy, const QString &title)
{
    AddControlPolicyDialogFrame *frame=new AddControlPolicyDialogFrame(timeLineControlPolicy);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildExecuterSpecialConrolDialog(const QString &title, MeshModel *meshModel, const QVariant &id, bool isGroup)
{
    QFrame *frame=NULL;
    if(isGroup)
    {
        NodeGroup *nodeGroup=meshModel->nodeGroupsModel()->getNodeGroup(id.toInt());
        if(nodeGroup==NULL)
        {
            return NULL;
        }
        if(nodeGroup->getGroupType()==NodeGroup::ADBoardGroup)
        {
            frame=new ADBoardControlDialogFrame(meshModel, id, true);
        }
    }
    else
    {
        Executer *executer=meshModel->executersModel()->getExecuterByExecuterId(id.toString());
        if(executer==NULL || (!executer->hasSpecialControl()))
        {
            return NULL;
        }
        if(executer->typeText()==ADBoard::staticTypeText())
        {
            frame=new ADBoardControlDialogFrame(meshModel, id, false);
        }
    }
    if(frame==NULL)
    {
        return NULL;
    }
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    return dialog;
}

QDialog *DialogBuilder::buildExportTimeLineControlItemTemplateDialog(MeshModel *meshModel, NodeGroup *group, int policySubId, const QString &title)
{
    ExportTimeLineControlItemTemplateDialogFrame *frame=new ExportTimeLineControlItemTemplateDialogFrame(meshModel, group, policySubId);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildTemplateManagementDialog(MeshModel *meshModel, NodeGroup *group, const QString &title)
{
    TemplateTimeLineControlPolicyListDialogFrame *frame=new TemplateTimeLineControlPolicyListDialogFrame(meshModel, group);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildTemplateTimeLineControlPolicySelectDialog(MeshModel *meshModel, NodeGroup *group, int subId, const QString &title)
{
    TemplateTimeLineControlPolicySelectDialogFrame *frame=new TemplateTimeLineControlPolicySelectDialogFrame(meshModel, group, subId);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildAddTemplateTimeLineControlPolicyDialog(const QString &title, QString *name, QString *description)
{
    AddTemplateTimeLineControlPolicyDialogFrame *frame=new AddTemplateTimeLineControlPolicyDialogFrame(name, description);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    return dialog;
}

QDialog *DialogBuilder::buildTemplateControlItemsEditDialog(MeshModel *meshModel, NodeGroup *group,  const QString &policyId, const QString &title)
{
    TemplateTimeLineControlItemsEditDialogFrame *frame=new TemplateTimeLineControlItemsEditDialogFrame(meshModel, group, policyId);
    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    return dialog;
}

QDialog *DialogBuilder::buildTemplateControlItemEditDialog(MeshModel *meshModel, NodeGroup *group,
                                                           TimeLineControlItem *item, const QString &title)
{
    if(group==NULL)
    {
        return NULL;
    }
    QFrame *frame;
    if(group->getGroupType()==NodeGroup::ADBoardGroup)
    {
        frame = new ControlItemEditDialogFrameOfADBoardGroup(meshModel, item);
    }
    else if(group->getGroupType()==NodeGroup::RelayGroup)
    {
        frame = new ControlItemEditDialogFrameOfRelayGroup(meshModel, item);
    }
    else
    {
        frame = new ControlItemEditDialogFrameOfLuminaireGroup(meshModel, item);
    }

    QDialog *dialog=buildDialog(frame, title);
    connect(frame, SIGNAL(accept()), dialog, SLOT(accept()));
    connect(frame, SIGNAL(reject()), dialog, SLOT(reject()));
    return dialog;
}

QDialog *DialogBuilder::buildAllocSwitchKeyGroupDialog(MeshModel *meshModel, QString sensorId, const QString &title)
{
    QFrame *ptrFrame = new AllocSwitchKeyGroupFrame(meshModel, sensorId);
    QDialog *ptrDialog = buildDialog(ptrFrame, title);
    connect(ptrFrame, SIGNAL(accept()), ptrDialog, SLOT(accept()));
    connect(ptrFrame, SIGNAL(reject()), ptrDialog, SLOT(reject()));

    return ptrDialog;
}

