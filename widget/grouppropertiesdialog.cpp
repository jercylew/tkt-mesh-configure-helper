#include "grouppropertiesdialog.h"
#include "ui_grouppropertiesdialog.h"

#include <QGraphicsDropShadowEffect>

GroupPropertiesDialog::GroupPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupPropertiesDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(ui->frameDialogContainer);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(Qt::black);
    shadow_effect->setBlurRadius(10);
    ui->frameDialogContainer->setGraphicsEffect(shadow_effect);

    ui->buttonDialogClose->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);

    this->setWindowTitle(tr("GroupPropertiesDialog.GroupProperties"));
}

GroupPropertiesDialog::~GroupPropertiesDialog()
{
    delete ui;
}

void GroupPropertiesDialog::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void GroupPropertiesDialog::on_buttonDialogBottomClose_clicked()
{
    this->accept();
}

void GroupPropertiesDialog::on_buttonDialogClose_clicked()
{
    this->accept();
}
