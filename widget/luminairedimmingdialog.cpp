#include "luminairedimmingdialog.h"
#include "ui_luminairedimmingdialog.h"
#include "model/meshmodel.h"
#include "utils/commandtaskmanager.h"

#include <QGraphicsDropShadowEffect>
#include <QDebug>

LuminaireDimmingDialog::LuminaireDimmingDialog(MeshModel *meshModel, QList<quint16> addrList, int currentValue, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LuminaireDimmingDialog),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    this->addrList=addrList;
    ui->sliderDimming->setValue(currentValue);

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

LuminaireDimmingDialog::LuminaireDimmingDialog(MeshModel *meshModel, quint8 groupId, int currentValue, QWidget *parent)
    :QDialog(parent),
     ui(new Ui::LuminaireDimmingDialog),
     m_meshModel(meshModel)
{
    ui->setupUi(this);
    this->groupId=groupId;
    ui->sliderDimming->setValue(currentValue);

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

LuminaireDimmingDialog::~LuminaireDimmingDialog()
{
    delete ui;
}

void LuminaireDimmingDialog::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

quint8 LuminaireDimmingDialog::getCurrentBrightness()
{
    return (quint8)(ui->sliderDimming->value());
}

void LuminaireDimmingDialog::on_sliderDimming_valueChanged(int value)
{
    ui->spinDimmingDialogBrightnessValue->setValue(value);
}

void LuminaireDimmingDialog::on_buttonDialogClose_clicked()
{
    this->accept();
}

void LuminaireDimmingDialog::on_buttonDialogBottomClose_clicked()
{
    this->accept();
}

void LuminaireDimmingDialog::on_spinDimmingDialogBrightnessValue_valueChanged(int value)
{
    ui->sliderDimming->setValue(value);
}

void LuminaireDimmingDialog::on_buttonDialogApply_clicked()
{
    if(!isGroupMode)
    {
        m_meshModel->commandTaskManager()->runDimmingCommand(addrList, ui->sliderDimming->value());
    }
    else
    {
        if(this->groupId==0)
        {
            m_meshModel->commandTaskManager()->runAllDimmingCommand(ui->sliderDimming->value());
        }
        else
        {
            m_meshModel->commandTaskManager()->runGroupDimmingCommand(groupId, ui->sliderDimming->value());
        }
    }
}
