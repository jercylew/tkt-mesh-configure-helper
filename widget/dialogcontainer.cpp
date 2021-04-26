#include "dialogcontainer.h"
#include "ui_dialogcontainer.h"
#include <QVBoxLayout>

DialogContainer::DialogContainer(bool closable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogContainer)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    setClosable(closable);
}

DialogContainer::~DialogContainer()
{
    delete ui;
}

void DialogContainer::setContentWidget(QWidget *widget)
{
    if(this->layout()==NULL)
    {
        QVBoxLayout *layout=new QVBoxLayout;
        this->setLayout(layout);
    }
    this->layout()->addWidget(widget);
    int iTitleBarHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight);
    this->resize(widget->width()+20, widget->height()+iTitleBarHeight);
}

void DialogContainer::setDialogTitle(const QString &title)
{
    this->setWindowTitle(title);
}

void DialogContainer::reject()
{
    if(m_closable)
    {
        QDialog::reject();
    }
}

void DialogContainer::setClosable(bool closable)
{
    this->m_closable=closable;
    if(closable)
    {
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    }
    else
    {
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    }
}
