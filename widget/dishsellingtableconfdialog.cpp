#include "dishsellingtableconfdialog.h"
#include "ui_dishsellingtableconfdialog.h"
#include "model/meshmodel.h"
#include "utils/commandtaskmanager.h"

#include <QGraphicsDropShadowEffect>
#include <QDebug>

DishSellingTableConfDialog::DishSellingTableConfDialog(MeshModel *meshModel, QList<quint16> addrList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DishSellingTableConfDialog),
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

    ui->edtMaxTemp->setValidator(new QIntValidator(0, 100, this));
    ui->edtMaxWaterLevel->setValidator(new QIntValidator(50, 2000, this));
}

DishSellingTableConfDialog::DishSellingTableConfDialog(MeshModel *meshModel, quint8 groupId, QWidget *parent)
    :QDialog(parent),
     ui(new Ui::DishSellingTableConfDialog),
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

    ui->edtMaxTemp->setValidator(new QIntValidator(0, 100, this));
    ui->edtMaxWaterLevel->setValidator(new QIntValidator(50, 2000, this));
}

DishSellingTableConfDialog::~DishSellingTableConfDialog()
{
    delete ui;
}

void DishSellingTableConfDialog::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void DishSellingTableConfDialog::on_buttonDialogClose_clicked()
{
    this->accept();
}

void DishSellingTableConfDialog::on_buttonDialogBottomClose_clicked()
{
    this->accept();
}

void DishSellingTableConfDialog::on_buttonDialogApply_clicked()
{
    int nMaxTemp = ui->edtMaxTemp->text().toInt();
    int nMaxWaterLevel = ui->edtMaxWaterLevel->text().toInt();

    m_meshModel->commandTaskManager()->runDishSellingTableConfigCommand(addrList,
                     nMaxTemp, nMaxWaterLevel);
}
