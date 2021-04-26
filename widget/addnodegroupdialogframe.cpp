#include "addnodegroupdialogframe.h"
#include "ui_addnodegroupdialogframe.h"
#include <QMessageBox>
#include "dialogbuilder.h"
#include "model/meshmodel.h"
#include "model/nodegroupsmodel.h"

bool groupIdCmp (quint8 i, quint8 j) { return (i > j); }

AddNodeGroupDialogFrame::AddNodeGroupDialogFrame(MeshModel *meshModel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddNodeGroupDialogFrame),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    ui->buttonGroupColor->setStyleSheet("border:1px solid #444444;");

    ui->buttonGroupColor->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogOk->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogCancel->setCursor(Qt::PointingHandCursor);

    QList<QString> groupTypeList=NodeGroup::getGroupTypeTextList();
    for(int i=0; i<groupTypeList.size(); i++)
    {
        QString groupType=groupTypeList.at(i);
        ui->combGroupType->addItem(tr(groupType.toLatin1().data()), groupType);
    }

    setDefaultGroupId();
}

void AddNodeGroupDialogFrame::setDefaultGroupId()
{
    QList<NodeGroup *> lstGroup = m_meshModel->nodeGroupsModel()->getNodeGroupList();

    if (lstGroup.size() == 0)
    {
        ui->spinGroupID->setValue(1);
        return;
    }

    std::vector<quint8> vecId;
    for (int i = 0;i < lstGroup.size();i++)
    {
        vecId.push_back(lstGroup[i]->getId());
    }
    std::sort(vecId.begin(), vecId.end(), groupIdCmp);

    quint8 n = 1;
    quint8 max = vecId[0];

    while (n < max)
    {
        if (std::find(vecId.begin(), vecId.end(), n) == vecId.end())
        {
            break;
        }

        n++;
    }

    if (n >= max)
    {
        n = max + 1;
    }

    ui->spinGroupID->setValue(n);
}

AddNodeGroupDialogFrame::~AddNodeGroupDialogFrame()
{
    delete ui;
}

QString AddNodeGroupDialogFrame::getGroupName()
{
    return ui->lineGroupName->text();
}

QColor AddNodeGroupDialogFrame::getGroupColor()
{
    return color;
}

void AddNodeGroupDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void AddNodeGroupDialogFrame::on_buttonGroupColor_clicked()
{
    bool ok=false;
    QColor c=DialogBuilder::getInstance()->runGetColorDialog("Please Select Color", color, &ok);
    if(ok)
    {
        color=c;
        ui->buttonGroupColor->setStyleSheet(QString("border:none; background-color:%1;").arg(color.name()));
    }
}

void AddNodeGroupDialogFrame::on_buttonDialogOk_clicked()
{
    if(ui->lineGroupName->text().isEmpty())
    {
        DialogBuilder::getInstance()->buildAboutDialog(tr("Message"), tr("Please input Group Name!"))->exec();
        return;
    }
    if(m_meshModel->nodeGroupsModel()->getNodeGroup(ui->spinGroupID->value())!=NULL)
    {
        DialogBuilder::getInstance()->buildAboutDialog(tr("Message"), tr("The group ID you entered already exists!"))->exec();
        return;
    }
    NodeGroup::GroupType groupType=NodeGroup::getGroupTypeByText(ui->combGroupType->currentData().toString());
    m_meshModel->nodeGroupsModel()->addGroup(ui->spinGroupID->value(), this->getGroupName(), this->getGroupColor(), groupType);
    emit accept();
}

void AddNodeGroupDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
