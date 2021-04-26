#include "statusdisplaydialog.h"
#include "ui_statusdisplaydialog.h"

StatusDisplayDialog::StatusDisplayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDisplayDialog)
{
    ui->setupUi(this);
    m_loadingMovie.setFileName(":/image/loading.gif");
    ui->labelAnimate->setMovie(&m_loadingMovie);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setModal(true);
}

StatusDisplayDialog::~StatusDisplayDialog()
{
    delete ui;
}

void StatusDisplayDialog::setStatusText(const QString &text, bool showAnimate)
{
    ui->labelStatus->setText(text);
    if(showAnimate)
    {
        ui->labelAnimate->setVisible(true);
        m_loadingMovie.start();
    }
    else
    {
        ui->labelAnimate->setVisible(false);
    }
}

void StatusDisplayDialog::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);
    m_loadingMovie.stop();
}
