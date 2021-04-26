#include "uvsterilizerconfdialog.h"
#include "ui_uvsterilizerconfdialog.h"
#include "model/meshmodel.h"
#include "utils/commandtaskmanager.h"

#include <QGraphicsDropShadowEffect>
#include <QDebug>

UVSterilizerConfDialog::UVSterilizerConfDialog(MeshModel *meshModel, QList<quint16> addrList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UVSterilizerConfDialog),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    this->addrList=addrList;
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(ui->frameDialogContainer);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(Qt::black);
    shadow_effect->setBlurRadius(10);
    ui->frameDialogContainer->setGraphicsEffect(shadow_effect);

    ui->buttonDialogApply->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogClose->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);
    isGroupMode=false;
}

UVSterilizerConfDialog::UVSterilizerConfDialog(MeshModel *meshModel, quint8 groupId, QWidget *parent)
    :QDialog(parent),
     ui(new Ui::UVSterilizerConfDialog),
     m_meshModel(meshModel)
{
    ui->setupUi(this);
    this->groupId=groupId;

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(ui->frameDialogContainer);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(Qt::black);
    shadow_effect->setBlurRadius(10);
    ui->frameDialogContainer->setGraphicsEffect(shadow_effect);
    ui->buttonDialogClose->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);
    isGroupMode=true;
}

UVSterilizerConfDialog::~UVSterilizerConfDialog()
{
    delete ui;
}

void UVSterilizerConfDialog::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void UVSterilizerConfDialog::on_buttonDialogClose_clicked()
{
    this->accept();
}

void UVSterilizerConfDialog::on_buttonDialogBottomClose_clicked()
{
    this->accept();
}

void UVSterilizerConfDialog::on_buttonDialogApply_clicked()
{
    //UV Sterilizing parameters
    bool bAutoSterilizeMode = ui->radBtnAutoCycle;
    int nSterilizeTimeMin = ui->spinSterilizeTime->value();
    int nCycleTime = 0;

    //Currently, scheduling with Group mode not supported, only in Executer can send this command
    if (bAutoSterilizeMode)
    {
        nCycleTime = ui->spinCycleTime->value();
    }

    m_meshModel->commandTaskManager()->runUVSterilizingCommand(addrList, nSterilizeTimeMin, nCycleTime, bAutoSterilizeMode);
}
